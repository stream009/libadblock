#include "filter_set.hpp"
#include "rule/rule.hpp"
#include "type.hpp"

#include <memory>
#include <vector>

namespace adblock::parser {

// parse filter list
std::vector<std::unique_ptr<Rule>>
    parse(FilterSet const&, StringRange list);

// parse single line
std::unique_ptr<Rule>
    parse(StringRange line);

} // namespace adblock::parser
