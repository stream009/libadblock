#include "adblock.hpp"

#include "context.hpp"
#include "filter_set.hpp"
#include "uri.hpp"

#include "parser/parser.hpp"
#include "rule/basic_filter_rule.hpp"
#include "rule/comment_rule.hpp"
#include "rule/element_hide_rule.hpp"
#include "rule/exception_filter_rule.hpp"
#include "rule/filter_rule.hpp"
#include "rule/rule.hpp"

#include <memory>

#include <boost/property_tree/ptree.hpp>
#include <boost/range/algorithm.hpp>

namespace adblock {

using FilterSetPtr = AdBlock::FilterSetPtr;
using FilterSetRng = AdBlock::FilterSetRng;

AdBlock::
AdBlock()
    : m_filterRuleBase {}
    , m_elementHideRuleBase { m_filterRuleBase }
{}

AdBlock::~AdBlock() = default;

//TODO: make origin uri into array of uris.
std::pair<bool, FilterRule const*> AdBlock::
shouldBlock(Uri const& uri, Context const& context) const
{
    return m_filterRuleBase.query(uri, context);
}

std::string AdBlock::
elementHideCss(Uri const& uri) const
{
    return m_elementHideRuleBase.query(uri); //TODO site key
}

std::vector<StringRange> AdBlock::
extendedElementHideSelector(Uri const& uri) const
{
    std::vector<StringRange> results;

    auto const& rules = m_elementHideRuleBase.lookupExtendedRule(uri);

    for (auto const& rule: rules) {
        results.push_back(rule->cssSelector());
    }

    return results;
}

StringRange AdBlock::
contentSecurityPolicy(Uri const& uri) const
{
    struct CspContext : Context {
        CspContext(Uri const& uri) : m_uri { uri } {}

        Uri const& origin() const { return m_uri; }
        bool isCsp() const override { return true; }

        Uri const& m_uri;
    } cxt { uri };

    auto const& [found, rule] = m_filterRuleBase.query(uri, cxt);
    if (!found) return {};

    auto* const policy = rule->cspValue();
    assert(policy);

    return *policy;
}

FilterSetRng AdBlock::
filterSets() const
{
    return boost::adaptors::indirect(m_filterSets);
}

FilterSet* AdBlock::
filterSet(Path const& filePath) const
{
    for (auto const& filterSet: m_filterSets) {
        if (filterSet->path() == filePath) {
            return filterSet.get();
        }
    }

    return nullptr;
}

boost::property_tree::ptree AdBlock::
statistics() const
{
    boost::property_tree::ptree result, detail;

    size_t total = 0u;

    auto stats = m_filterRuleBase.statistics();
    auto num = stats.get<size_t>("Total");
    total += num;
    result.put("Filter rule", num);
    detail.put_child("Filter rule", stats);

    stats = m_elementHideRuleBase.statistics();
    num = stats.get<size_t>("Total");
    total += num;
    result.put("Element hide rule", num);
    detail.put_child("Element hide rule", stats);

    result.put("Total", total);

    result.put_child("detail", detail);

    return result;
}

void AdBlock::
addFilterSet(Path const& filePath)
{
    auto &&filterSet = std::make_unique<FilterSet>(filePath);

    registerFilterSetToRuleBases(filterSet);

    m_filterSets.push_back(std::move(filterSet));
}

void AdBlock::
removeFilterSet(Path const& filePath)
{
    auto const it = boost::find_if(m_filterSets,
        [&](auto& ptr) {
            return ptr->path() == filePath;
        });

    if (it == m_filterSets.end()) return;

    m_filterSets.erase(it);

    reload();
}

void AdBlock::
removeFilterSet(FilterSet const& filterSet)
{
    auto const it = boost::find_if(m_filterSets,
        [&](auto& ptr) {
            return ptr.get() == &filterSet;
        });

    if (it == m_filterSets.end()) return;

    m_filterSets.erase(it);

    reload();
}

void AdBlock::
reload()
{
    m_filterRuleBase.clear();
    m_elementHideRuleBase.clear();

    for (auto const& filterSet: m_filterSets) {
        filterSet->reload();

        registerFilterSetToRuleBases(filterSet);
    }
}

void AdBlock::
registerFilterSetToRuleBases(FilterSetPtr const& filterSet)
{
    for (auto const& rule: filterSet->rules()) {
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
        else if (dynamic_cast<CommentRule const*>(&rule)) {
            // comment rule will be skipped
        }
        else {
            assert(false && "unknown type of rule");
        }
    }
}

} // namespace adblock
