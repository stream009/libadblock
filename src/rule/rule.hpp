#ifndef RULE_HPP
#define RULE_HPP

#include "type.hpp"

#include <ostream>

#include <boost/noncopyable.hpp>

namespace adblock {

class FilterSet;

class Rule : boost::noncopyable
{
public:
    virtual ~Rule() = default;

    // accessor
    const FilterSet *filterSet() const { return m_filterSet; }
    const StringRange &line() const { return m_line; }

    // modifier
    void setFilterSet(const FilterSet&);
    void setLine(const StringRange&);

private:
    virtual void print(std::ostream&) const = 0;

    friend std::ostream &operator<<(std::ostream &os, const Rule &rule)
    {
        rule.print(os);
        return os;
    }

private:
    const FilterSet *m_filterSet = nullptr;
    StringRange m_line;
};

} // namespace adblock

#endif // RULE_HPP
