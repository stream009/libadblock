#ifndef BASIC_FILTER_RULE_HPP
#define BASIC_FILTER_RULE_HPP

#include "filter_rule.hpp"

#include <memory>
#include <vector>

namespace adblock {

class BasicFilterRule : public FilterRule
{
    using Base = FilterRule;
public:
    BasicFilterRule(std::shared_ptr<Pattern> const& pattern,
                    std::vector<std::shared_ptr<Option>> const& options)
        : Base { pattern, options }
    {}
};

} // namespace adblock

#endif // BASIC_FILTER_RULE_HPP
