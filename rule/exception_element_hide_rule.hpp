#ifndef EXCEPTION_ELEMENT_HIDE_RULE_HPP
#define EXCEPTION_ELEMENT_HIDE_RULE_HPP

#include "element_hide_rule.hpp"

class ExceptionElementHideRule : public ElementHideRule
{
    using Base = ElementHideRule;
public:
    ExceptionElementHideRule() = default;

    template<typename Str>
    ExceptionElementHideRule(Str&&);

    template<typename Str>
    static bool matchFormat(Str&&);
};

#include <utility>

#include <boost/regex.hpp>

template<typename Str>
ExceptionElementHideRule::
ExceptionElementHideRule(Str &&line)
    : Base { std::forward<Str>(line) }
{
    //TODO assert line
    //TODO split domain part and selector part
#if 0
    this->parseDomain(domainR);
    this->setCssSelector(selectorR);
#endif
}

template<typename Str>
bool ExceptionElementHideRule::
matchFormat(Str &&line)
{
    //TODO needs to be more strict
    static const boost::regex pattern {
        R"([a-zA-Z.,~]*#@#.*)"
    };

    return boost::regex_match(line, pattern);
}

#endif // EXCEPTION_ELEMENT_HIDE_RULE_HPP
