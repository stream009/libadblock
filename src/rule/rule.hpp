#ifndef RULE_HPP
#define RULE_HPP

#include "namespace.hpp"

#include "core/string_range.hpp"

#include <ostream>

#include <stream9/nonmovable.hpp>

namespace adblock {

class FilterList;

class Rule : st9::nonmovable
{
public:
    virtual ~Rule() = default;

    // accessor
    FilterList const* filterList() const { return m_filterList; }
    StringRange const line() const { return m_line; }

    // modifier
    void setFilterList(FilterList const&);
    void setLine(StringRange const);

private:
    virtual void print(std::ostream&) const = 0;

    friend std::ostream &operator<<(std::ostream &os, Rule const& rule)
    {
        rule.print(os);
        return os;
    }

private:
    FilterList const* m_filterList = nullptr;
    StringRange m_line;
};

} // namespace adblock

#endif // RULE_HPP
