#ifndef EXCEPTION_ELEMENT_HIDE_RULE_HPP
#define EXCEPTION_ELEMENT_HIDE_RULE_HPP

#include "element_hide_rule.hpp"

class ExceptionElementHideRule : public ElementHideRule
{
    using Base = ElementHideRule;
public:
    using Base::String;
    using Base::Domain;

public:
    ExceptionElementHideRule(const String &selector,
            const boost::optional<std::vector<Domain>> &domains);

    template<typename Str>
    static bool matchFormat(Str&&);
};

std::ostream &operator<<(std::ostream&, const ExceptionElementHideRule&);

#include <utility>

#include <boost/regex.hpp>

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
