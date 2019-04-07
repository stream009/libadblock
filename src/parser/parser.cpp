#include "parser.hpp"

#include "filter_set.hpp"
#include "rule_builder.hpp"

#include <string_view>
#include <memory>
#include <vector>

namespace adblock::parser {

std::vector<std::shared_ptr<Rule>>
parse(FilterSet const& set, StringRange const text)
{
    std::vector<std::shared_ptr<Rule>> rules;
    std::string_view sv { text.begin(), text.size() };

    RuleBuilder builder { set, rules };

    adblock_parser::parse_filter_list(sv, builder);

    return rules;
}

std::shared_ptr<Rule>
parse(StringRange const line)
{
    std::vector<std::shared_ptr<Rule>> rules;
    RuleBuilder builder { rules };

    std::string_view const line_ { line.begin(), line.size() };

    adblock_parser::parse_filter(line_, builder);

    if (rules.empty()) {
        return nullptr;
    }
    else {
        return rules.front();
    }
}

} // namespace adblock::parser
