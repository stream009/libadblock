#include "rule.hpp"

namespace adblock {

void Rule::
setFilterSet(FilterSet const& filterSet)
{
    m_filterSet = &filterSet;
}

void Rule::
setLine(StringRange const& line)
{
    m_line = line;
}

} // namespace adblock
