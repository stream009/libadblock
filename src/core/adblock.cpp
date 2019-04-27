#include "adblock.hpp"

#include "context.hpp"
#include "filter_list.hpp"
#include "json.hpp"
#include "uri.hpp"

#include "parser/parser.hpp"
#include "rule/basic_filter_rule.hpp"
#include "rule/comment_rule.hpp"
#include "rule/element_hide_rule.hpp"
#include "rule/exception_filter_rule.hpp"
#include "rule/filter_rule.hpp"
#include "rule/rule.hpp"
#include "rule/snippet_rule.hpp"

#include <memory>

namespace adblock {

using FilterListPtr = AdBlock::FilterListPtr;
using FilterListRng = AdBlock::FilterListRng;

AdBlock::
AdBlock()
    : m_filterRuleBase {}
    , m_elementHideRuleBase { m_filterRuleBase }
    , m_snippetRuleBase { m_filterRuleBase }
{}

AdBlock::~AdBlock() = default;

std::pair<bool, FilterRule const*> AdBlock::
shouldBlock(Uri const& uri, Context const& context) const
{
    return m_filterRuleBase.query(uri, context);
}

std::vector<StringRange> AdBlock::
elementHideSelectors(Uri const& uri, StringRange const siteKey/* = {}*/) const
{
    std::vector<StringRange> results;

    auto const& rules =
        m_elementHideRuleBase.lookupRules(uri, siteKey);

    for (auto const& rule: rules) {
        results.push_back(rule->cssSelector());
    }

    return results;
}

std::vector<StringRange> AdBlock::
extendedElementHideSelectors(Uri const& uri,
                             StringRange const siteKey/*= {}*/) const
{
    std::vector<StringRange> results;

    auto const& rules =
        m_elementHideRuleBase.lookupExtendedRules(uri, siteKey);

    for (auto const& rule: rules) {
        results.push_back(rule->cssSelector());
    }

    return results;
}

StringRange AdBlock::
contentSecurityPolicy(Uri const& uri,
                      StringRange const siteKey/*= {}*/) const
{
    struct CspContext : Context {
        CspContext(Uri const& uri, StringRange const siteKey)
            : m_uri { uri }
            , m_siteKey { siteKey }
        {}

        Uri const& origin() const override { return m_uri; }
        bool isCsp() const override { return true; }
        StringRange siteKey() const override { return m_siteKey; }

        Uri const& m_uri;
        StringRange m_siteKey;
    } cxt { uri, siteKey };

    //TODO what happen if multiple rules hit?
    auto const& [found, rule] = m_filterRuleBase.query(uri, cxt);
    if (!found) return {};

    auto* const policy = rule->cspValue();
    assert(policy);

    return *policy;
}

std::vector<SnippetRule const*> AdBlock::
snippets(Uri const& uri, StringRange const siteKey/*= {}*/) const
{
    return m_snippetRuleBase.lookup(uri, siteKey);
}

FilterListRng AdBlock::
filterLists() const
{
    return boost::adaptors::indirect(m_filterLists);
}

FilterList const* AdBlock::
filterList(Path const& filePath) const
{
    for (auto const& filterList: m_filterLists) {
        if (filterList->path() == filePath) {
            return filterList.get();
        }
    }

    return nullptr;
}

json::object AdBlock::
statistics() const
{
    json::object result, detail;

    double total = 0;

    auto stats = m_filterRuleBase.statistics();
    auto num = to_number(stats["Total"]);
    total += num;
    result["Filter rule"] = num;
    detail["Filter rule"] = stats;

    stats = m_elementHideRuleBase.statistics();
    num = to_number(stats["Total"]);
    total += num;
    result["Element hide rule"] = num;
    detail["Element hide rule"] = stats;

    stats = m_snippetRuleBase.statistics();
    num = to_number(stats["Total"]);
    total += num;
    result["Snippet rule"] = num;
    detail["Snippet rule"] = stats;

    result["Total"] = total;
    result["detail"] = detail;

    return result;
}

void AdBlock::
addFilterList(Path const& filePath)
{
    auto filterList = std::make_unique<FilterList>(filePath);

    registerFilterListToRuleBases(filterList);

    m_filterLists.push_back(std::move(filterList));
}

void AdBlock::
removeFilterList(Path const& filePath)
{
    auto const it = std::find_if(
        m_filterLists.begin(), m_filterLists.end(),
        [&](auto& ptr) {
            return ptr->path() == filePath;
        });

    if (it == m_filterLists.end()) return;

    m_filterLists.erase(it);

    reload();
}

void AdBlock::
removeFilterList(FilterList const& filterList)
{
    auto const it = std::find_if(
        m_filterLists.begin(), m_filterLists.end(),
        [&](auto& ptr) {
            return ptr.get() == &filterList;
        });

    if (it == m_filterLists.end()) return;

    m_filterLists.erase(it);

    reload();
}

void AdBlock::
reload()
{
    m_filterRuleBase.clear();
    m_elementHideRuleBase.clear();
    m_snippetRuleBase.clear();

    for (auto const& filterList: m_filterLists) {
        filterList->reload();

        registerFilterListToRuleBases(filterList);
    }
}

void AdBlock::
registerFilterListToRuleBases(FilterListPtr const& filterList)
{
    for (auto const& rule: filterList->rules()) {
        if (auto* const ptr = dynamic_cast<BasicFilterRule const*>(&rule)) {
            assert(ptr);
            m_filterRuleBase.put(*ptr);
        }
        else if (auto* const ptr =
                          dynamic_cast<ExceptionFilterRule const*>(&rule))
        {
            assert(ptr);
            m_filterRuleBase.put(*ptr);
        }
        else if (auto* const ptr =
                            dynamic_cast<ElementHideRule const*>(&rule))
        {
            assert(ptr);
            m_elementHideRuleBase.put(*ptr);
        }
        else if (auto* const ptr = dynamic_cast<SnippetRule const*>(&rule)) {
            assert(ptr);
            m_snippetRuleBase.put(*ptr);
        }
        else if (dynamic_cast<CommentRule const*>(&rule)) {
            // comment rule will be skipped
        }
        else {
            assert(false && "unknown type of rule");
        }
    }
}

} // namespace adblock
