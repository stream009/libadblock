#ifndef EXCEPTION_FILTER_RULE_HPP
#define EXCEPTION_FILTER_RULE_HPP

#include "filter_rule.hpp"

#include <memory>
#include <vector>

#include <boost/optional.hpp>

class ExceptionFilterRule : public FilterRule
{
    using Base = FilterRule;
public:
    ExceptionFilterRule(const std::shared_ptr<Pattern> &pattern,
                        const boost::optional<
                              std::vector<std::shared_ptr<Option>>> &options)
        : Base { pattern, options }
    {}

    bool match(const Uri&, const Context&) const override;
};

inline bool ExceptionFilterRule::
match(const Uri &url, const Context &context) const
{
    return !Base::match(url, context);
}

#endif // EXCEPTION_FILTER_RULE_HPP
