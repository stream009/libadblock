#ifndef ADBLOCK_HPP
#define ADBLOCK_HPP

#include "element_hide_rule_base.hpp"
#include "filter_rule_base.hpp"
#include "snippet_rule_base.hpp"
#include "string_range.hpp"
#include "json_fwd.hpp"

#include <filesystem>
#include <memory>
#include <vector>

namespace adblock {

class Context;
class FilterList;
class Uri;

class AdBlock
{
public:
    using FilterListPtr = std::unique_ptr<FilterList>;
    using FilterListPtrs = std::vector<FilterListPtr>;
    using Path = std::filesystem::path;

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

    std::vector<SnippetRule const*>
        snippets(Uri const&, StringRange siteKey = {}) const;

    std::vector<FilterList const*> filterLists() const;
    FilterList const* filterList(Path const& filePath) const;

    json::object statistics() const;

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
    SnippetRuleBase m_snippetRuleBase;
};

} // namespace adblock

#endif // ADBLOCK_HPP
