#ifndef EXCEPTION_FILTER_RULE_HPP
#define EXCEPTION_FILTER_RULE_HPP

#include "filter_rule.hpp"
#include "filter_option.hpp"

#include "core/type.hpp"
#include "pattern/pattern.hpp"

#include <memory>

namespace adblock {

class ExceptionFilterRule : public FilterRule
{
    using Base = FilterRule;
public:
    ExceptionFilterRule(std::unique_ptr<Pattern> pattern,
                        FilterOptionSet&& options,
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

#endif // EXCEPTION_FILTER_RULE_HPP
