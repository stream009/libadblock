#ifndef BASIC_FILTER_RULE_HPP
#define BASIC_FILTER_RULE_HPP

#include "filter_rule.hpp"

#include <memory>
#include <vector>

#include <boost/optional.hpp>

class BasicFilterRule : public FilterRule
{
    using Base = FilterRule;
public:
    BasicFilterRule(const std::shared_ptr<Pattern> &pattern,
                    const boost::optional<
                          std::vector<std::shared_ptr<Option>>> &options)
        : Base { pattern, options }
    {}
};

#endif // BASIC_FILTER_RULE_HPP
