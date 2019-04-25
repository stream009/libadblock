#include "snippet_rule_base.hpp"

#include "core/filter_rule_base.hpp"
#include "core/string_range.hpp"
#include "core/uri.hpp"
#include "rule/snippet_rule.hpp"

#include <algorithm>
#include <iterator>
#include <memory>

#include <boost/property_tree/ptree.hpp>

namespace adblock {

SnippetRuleBase::
SnippetRuleBase(FilterRuleBase const& filterRuleBase)
    : m_filterRuleBase { filterRuleBase }
{}

SnippetRuleBase::~SnippetRuleBase() = default;

SnippetRuleBase::Rules SnippetRuleBase::
lookup(Uri const& uri, StringRange const siteKey/*= {}*/) const
{
    Rules result;

    if (isWhiteListed(uri, siteKey)) return result;

    if (!isDomainSpecificOnly(uri, siteKey)) {
        result = m_genericRules;
    }

    std::copy_if(
        m_specificRules.begin(), m_specificRules.end(),
        std::back_inserter(result),
        [&](auto* rule) { return rule->match(uri); }
    );

    return result;
}

boost::property_tree::ptree SnippetRuleBase::
statistics() const
{
    boost::property_tree::ptree result;

    size_t total = 0u;

    auto num = m_genericRules.size();
    total += num;
    result.put("Generic snippet rule", num);

    num = m_specificRules.size();
    total += num;
    result.put("Specific snippet rule", num);

    result.put("Total", total);

    return result;
}

void SnippetRuleBase::
put(SnippetRule const& rule)
{
    if (rule.domains()) {
        m_specificRules.push_back(&rule);
    }
    else {
        m_genericRules.push_back(&rule);
    }
}

void SnippetRuleBase::
clear()
{
    m_specificRules.clear();
    m_genericRules.clear();
}

bool SnippetRuleBase::
isWhiteListed(Uri const& uri, StringRange const siteKey) const
{
    return m_filterRuleBase.getFrameBlockDisabler(uri, siteKey) != nullptr;
}

bool SnippetRuleBase::
isDomainSpecificOnly(Uri const& uri, StringRange const siteKey) const
{
    return m_filterRuleBase.getGenericBlockDisabler(uri, siteKey) != nullptr;
}

} // namespace adblock
