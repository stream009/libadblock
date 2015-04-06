#include "rule/rule.hpp"
#include "parser/parser.hpp"

#include "rule/comment_rule.hpp"
#include "rule/basic_filter_rule.hpp"
#include "rule/exception_filter_rule.hpp"
#include "rule/basic_element_hide_rule.hpp"
#include "rule/exception_element_hide_rule.hpp"
#include "pattern/basic_match_pattern.hpp"
#include "pattern/begin_match_pattern.hpp"
#include "pattern/end_match_pattern.hpp"
#include "pattern/domain_match_pattern.hpp"
#include "pattern/regex_pattern.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>

#include <boost/range/algorithm.hpp>

template<typename R>
size_t
countRule(const std::vector<std::shared_ptr<Rule>> rules)
{
    return boost::count_if(rules,
        [] (const std::shared_ptr<Rule> &rule) {
            return std::dynamic_pointer_cast<R>(rule);
        }
    );
}

template<typename R, typename P>
size_t
countPattern(const std::vector<std::shared_ptr<Rule>> rules)
{
    return boost::count_if(rules,
        [] (const std::shared_ptr<Rule> &rule) {
            const auto &filterRule = std::dynamic_pointer_cast<R>(rule);
            return filterRule &&
                   dynamic_cast<const P*>(&filterRule->pattern());
        }
    );
}

int main()
{
    std::string line;
    std::vector<std::shared_ptr<Rule>> rules;

    do {
        std::getline(std::cin, line);
        if (!std::cin) break;

        rules.push_back(parse(line));
    } while (true);

    std::cout << "comment: " << countRule<CommentRule>(rules) << "\n";

    std::cout << "basic filter rule: "
              << countRule<BasicFilterRule>(rules) << "\n";
    std::cout << "\tbasic match pattern: "
              << countPattern<BasicFilterRule, BasicMatchPattern>(rules) << "\n";
    std::cout << "\tbegin match pattern: "
              << countPattern<BasicFilterRule, BeginMatchPattern>(rules) << "\n";
    std::cout << "\tend match pattern: "
              << countPattern<BasicFilterRule, EndMatchPattern>(rules) << "\n";
    std::cout << "\tdomain match pattern: "
              << countPattern<BasicFilterRule, DomainMatchPattern>(rules) << "\n";
    std::cout << "\tregex pattern: "
              << countPattern<BasicFilterRule, RegexPattern>(rules) << "\n";

    std::cout << "exception filter rule: "
              << countRule<ExceptionFilterRule>(rules) << "\n";
    std::cout << "\tbasic match pattern: "
              << countPattern<ExceptionFilterRule, BasicMatchPattern>(rules) << "\n";
    std::cout << "\tbegin match pattern: "
              << countPattern<ExceptionFilterRule, BeginMatchPattern>(rules) << "\n";
    std::cout << "\tend match pattern: "
              << countPattern<ExceptionFilterRule, EndMatchPattern>(rules) << "\n";
    std::cout << "\tdomain match pattern: "
              << countPattern<ExceptionFilterRule, DomainMatchPattern>(rules) << "\n";
    std::cout << "\tregex pattern: "
              << countPattern<ExceptionFilterRule, RegexPattern>(rules) << "\n";

    std::cout << "basic element hide rule: "
              << countRule<BasicElementHideRule>(rules) << "\n";
    std::cout << "exception element hide rule: "
              << countRule<ExceptionElementHideRule>(rules) << "\n";

    std::cout << "total: " << rules.size() << "\n";
}
