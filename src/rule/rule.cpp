#include "rule.hpp"
#include "filter_set.hpp"

namespace adblock {

void Rule::
setFilterSet(const FilterSet &filterSet)
{
    m_filterSet = &filterSet;
}

void Rule::
setLine(const StringRange &line)
{
    m_line = line;
}

} // namespace adblock
