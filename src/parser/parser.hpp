#include "rule/rule.hpp"

#include <memory>
#include <vector>

namespace adblock {

class FilterList;
class StringRange;
class ParseError;

} // namespace adblock

namespace adblock::parser {

// parse filter list
std::vector<std::unique_ptr<Rule>>
    parse(FilterList const&, std::vector<ParseError>&);

// parse single line
std::unique_ptr<Rule>
    parse(StringRange line, std::vector<ParseError>&);

} // namespace adblock::parser
