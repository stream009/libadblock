#ifndef BASIC_ELEMENT_HIDE_RULE_HPP
#define BASIC_ELEMENT_HIDE_RULE_HPP

#include "element_hide_rule.hpp"

class BasicElementHideRule : public ElementHideRule
{
    using Base = ElementHideRule;
public:
    BasicElementHideRule() = default;

    template<typename Str>
    BasicElementHideRule(Str&&);

    template<typename Str>
    static bool matchFormat(Str&&);
};

#include <cassert>
#include <utility>

#include <boost/regex.hpp>

template<typename Str>
BasicElementHideRule::
BasicElementHideRule(Str &&line)
    : Base { std::forward<Str>(line) }
{
    const auto &l = this->line();
    assert(matchFormat(l));
    //TODO split domain part and selector part
#if 0
    this->parseDomain(domainR);
    this->setCssSelector(selectorR);
#endif
    std::cout << "*** " << l << " ***\n";
}

template<typename Str>
bool BasicElementHideRule::
matchFormat(Str &&line)
{
    //TODO needs to be more strict
    static const boost::regex pattern {
        R"([a-zA-Z.,~]*##.*)"
    };

    return boost::regex_match(line, pattern);
}

#endif // BASIC_ELEMENT_HIDE_RULE_HPP
