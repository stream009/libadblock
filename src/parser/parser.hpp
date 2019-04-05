#include "filter_set.hpp"
#include "rule/rule.hpp"
#include "type.hpp"

#include <memory>
#include <vector>

namespace adblock::parser {

// parse filter list
std::vector<std::shared_ptr<Rule>>
    parse(FilterSet const&, StringRange list);

// parse single line
std::shared_ptr<Rule>
    parse(StringRange line);

} // namespace adblock::parser
