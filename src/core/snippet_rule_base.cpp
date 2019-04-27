#include "snippet_rule_base.hpp"

#include "core/filter_rule_base.hpp"
#include "core/json.hpp"
#include "core/string_range.hpp"
#include "core/uri.hpp"
#include "rule/snippet_rule.hpp"

#include <algorithm>
#include <iterator>
#include <memory>

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

json::object SnippetRuleBase::
statistics() const
{
    json::object result;

    double total = 0;

    auto num = static_cast<double>(m_genericRules.size());
    total += num;
    result["Generic snippet rule"] = num;

    num = static_cast<double>(m_specificRules.size());
    total += num;
    result["Specific snippet rule"] = num;
    result["Total"] = total;

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
