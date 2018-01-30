#include "adblock.hpp"

#include "parser/parser.hpp"
#include "rule/basic_filter_rule.hpp"
#include "rule/comment_rule.hpp"
#include "rule/element_hide_rule.hpp"
#include "rule/exception_filter_rule.hpp"
#include "rule/filter_rule.hpp"
#include "rule/rule.hpp"
#include "option/generic_hide_option.hpp"
#include "option/generic_block_option.hpp"

#include <memory>

#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/range/adaptor/indirected.hpp>
#include <boost/range/algorithm.hpp>

namespace adblock {

using FilterSetPtr = AdBlock::FilterSetPtr;
using FilterSetRng = AdBlock::FilterSetRng;

AdBlock::
AdBlock()
    : m_filterRuleBase {}
    , m_elementHideRuleBase { m_filterRuleBase }
{}

//TODO: make origin uri into array of uris.
std::pair<bool, const FilterRule*> AdBlock::
shouldBlock(const Uri &uri, const Context &context) const
{
    return m_filterRuleBase.query(uri, context);
}

std::string AdBlock::
elementHideCss(const Uri &uri) const
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

const FilterSetRng AdBlock::
filterSets() const
{
    return boost::adaptors::indirect(m_filterSets);
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
addFilterSet(const Path &filePath)
{
    auto &&filterSet = boost::make_unique<FilterSet>(filePath);

    registerFilterSetToRuleBases(filterSet);

    m_filterSets.push_back(std::move(filterSet));
}

void AdBlock::
removeFilterSet(const FilterSet &filterSet)
{
    const auto &it = boost::find_if(m_filterSets,
        [&filterSet](const FilterSetPtr &ptr) {
            return ptr.get() == &filterSet;
        });
    assert(it != m_filterSets.end());

    m_filterSets.erase(it);

    reload();
}

void AdBlock::
reload()
{
    m_filterRuleBase.clear();
    m_elementHideRuleBase.clear();

    for (const auto &filterSet: m_filterSets) {
        filterSet->reload();

        registerFilterSetToRuleBases(filterSet);
    }
}

void AdBlock::
registerFilterSetToRuleBases(const FilterSetPtr &filterSet)
{
    for (const auto &rule: filterSet->rules()) {
        if (const auto *ptr = dynamic_cast<const BasicFilterRule*>(&rule)) {
            assert(ptr);
            m_filterRuleBase.put(*ptr);
        }
        else if (const auto *ptr =
                          dynamic_cast<const ExceptionFilterRule*>(&rule))
        {
            assert(ptr);
            m_filterRuleBase.put(*ptr);
        }
        else if (const auto *ptr =
                            dynamic_cast<const ElementHideRule*>(&rule))
        {
            assert(ptr);
            m_elementHideRuleBase.put(*ptr);
        }
        else if (dynamic_cast<const CommentRule*>(&rule)) {
            // comment rule will be skipped
        }
        else {
            assert(false && "unknown type of rule");
        }
    }
}

} // namespace adblock
