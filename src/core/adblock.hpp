#ifndef ADBLOCK_HPP
#define ADBLOCK_HPP

#include "element_hide_rule_base.hpp"
#include "filter_rule_base.hpp"
#include "string_range.hpp"

#include <memory>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <boost/range/adaptor/indirected.hpp>

namespace adblock {

class Context;
class FilterSet;
class Uri;

class AdBlock
{
public:
    using FilterSetPtr = std::unique_ptr<FilterSet>;
    using FilterSetPtrs = std::vector<FilterSetPtr>;
    using FilterSetRng = boost::indirected_range<FilterSetPtrs const>;
    using Path = boost::filesystem::path;

public:
    AdBlock();
    ~AdBlock();

    // query
    std::pair<bool, FilterRule const*>
        shouldBlock(Uri const&, Context const&) const;

    std::string elementHideCss(Uri const&) const;

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
    void registerFilterSetToRuleBases(FilterSetPtr const&);

private:
    FilterSetPtrs m_filterSets;
    FilterRuleBase m_filterRuleBase;
    ElementHideRuleBase m_elementHideRuleBase;
};

} // namespace adblock

#endif // ADBLOCK_HPP
