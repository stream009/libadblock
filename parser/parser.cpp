#include "grammar.hpp"

#include "rule/element_hide_rule.hpp"

#include <memory>

std::shared_ptr<Rule>
parse(const std::string &line)
{
    adblock::parser::Grammar grammar;
    std::shared_ptr<Rule> rule;

    const auto rv = parse(line.begin(), line.end(), grammar, rule);
    if (rv) {
        //const auto &aRule = *rule;
        //std::cout << aRule << "\n";
    }
    else {
        std::cout << "Couldn't parse: " << line << "\n";
    }

    return rule;
}
