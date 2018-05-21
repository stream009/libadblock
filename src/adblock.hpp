#ifndef ADBLOCK_HPP
#define ADBLOCK_HPP

#include "context.hpp"
#include "element_hide_rule_base.hpp"
#include "filter_rule_base.hpp"
#include "filter_set.hpp"
#include "type.hpp"

#include <memory>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <boost/range/iterator_range.hpp>

namespace adblock {

class AdBlock
{
public:
    using FilterSetPtr = std::unique_ptr<FilterSet>;
    using FilterSetPtrs = std::vector<FilterSetPtr>;
    using FilterSetRng = boost::indirected_range<const FilterSetPtrs>;
    using Path = boost::filesystem::path;

public:
    AdBlock();

    // query
    std::pair<bool, const FilterRule*>
        shouldBlock(const Uri&, const Context&) const;

    std::string elementHideCss(const Uri&) const;

    std::vector<StringRange> extendedElementHideSelector(Uri const&) const;

    StringRange contentSecurityPolicy(Uri const&) const;

    FilterSetRng filterSets() const;
    FilterSet* filterSet(Path const& filePath) const;

    boost::property_tree::ptree statistics() const;

    // modifier
    void addFilterSet(Path const& filePath);
    void removeFilterSet(Path const& filePath);
    void removeFilterSet(FilterSet const&);

    void reload();

private:
    void registerFilterSetToRuleBases(const FilterSetPtr&);

private:
    FilterSetPtrs m_filterSets;
    FilterRuleBase m_filterRuleBase;
    ElementHideRuleBase m_elementHideRuleBase;
};

} // namespace adblock

#endif // ADBLOCK_HPP
