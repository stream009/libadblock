#ifndef ADBLOCK_CORE_SNIPPET_RULE_BASE_HPP
#define ADBLOCK_CORE_SNIPPET_RULE_BASE_HPP

#include "core/string_range.hpp"

#include <vector>

#include <boost/property_tree/ptree_fwd.hpp>

namespace adblock {

class FilterRuleBase;
class SnippetRule;
class Uri;

class SnippetRuleBase
{
public:
    using Rules = std::vector<SnippetRule const*>;

public:
    SnippetRuleBase(FilterRuleBase const&);
    ~SnippetRuleBase();

    // query
    Rules lookup(Uri const&, StringRange siteKey = {}) const;

    boost::property_tree::ptree statistics() const;

    // modifier
    void put(SnippetRule const&);
    void clear();

private:
    bool isWhiteListed(Uri const&, StringRange siteKey) const;
    bool isDomainSpecificOnly(Uri const&, StringRange siteKey) const;

private:
    Rules m_genericRules;
    Rules m_specificRules;
    FilterRuleBase const& m_filterRuleBase;
};

} // namespace adblock

#endif // ADBLOCK_CORE_SNIPPET_RULE_BASE_HPP
