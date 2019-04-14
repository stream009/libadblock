#ifndef FILTER_RULE_HPP
#define FILTER_RULE_HPP

#include "filter_option.hpp"
#include "rule.hpp"
#include "type.hpp"

#include <cassert>
#include <iosfwd>
#include <memory>
#include <vector>

namespace adblock {

class Pattern;
class Context;
class WhiteListQueryContext;

class FilterRule : public Rule
{
    using Base = Rule;
public:
    using Domains = std::vector<StringRange>;
    using SiteKeys = std::vector<StringRange>;

public:
    ~FilterRule() override;

    // accessor
    Pattern const& pattern() const;
    Domains* domains() const { return m_domains.get(); }
    SiteKeys* siteKeys() const { return m_siteKeys.get(); }
    StringRange* cspValue() const { return m_cspValue.get(); }

    // query
    bool match(Uri const&, Context const&,
                           bool const specificOnly = false) const;

    bool hasOption(FilterOption mask) const;
    size_t numOptions() const;

protected:
    FilterRule(std::unique_ptr<Pattern>,
               FilterOptionSet,
               std::unique_ptr<Domains>,
               std::unique_ptr<SiteKeys>,
               std::unique_ptr<StringRange> cspValue);
private:
    bool matchTypeOptions(Context const&) const;
    bool matchWhiteListOptions(WhiteListQueryContext const&) const;
    bool matchRestrictionOptions(Uri const&, Context const&) const;
    bool matchDomain(Context const&) const;
    bool matchSiteKey(Context const&) const;
    bool matchOrigin(Uri const&, Context const&) const;

    // @override Rule
    void print(std::ostream&) const override;

    void validate() const
    {
        assert(m_pattern);
    }

private:
    std::unique_ptr<Pattern> m_pattern;
    FilterOptionSet m_options;
    std::unique_ptr<Domains> m_domains;
    std::unique_ptr<SiteKeys> m_siteKeys;
    std::unique_ptr<StringRange> m_cspValue;
};

} // namespace adblock

#endif // FILTER_RULE_HPP
