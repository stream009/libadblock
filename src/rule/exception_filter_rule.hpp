#ifndef EXCEPTION_FILTER_RULE_HPP
#define EXCEPTION_FILTER_RULE_HPP

#include "filter_rule.hpp"

#include <memory>
#include <vector>

namespace adblock {

class ExceptionFilterRule : public FilterRule
{
    using Base = FilterRule;
public:
    ExceptionFilterRule(std::shared_ptr<Pattern> const& pattern,
                        std::vector<std::shared_ptr<Option>> const& options)
        : Base { pattern, options }
    {}
};

} // namespace adblock

#endif // EXCEPTION_FILTER_RULE_HPP
