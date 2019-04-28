#include "parser.hpp"

#include "parse_error.hpp"
#include "rule_builder.hpp"

#include "core/filter_list.hpp"
#include "core/string_range.hpp"

#include <string_view>
#include <memory>
#include <vector>

namespace adblock::parser {

std::vector<std::unique_ptr<Rule>>
parse(FilterList const& list, std::vector<ParseError>& errors)
{
    std::vector<std::unique_ptr<Rule>> rules;

    RuleBuilder builder { list, rules, errors };

    StringRange const text { list.begin(), list.end() };

    adblock_parser::parse_filter_list(text, builder);

    return rules;
}

std::unique_ptr<Rule>
parse(StringRange const line, std::vector<ParseError>& errors)
{
    std::vector<std::unique_ptr<Rule>> rules;

    RuleBuilder builder { rules, errors };

    adblock_parser::parse_filter(line, builder);

    if (rules.empty()) {
        return nullptr;
    }
    else {
        return std::move(rules.front());
    }
}

} // namespace adblock::parser
