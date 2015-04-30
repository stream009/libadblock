#ifndef PARSER_PARSER_HPP
#define PARSER_PARSER_HPP

#include "type.hpp"
#include "rule/rule.hpp"

#include <memory>

namespace adblock { namespace parser {

std::shared_ptr<Rule>
parse(const StringRange &line);

}} // namespace adblock::parser

#endif // PARSER_PARSER_HPP
