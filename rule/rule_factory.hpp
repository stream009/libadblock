#ifndef RULE_FACTORY_HPP
#define RULE_FACTORY_HPP

class Rule;

class RuleFactory
{
public:
    template<typename Str>
    std::unique_ptr<Rule> create(Str&&);
};

#include "comment_rule.hpp"
#include "basic_filter_rule.hpp"
#include "exception_filter_rule.hpp"
#include "basic_element_hide_rule.hpp"
#include "exception_element_hide_rule.hpp"

#include <utility>

#include <boost/make_unique.hpp>

template<typename Str>
inline std::unique_ptr<Rule> RuleFactory::
create(Str &&line)
{
    if (CommentRule::matchFormat(line)) {
        return boost::make_unique<CommentRule>(std::forward<Str>(line));
    }
    else if (BasicElementHideRule::matchFormat(line)) {
        return boost::make_unique<BasicElementHideRule>(std::forward<Str>(line));
    }
    else if (ExceptionElementHideRule::matchFormat(line)) {
        return boost::make_unique<ExceptionElementHideRule>(std::forward<Str>(line));
    }
    else if (ExceptionFilterRule::matchFormat(line)) {
        return boost::make_unique<ExceptionFilterRule>(std::forward<Str>(line));
    }
    else {
        return boost::make_unique<BasicFilterRule>(std::forward<Str>(line));
    }
}

#endif // RULE_FACTORY_HPP
