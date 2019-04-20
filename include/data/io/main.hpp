#ifndef DATA_IO_MAIN
#define DATA_IO_MAIN

#include <iostream>

#include <boost/program_options.hpp>

#include <data/types.hpp>
#include <data/list/linked_list.hpp>

namespace data {
    
    namespace program {
        namespace po = boost::program_options;
        
        using input = po::variables_map;
    
        struct output {
            bool Error;
            string Response;
            
            output() : Error{false}, Response{} {}
            output(string r) : Error{false}, Response{r} {}
            output(bool e, string r) : Error{e}, Response{r} {}
        };
        
        struct environment {
            std::istream& In;
            std::ostream& Out;
            std::ostream& Error;
                
            environment(std::istream& in, std::ostream& out, std::ostream& err) : In{in}, Out{out}, Error{err} {}
            
            template <typename f>
            int operator()(f fun, int argc, char *argv[]) {
                output o = fun(argc, argv);
                (o.Error ? Error : Out) << o.Response << std::endl;
                return o.Error;
            }
        };
        
        template <typename f>
        struct catch_all {
            f Function;
            output operator()(int argc, char *argv[]) {
                try {
                    return Function(argc, argv);
                } catch(std::exception& e) { 
                    return {true, e.what()}; 
                } 
            }
            
            catch_all(f fun) : Function{fun} {}
        };
        
        struct input_parser {
            po::options_description Named; 
            po::positional_options_description Positional;
                
            input operator()(int argc, char *argv[]) {
                input vm;
                po::store(po::command_line_parser(argc, argv).options(Named).positional(Positional).run(), vm);
                po::notify(vm);
                return vm;
            }
            
            input_parser(po::options_description n, po::positional_options_description p)
                : Named{n}, Positional{p} {}
        };
        
        template <typename f>
        struct parse_input {
            f Function;
            input_parser InputParser;
            
            output operator()(int argc, char *argv[]) {
                return Function(InputParser(argc, argv));
            }
            
            parse_input(f fun, input_parser ip) : Function{fun}, InputParser{ip} {}
        };
        
        struct main {
            output (* Function)(input);
            input_parser InputParser;
            
            int operator()(int argc, char *argv[]){
                return environment{std::cin, std::cout, std::cerr}(
                    catch_all{parse_input{Function, InputParser}}, argc, argv);
            }
        };
    
    }
    
}

#endif
