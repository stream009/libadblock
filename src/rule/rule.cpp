#include "rule.hpp"

namespace adblock {

void Rule::
setFilterList(FilterList const& filterList)
{
    m_filterList = &filterList;
}

void Rule::
setLine(StringRange const line)
{
    m_line = line;
}

} // namespace adblock
