#ifndef DATA_IO_MAIN
#define DATA_IO_MAIN

#include <iostream>

#include <boost/program_options.hpp>

#include <data/types.hpp>
#include <data/list/linked.hpp>

// Ways of translating the c++ main function into something
// a little more high-level. 
namespace data {
    
    struct main {
        virtual int operator()(int argc, char *argv[]) const = 0;
    };
    
    namespace program {
        namespace po = boost::program_options;
        
        using input = po::variables_map;
    
        struct output {
            uint Error;
            string Response;
            
            output() : Error{0}, Response{} {}
            output(string r) : Error{0}, Response{r} {}
            output(uint e, string r) : Error{e}, Response{r} {}
        };
        
        template <typename input>
        struct input_parser {
            virtual input operator()(int argc, char *argv[]) const = 0;
        };
        
        struct environment {
            std::istream& In;
            std::ostream& Out;
            std::ostream& Error;
                
            environment(std::istream& in, std::ostream& out, std::ostream& err) : In{in}, Out{out}, Error{err} {}
            
            template <typename f> struct main;
            
            template <typename f>
            data::main& operator()(f fun) {
                return *new main<f>{fun};
            }
        };
        
        template <typename f>
        struct environment::main : public data::main {
            f fun;
            environment Environment;
            
            int operator()(int argc, char *argv[]){
                output o = fun(argc, argv);
                (o.Error ? Environment.Error : Environment.Out) << o.Response << std::endl;
                return o.Error;
            }
        };
        
        template <typename f>
        struct catch_all {
            f& Function;
            output operator()(int argc, char *argv[]) {
                try {
                    return Function(argc, argv);
                } catch(std::exception& e) { 
                    return {true, e.what()}; 
                } 
            }
            
            catch_all(f& fun) : Function{fun} {}
        };
        
        struct boost_input_parser : public input_parser<input> {
            po::options_description Named; 
            po::positional_options_description Positional;
                
            input operator()(int argc, char *argv[]) const {
                input vm;
                po::store(po::command_line_parser(argc, argv).options(Named).positional(Positional).run(), vm);
                po::notify(vm);
                return vm;
            }
            
            boost_input_parser(po::options_description n, po::positional_options_description p)
                : Named{n}, Positional{p} {}
        };
        
        template <typename L>
        struct list_input_parser : public input_parser<L> {
            L operator()(int argc, char *argv[]) {
                L list{};
                for (uint i = 0; i < argc; i++){
                    list = list + std::string(argv[i]);
                } 
                return list;
            }
        };
        
        template <typename input>
        data::main& main(output (* fun)(input), input_parser<input>& parser){
            
            struct program_main : public data::main {
                output (* Fun)(input);
                input_parser<input>& Parser;
                
                output operator()(int argc, char *argv[]){
                    return Fun(Parser(argc, argv));
                }
            };
            
            return environment{std::cin, std::cout, std::cerr}(
                catch_all<program_main>{program_main{fun, parser}});
        }
    
    }
    
}

#endif
