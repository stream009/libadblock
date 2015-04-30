#ifndef EXCEPTION_FILTER_RULE_HPP
#define EXCEPTION_FILTER_RULE_HPP

#include "filter_rule.hpp"

#include <memory>
#include <vector>

#include <boost/optional.hpp>

namespace adblock {

class ExceptionFilterRule : public FilterRule
{
    using Base = FilterRule;
public:
    ExceptionFilterRule(const std::shared_ptr<Pattern> &pattern,
                        const boost::optional<
                              std::vector<std::shared_ptr<Option>>> &options)
        : Base { pattern, options }
    {}
};

} // namespace adblock

#endif // EXCEPTION_FILTER_RULE_HPP
