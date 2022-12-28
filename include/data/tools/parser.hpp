// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_PARSER
#define DATA_TOOLS_PARSER

#include <data/tools/expression.hpp>

namespace data::parse {
    
    struct exception {};
    
    template <typename word, typename X>
    struct parser : data::parser<word, X> {
        virtual bool read(char) = 0;
        virtual bool match() = 0;
        
        // throw an exception if a match fails. 
        virtual stack<X> apply() const = 0;
        
        stack<X> Stack;
        
        parser(stack<X> x): Stack{x} {}
    };
    
    template <typename word, typename X>
    struct rule {
        virtual parser<X> invoke(stack<X>) const = 0;
    };
    
    template <typename X>
    struct tokenizer final: parser<X> {
        ptr<parser<X>> Parser;
        string Buffer;
        
        bool read(char x) final override {
            bool r = Parser->read(x); 
            if (r) Buffer.push_back(x);
            return r;
        }
        
        bool match() final override {
            return Parser->match(); 
        }
        
        std::function<X(string)> Make;
        
        stack<X> apply() final override {
            auto z = Parser->apply() << Make(Buffer);
            Buffer.clear();
            return z;
        }
        
        tokenizer(stack<X> x, ptr<rule<X>> p, std::function<X(string)> m): parser<X>{x}, 
            Parser{p->invoke(x)}, Buffer{}, Make{m} {}
    };
    
    template <typename X>
    struct application final: parser<X> {
        ptr<parser<X>> Parser;
        
        bool read(char x) final override {
            return Parser->read(x); 
        }
        
        bool match() final override {
            return Parser->match(); 
        }
        
        std::function<stack<X>(stack<X>)> Apply;
        
        stack<X> apply() final override {
            return Apply(this->Stack);
        }
        
        application(stack<X> x, ptr<parser<X>> p, std::function<stack<X>(stack<X>)> a): 
            parser<X>{x}, Parser{p}, Apply{a} {}
    };
    
    struct state {
        grammar Grammar;
        stack<rule> Reading;
        string Buffer;
        list<string> Tokens;
        
        void token() {
            Tokens = Tokens << Buffer;
            Buffer = string{};
        }
        
        
    };
    
    // read a character 
    
    // if match succeeds, add to buffer 
    
}

#endif
