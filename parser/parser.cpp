#include "parser.hpp"

#include "grammar.hpp"

namespace adblock { namespace parser {

std::shared_ptr<Rule>
parse(const StringRange &line)
{
    static const Grammar grammar;
    std::shared_ptr<Rule> rule;

    const auto rv = parse(line.begin(), line.end(), grammar, rule);
    if (!rv) {
        std::cout << "Couldn't parse: " << line << "\n";
    }

    return rule;
}

}} // namespace adblock::parser
