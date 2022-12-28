#ifndef DATA_EXPRESSION
#define DATA_EXPRESSION

#include <data/tools.hpp>

namespace data {
    
    template <typename head, typename expr>
    struct call {
        head Head;
        stack<expr> Body;
    };
    
    template <typename head, typename expr>
    bool operator==(const call<head, expr> &, const call<head, expr> &);
    
    template <typename leaf, typename branch>
    struct expression : variant<leaf, call<branch, expression<leaf, branch>>> {
        expression(const leaf &);
        expression(const branch &, stack<expression>);
    };
    
    template <typename leaf, typename branch>
    bool operator==(const expression<leaf, branch> &, const expression<leaf, branch> &);
    
    template <typename leaf, typename branch> 
    using rule = entry<expression<leaf, branch>, expression<leaf, branch>>;
    
    template <typename leaf, typename branch> 
    using rules = stack<rule<leaf, branch>>;
    
    template <typename leaf, typename branch> 
    expression<leaf, branch> replace(const expression<leaf, branch> &, const rule<leaf, branch> &);
    
    template <typename leaf, typename branch> 
    expression<leaf, branch> replace(const expression<leaf, branch> &, rules<leaf, branch>);
    
    template <typename leaf, typename branch> 
    expression<leaf, branch> replace_all(const expression<leaf, branch> &, const rule<leaf, branch> &);
    
    template <typename leaf, typename branch> 
    expression<leaf, branch> replace_all(const expression<leaf, branch> &, rules<leaf, branch>);
    
    template <typename leaf, typename branch> struct any;
    template <typename leaf, typename branch> struct named;
    template <typename leaf, typename branch> struct qualified;
    template <typename leaf, typename branch> struct repeated;
    
    template <typename leaf, typename branch> 
    using pattern = expression<variant<leaf, any<leaf, branch>, named<leaf, branch>, qualified<leaf, branch>>, branch>;
    
    template <typename leaf, typename branch> 
    using matches = optional<map<expression<leaf, branch>, stack<expression<leaf, branch>>>>;
    
    template <typename leaf, typename branch>
    matches<leaf, branch> match(const expression<leaf, branch>, pattern<leaf, branch>);
    
    template <typename leaf, typename branch> struct any {};
    
    template <typename leaf, typename branch> struct named {
        expression<leaf, branch> Name;
        pattern<leaf, branch> Pattern;
    };
    
    template <typename leaf, typename branch> struct qualified {
        proposition<expression<leaf, branch>> Qualification;
        pattern<leaf, branch> Pattern;
    };
    
    template <typename leaf, typename branch> struct repeated {
        pattern<leaf, branch> Pattern;
        uint32 Min, Max;
    };
    
    template <typename leaf, typename branch> leaf_definition = entry<leaf, expression<leaf, branch>>;
    
    template <typename leaf, typename branch> branch_definition = entry<call<branch, pattern<leaf, branch>>, expression<leaf, branch>>;
    
    template <typename leaf, typename branch>
    matches<leaf, branch> match(const expression<leaf, branch>, pattern<leaf, branch>, matches<leaf, branch>);
    
    template <typename head, typename expr>
    bool inline operator==(const call<head, expr> &left, const call<head, expr> &right) {
        return left.Head == right.Head && left.Body == right.Body;
    }
    
    template <typename leaf, typename branch>
    bool inline operator==(const expression<leaf, branch> &left, const expression<leaf, branch> &right) {
        return static_cast<const variant<leaf, call<branch, expression<leaf, branch>>>>(left) == 
            static_cast<const variant<leaf, call<branch, expression<leaf, branch>>>>(right);
    }
    
    template <typename leaf, typename branch>
    expression<leaf, branch> inline replace(const expression<leaf, branch> &expr, 
        entry<expression<leaf, branch>, rule<leaf, branch>> r) {
        return expr == r.Key ? r.Value : exp;
    }
    
    template <typename leaf, typename branch>
    expression<leaf, branch> inline replace(const expression<leaf, branch> &expr, rules<leaf, branch> rr) {
        for (const auto &r : rr) if (expr == r.Key) return r.Value;
        return expr;
    }
    
    template <typename leaf, typename branch>
    expression<leaf, branch> replace_all(const expression<leaf, branch> &expr, rule<leaf, branch> r) {
        if (expr == r.Key) return r.Value;
        return expression<leaf, branch>{replace_all(expr.Head, r), 
            for_each([r](const expression<leaf, branch> &expr) -> expression<leaf, branch> {
                return replace_all(expr, r);
            }, expr.Body)};
    }
    
    template <typename leaf, typename branch>
    expression<leaf, branch> replace_all(const expression<leaf, branch> &expr, rules<leaf, branch> rr) {
        for (const auto &r : rr) if (expr == r.Key) return r.Value;
        return expression<leaf, branch>{replace_all(expr.Head, rr), 
            for_each([rr](const expression<leaf, branch> &expr) -> expression<leaf, branch> {
                return replace_all(expr, rr);
            }, expr.Body)};
    }
    
    template <typename leaf, typename branch>
    matches<leaf, branch> inline match(const expression<leaf, branch> expr, pattern<leaf, branch> patt) {
        return match(expr, patt, {});
    }
    
}

#endif

