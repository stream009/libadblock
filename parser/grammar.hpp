#ifndef PARSER_GRAMMER_HPP
#define PARSER_GRAMMER_HPP

#include "element_hide_rule.hpp"
#include "filter_rule.hpp"
#include "type.hpp"

#include <memory>

namespace adblock {

class Rule;

} // namespace adblock

namespace adblock { namespace parser {

struct Grammar : grammar<std::shared_ptr<Rule>()>
{
public:
    Grammar();

private:
    rule<std::shared_ptr<Rule>()> start;

    FilterRule filter_rule;
    ElementHideRule element_hide_rule;
    rule<std::shared_ptr<Rule>()> comment_rule;
};

}} // namespace adblock::parser

#endif // PARSER_GRAMMER_HPP
