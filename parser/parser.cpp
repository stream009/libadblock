#include "grammar.hpp"

#include "rule/element_hide_rule.hpp"

#include <memory>

std::shared_ptr<Rule>
parse(const std::string &line)
{
    adblock::parser::Grammar<std::string::const_iterator> grammar;
    std::shared_ptr<Rule> rule;

    const auto rv = parse(line.begin(), line.end(), grammar, rule);
    if (rv) {
        const auto &aRule = *rule;
        std::cout << typeid(aRule).name() << "\n";

        const auto &elementHideRule =
                std::dynamic_pointer_cast<ElementHideRule>(rule);
        if (elementHideRule) {
            std::cout << *elementHideRule;
        }
    }
    else {
        std::cout << "Couldn't parse.";
    }

    return rule;
}
