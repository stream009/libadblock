#ifndef BASIC_FILTER_RULE_HPP
#define BASIC_FILTER_RULE_HPP

#include "filter_rule.hpp"
#include "pattern/pattern.hpp"

#include <memory>
#include <vector>

namespace adblock {

class Option;

class BasicFilterRule : public FilterRule
{
    using Base = FilterRule;
public:
    BasicFilterRule(std::unique_ptr<Pattern> pattern,
                    std::vector<std::shared_ptr<Option>> const& options)
        : Base { std::move(pattern), options }
    {}
};

} // namespace adblock

#endif // BASIC_FILTER_RULE_HPP
