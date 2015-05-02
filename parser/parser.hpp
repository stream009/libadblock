#ifndef PARSER_PARSER_HPP
#define PARSER_PARSER_HPP

#include "type.hpp"
#include "rule/rule.hpp"

#include <memory>

namespace adblock { namespace parser {

StringRange
parseHeader(const StringRange&);

std::shared_ptr<Rule>
parse(const StringRange &header);

}} // namespace adblock::parser

#endif // PARSER_PARSER_HPP
