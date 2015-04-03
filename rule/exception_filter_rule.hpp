#ifndef EXCEPTION_FILTER_RULE_HPP
#define EXCEPTION_FILTER_RULE_HPP

#include "filter_rule.hpp"

class ExceptionFilterRule : public FilterRule
{
    using Base = FilterRule;
public:
    ExceptionFilterRule() = default;

    template<typename Str>
    ExceptionFilterRule(Str&&);

    bool match(const Uri&, const Context&) const override;

    template<typename Str>
    static bool matchFormat(Str&&);
};

#include <iterator>
#include <utility>

#include <boost/algorithm/string/predicate.hpp>

template<typename Str>
inline ExceptionFilterRule::
ExceptionFilterRule(Str &&line)
    : Base { std::forward<Str>(line) }
{
    const auto &l = this->line();
    assert(matchFormat(l));

    const Base::StringRange range {
                std::next(std::begin(l), 2), std::end(l) };
    Base::parse(range);
}

inline bool ExceptionFilterRule::
match(const Uri &url, const Context &context) const
{
    return !Base::match(url, context);
}

template<typename Str>
inline bool ExceptionFilterRule::
matchFormat(Str &&line)
{
    return boost::algorithm::starts_with(line, "@@");
}

#endif // EXCEPTION_FILTER_RULE_HPP
