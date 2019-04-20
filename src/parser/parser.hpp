#include "rule/rule.hpp"

#include <memory>
#include <vector>

namespace adblock {

class FilterSet;
class StringRange;

} // namespace adblock

namespace adblock::parser {

// parse filter list
std::vector<std::unique_ptr<Rule>>
    parse(FilterSet const&, StringRange list);

// parse single line
std::unique_ptr<Rule>
    parse(StringRange line);

} // namespace adblock::parser
