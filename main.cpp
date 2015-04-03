#include "rule/rule.hpp"
#include "rule/rule_factory.hpp"
#include "parser/parser.hpp"

#include <iomanip>
#include <iostream>
#include <memory>

int main()
{
    std::string line;
    std::unique_ptr<Rule> rule;
    RuleFactory factory;
    do {
        std::cout << "rule: ";
        std::getline(std::cin, line);
        if (line.empty()) continue;

        rule = factory.create(line);

        parse(line);
    } while (!rule);

    do {
        std::cout << "URL: " << std::flush;
        std::getline(std::cin, line);
        if (line.empty()) break;

        const auto * const filterRule = dynamic_cast<FilterRule*>(rule.get());
        assert(filterRule);
        const auto rv = filterRule->match(line, "");
        std::cout << "matched: " << std::boolalpha << rv << "\n";
    } while (true);
}
