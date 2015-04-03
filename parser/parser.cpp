#include "grammar.hpp"

#include <boost/variant/polymorphic_get.hpp>

void
parse(const std::string &line)
{
    adblock::parser::Grammar<std::string::const_iterator> grammar;
    adblock::parser::VariantRule rule;

    const auto rv = parse(line.begin(), line.end(), grammar, rule);
    if (rv) {
        std::cout << rule.type().name() << "\n";
        std::cout << boost::polymorphic_get<Rule>(rule).line() << "\n";
    }
    else {
        std::cout << "Couldn't parse.";
    }
}
