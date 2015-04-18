#include "rule/rule.hpp"
#include "parser/parser.hpp"

#include "rule/comment_rule.hpp"
#include "rule/basic_filter_rule.hpp"
#include "rule/exception_filter_rule.hpp"
#include "rule/basic_element_hide_rule.hpp"
#include "rule/exception_element_hide_rule.hpp"
#include "pattern/basic_match_pattern.hpp"
#include "pattern/domain_match_pattern.hpp"
#include "pattern/regex_pattern.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>

#include <boost/algorithm/string/compare.hpp>
#include <boost/algorithm/string/find_iterator.hpp>
#include <boost/format.hpp>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/program_options.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/timer/timer.hpp>

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

void
printStatistics(const std::vector<std::shared_ptr<Rule>> &rules)
{
    using namespace adblock;
    std::cout << "comment: " << countRule<CommentRule>(rules) << "\n";

    std::cout << "basic filter rule: "
              << countRule<BasicFilterRule>(rules) << "\n";
    std::cout << "\tbasic match pattern: "
              << countPattern<BasicFilterRule, BasicMatchPattern>(rules) << "\n";
    std::cout << "\tdomain match pattern: "
              << countPattern<BasicFilterRule, DomainMatchPattern>(rules) << "\n";
    std::cout << "\tregex pattern: "
              << countPattern<BasicFilterRule, RegexPattern>(rules) << "\n";

    std::cout << "exception filter rule: "
              << countRule<ExceptionFilterRule>(rules) << "\n";
    std::cout << "\tbasic match pattern: "
              << countPattern<ExceptionFilterRule, BasicMatchPattern>(rules) << "\n";
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

int
main(const int argc, const char *argv[])
{
    using namespace adblock;
    namespace bpo = boost::program_options;
    namespace bio = boost::iostreams;

    bpo::options_description desc;
    desc.add_options()
        ("help", "print help message")
        ("input-file", bpo::value<std::string>(), "input file")
        ("stdin", "read rules from stdin")
        ("stats", "print statistics")
        ("map", "search rule by using std::map")
        ;
    bpo::positional_options_description pos;
    pos.add("input-file", -1);

    bpo::variables_map vm;
    bpo::store(
        bpo::command_line_parser(argc, argv)
            .options(desc)
            .positional(pos)
            .run(), vm);
    bpo::notify(vm);

    std::vector<std::shared_ptr<Rule>> rules;
    std::vector<std::string> lines;
    bio::mapped_file_source file;

    if (vm.empty() || vm.count("help")) {
        std::cout << desc << "\n";
        return 1;
    }
    else if (vm.count("stdin")) {
        lines = fromStdIn(rules);
    }
    else if (vm.count("input-file")) {
        //TODO check file's existance.
        const auto &filename = vm["input-file"].as<std::string>();
        file.open(filename);
        assert(file.is_open());

        const auto begin = file.data();
        const auto end = begin + file.size();

        // first line have to be version string
        const auto it = std::find(begin, end, '\n');
        assert(it != end);
        assert(boost::equals(StringRange(begin, it), "[Adblock Plus 2.0]"));

        const auto &bufferR = boost::make_iterator_range(it + 1, end);

        rules = fromBuffer(bufferR);
    }

    if (vm.count("stats")) {
        printStatistics(rules);
    }
    else if (vm.count("map")) {
        using namespace adblock;

        using RuleMap = std::map<
                            StringRange,
                            std::vector<std::shared_ptr<Rule>>,
                            std::greater<StringRange>>;
        RuleMap basicMatchRules;
        RuleMap beginMatchRules;

        boost::timer::cpu_timer t;
        for (auto line = 0u; line < rules.size(); ++line) {
            const auto &rule =
                  std::dynamic_pointer_cast<BasicFilterRule>(rules.at(line));
            if (!rule) continue;

            const auto *pattern =
                  dynamic_cast<const BasicMatchPattern*>(&rule->pattern());
            if (!pattern) continue;

            basicMatchRules[pattern->firstToken()].push_back(rule);
        }
        t.stop();
        std::cout << t.format();
#if 0
        for (const auto &p: basicMatchRules) {
            std::cout << p << "\n";
        }
#endif
        std::cout << "map size: " << basicMatchRules.size() << "\n";

        std::string line;
        do {
            std::cout << "URL: ";
            if (!std::getline(std::cin, line)) break;
            if (line.empty()) break;

            auto begin = line.data();
            const auto end = begin + line.size();
            t.start();
            for (; begin != end; ++begin) {
                const StringRange range { begin, end, };
                const auto it = basicMatchRules.lower_bound(range);

                if (it != basicMatchRules.end()
                                && boost::starts_with(range, it->first))
                {
                    std::cout << "prefix match : "
                              << range << " [" << it->first << "]\n";
                    break;
                }
            }
            t.stop();
            std::cout << t.format();

        } while (true);
    }
}
