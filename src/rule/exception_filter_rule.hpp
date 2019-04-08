#ifndef EXCEPTION_FILTER_RULE_HPP
#define EXCEPTION_FILTER_RULE_HPP

#include "filter_rule.hpp"
#include "pattern/pattern.hpp"

#include <memory>
#include <vector>

namespace adblock {

class Option;

class ExceptionFilterRule : public FilterRule
{
    using Base = FilterRule;
public:
    ExceptionFilterRule(std::unique_ptr<Pattern> pattern,
                        std::vector<std::unique_ptr<Option>> options)
        : Base { std::move(pattern), std::move(options) }
    {}
};

} // namespace adblock

#endif // EXCEPTION_FILTER_RULE_HPP
