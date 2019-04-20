#ifndef BASIC_FILTER_RULE_HPP
#define BASIC_FILTER_RULE_HPP

#include "filter_rule.hpp"
#include "filter_option.hpp"

#include "core/string_range.hpp"
#include "pattern/pattern.hpp"

#include <memory>

namespace adblock {

class Option;

class BasicFilterRule : public FilterRule
{
    using Base = FilterRule;
public:
    BasicFilterRule(std::unique_ptr<Pattern> pattern,
                    FilterOptionSet options,
                    std::unique_ptr<Domains> domains,
                    std::unique_ptr<SiteKeys> siteKeys,
                    std::unique_ptr<StringRange> cspValue)
        : Base {
            std::move(pattern),
            std::move(options),
            std::move(domains),
            std::move(siteKeys),
            std::move(cspValue)
        }
    {}
};

} // namespace adblock

#endif // BASIC_FILTER_RULE_HPP
