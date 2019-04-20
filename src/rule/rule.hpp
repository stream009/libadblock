#ifndef RULE_HPP
#define RULE_HPP

#include "core/string_range.hpp"

#include <ostream>

#include <boost/noncopyable.hpp>

namespace adblock {

class FilterSet;

class Rule : boost::noncopyable
{
public:
    virtual ~Rule() = default;

    // accessor
    FilterSet const* filterSet() const { return m_filterSet; }
    StringRange const line() const { return m_line; }

    // modifier
    void setFilterSet(FilterSet const&);
    void setLine(StringRange const);

private:
    virtual void print(std::ostream&) const = 0;

    friend std::ostream &operator<<(std::ostream &os, Rule const& rule)
    {
        rule.print(os);
        return os;
    }

private:
    FilterSet const* m_filterSet = nullptr;
    StringRange m_line;
};

} // namespace adblock

#endif // RULE_HPP
