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
class FilterList;
class Uri;

class AdBlock
{
public:
    using FilterListPtr = std::unique_ptr<FilterList>;
    using FilterListPtrs = std::vector<FilterListPtr>;
    using FilterListRng = boost::indirected_range<FilterListPtrs const>;
    using Path = boost::filesystem::path;

public:
    AdBlock();
    ~AdBlock();

    // query
    std::pair<bool, FilterRule const*>
        shouldBlock(Uri const&, Context const&) const;

    std::vector<StringRange>
        elementHideSelectors(Uri const&, StringRange siteKey = {}) const;

    std::vector<StringRange>
        extendedElementHideSelectors(Uri const&, StringRange siteKey = {}) const;

    StringRange contentSecurityPolicy(Uri const&, StringRange siteKey = {}) const;

    FilterListRng filterLists() const;
    FilterList* filterList(Path const& filePath) const;

    boost::property_tree::ptree statistics() const;

    // modifier
    void addFilterList(Path const& filePath);
    void removeFilterList(Path const& filePath);
    void removeFilterList(FilterList const&);

    void reload();

private:
    void registerFilterListToRuleBases(FilterListPtr const&);

private:
    FilterListPtrs m_filterLists;
    FilterRuleBase m_filterRuleBase;
    ElementHideRuleBase m_elementHideRuleBase;
};

} // namespace adblock

#endif // ADBLOCK_HPP
