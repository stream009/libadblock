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

#include <boost/algorithm/string/compare.hpp>
#include <boost/algorithm/string/find_iterator.hpp>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/range/algorithm.hpp>

using adblock::Rule;

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

std::vector<std::string>
fromStdIn(std::vector<std::shared_ptr<Rule>> &rules)
{
    using adblock::StringRange;

    std::string line;
    std::vector<std::string> lines;

    do {
        std::getline(std::cin, line);
        if (!std::cin) break;

        lines.push_back(line);
        const auto &l = lines.back();
        const StringRange range { l.data(), l.data() + l.size() };
        rules.push_back(adblock::parser::parse(range));

    } while (true);

    return lines;
}

std::vector<std::shared_ptr<Rule>>
fromBuffer(const boost::iterator_range<const char*> &bufferR)
{
    std::vector<std::shared_ptr<Rule>> rules;

    const auto num = boost::count(bufferR, '\n');
    rules.reserve(num);

    for (auto &&lineIt = boost::make_split_iterator(
                              bufferR, first_finder("\n", boost::is_equal()));
         !lineIt.eof();
         ++lineIt)
    {
        const auto &line = *lineIt;
        if (line.empty()) continue;

        rules.push_back(adblock::parser::parse(line));
    }

    return rules;
}

int
main(const int argc, const char *argv[])
{
    namespace bio = boost::iostreams;

    std::vector<std::shared_ptr<Rule>> rules;
    std::vector<std::string> lines;
    bio::mapped_file_source file;

    if (argc == 1) {
        lines = fromStdIn(rules);
    }
    else {
        //TODO check file's existance.
        file.open(argv[1]);
        assert(file.is_open());

        std::cout << argv[1] << ": " << file.size() << "\n";
        const auto &bufferR = boost::make_iterator_range(
                                     file.data(), file.data() + file.size());
        rules = fromBuffer(bufferR);
    }

    using namespace adblock;
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
