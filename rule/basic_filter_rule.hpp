#ifndef BASIC_FILTER_RULE_HPP
#define BASIC_FILTER_RULE_HPP

#include "filter_rule.hpp"

class BasicFilterRule : public FilterRule
{
    using Base = FilterRule;
public:
    BasicFilterRule() = default;

    template<typename Str>
    BasicFilterRule(Str&&);

    template<typename Str>
    static bool matchFormat(Str&&);
};

#include <iterator>
#include <utility>

#include <boost/algorithm/string/predicate.hpp>

template<typename Str>
inline BasicFilterRule::
BasicFilterRule(Str &&line)
    : Base { std::forward<Str>(line) }
{
    const auto &l = this->line();
    assert(matchFormat(l));

    Base::parse(Base::StringRange { std::begin(l), std::end(l) });
}

template<typename Str>
inline bool BasicFilterRule::
matchFormat(Str &&line)
{
    return !boost::algorithm::starts_with(line, "@@");
}

#endif // BASIC_FILTER_RULE_HPP
