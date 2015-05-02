#include "subscription.hpp"

#include <iomanip>

#include "parser/parser.hpp"
#include "pattern/basic_match_pattern.hpp"
#include "pattern/domain_match_pattern.hpp"
#include "pattern/regex_pattern.hpp"
#include "rule/basic_filter_rule.hpp"
#include "rule/exception_filter_rule.hpp"
#include "rule/basic_element_hide_rule.hpp"
#include "rule/exception_element_hide_rule.hpp"
#include "rule/comment_rule.hpp"

namespace adblock {

using Rules = Subscription::Rules;
using RulesRange = Subscription::RulesRange;
using const_iterator = Subscription::const_iterator;

static Rules
parse(const char *buffer, const size_t size)
{
    const auto *begin = buffer, *end = buffer + size;

    // first line have to be version string
    const auto it = std::find(begin, end, '\n');
    assert(it != end);
    const StringRange firstLine { begin, it };
    const auto &version = parser::parseHeader(firstLine);
    if (version.empty()) {
        throw Subscription::ParseError {};
    }
    else if (version != Subscription::supportedVersion()) {
        std::cerr << "Warning: Unsupported version of subscription is detected: "
                  << version << "\n"
                  << "Continuing process anyway.\n";
    }

    const StringRange bufferR { it + 1, end };
    const auto num = boost::count(bufferR, '\n');
    Rules results;
    results.reserve(num);

    for (auto &&lineIt = boost::make_split_iterator(
                      bufferR, first_finder("\n", boost::is_equal()));
         !lineIt.eof();
         ++lineIt)
    {
        const auto &line = *lineIt;
        if (line.empty()) continue;

        results.push_back(parser::parse(line));
    }

    return results;
}


const char*Subscription::ParseError::
what() const noexcept
{
    return "Error: Fail to parse header.";
}

Subscription::
Subscription(const Path &filePath)
{
    m_file.open(filePath);
    assert(m_file.is_open());

    m_rules = parse(m_file.data(), m_file.size());
}

const_iterator Subscription::
begin() const
{
    return m_file.data();
}

const_iterator Subscription::
end() const
{
    return m_file.data() + m_file.size();
}

RulesRange Subscription::
rules() const
{
    return m_rules;
}

const StringRange Subscription::
supportedVersion()
{
    static const StringRange &result = "2.0"_r; //TODO configurable
    return result;
}

static void
countFilterRule(const FilterRule &rule,
                Subscription::Statistics::FilterRule &stats)
{
    ++stats.total;
    const auto &pattern = rule.pattern();
    if (typeid(pattern) == typeid(BasicMatchPattern)) {
        ++stats.basicMatchPattern;
    }
    else if (typeid(pattern) == typeid(DomainMatchPattern)) {
        ++stats.domainMatchPattern;
    }
    else if (typeid(pattern) == typeid(RegexPattern)) {
        ++stats.regexPattern;
    }
    else {
        assert(false && "unknown type of pattern");
    }
}

static void
countElementHideRule(const ElementHideRule &rule,
                Subscription::Statistics::ElementHideRule &stats)
{
    ++stats.total;
    if (rule.isDomainRestricted()) {
        ++stats.domainRestricted;
    }
    else {
        ++stats.basic;
    }
}

Subscription::Statistics Subscription::
statistics() const
{
    Statistics result {};

    for (const auto &ptr: m_rules) {
        assert(ptr);
        if (const auto *rule = dynamic_cast<BasicFilterRule*>(&*ptr)) {
            countFilterRule(*rule, result.basicFilterRule);
        }
        else if (const auto *rule =
                           dynamic_cast<ExceptionFilterRule*>(&*ptr))
        {
            countFilterRule(*rule, result.exceptionFilterRule);
        }
        else if (const auto *rule =
                            dynamic_cast<BasicElementHideRule*>(&*ptr))
        {
            countElementHideRule(*rule, result.basicElementHideRule);
        }
        else if (const auto *rule =
                           dynamic_cast<ExceptionElementHideRule*>(&*ptr))
        {
            countElementHideRule(*rule, result.exceptionElementHideRule);
        }
        else if (dynamic_cast<CommentRule*>(&*ptr)) {
            ++result.commentRule;
        }
        else {
            assert(false && "unknown type of rule");
        }
    }

    return result;
}

static void
println(std::ostream &os, const char *label, const size_t value)
{
    os << std::right << std::setw(22) << label
                     << std::setw(8) << value << "\n";
}

static void
println2(std::ostream &os, const char *label, const size_t value)
{
    os << std::left << std::setw(22) << label
       << std::right << std::setw(8) << value << "\n";
}

static void
printFilterRuleStatistics(std::ostream &os,
                          const Subscription::Statistics::FilterRule &stats)
{
    println(os, "Basic Match Pattern: ", stats.basicMatchPattern);
    println(os, "Domain Match Pattern: ", stats.domainMatchPattern);
    println(os, "Regex Pattern: ", stats.regexPattern);
    println(os ,"Total: ", stats.total);
}

static void
printElementHideRuleStatistics(std::ostream &os,
                    const Subscription::Statistics::ElementHideRule &stats)
{
    println(os ,"Basic: ", stats.basic);
    println(os ,"Domain Restristed: ", stats.domainRestricted);
    println(os ,"Total: ", stats.total);
}

std::ostream &operator<<(std::ostream &os,
                         const Subscription::Statistics &stats)
{
    os << "[Basic Filter Rule]\n";
    printFilterRuleStatistics(os, stats.basicFilterRule);
    os << "\n";

    os << "[Exception Filter Rule]\n";
    printFilterRuleStatistics(os, stats.exceptionFilterRule);
    os << "\n";

    os << "[Basic Element Hide Rule]\n";
    printElementHideRuleStatistics(os, stats.basicElementHideRule);
    os << "\n";

    os << "[Exception Element Hide Rule]\n";
    printElementHideRuleStatistics(os, stats.exceptionElementHideRule);
    os << "\n";

    println2(os, "[Comment Rule]", stats.commentRule);
    os << "\n";

    println2(os, "[Total]",
        stats.basicFilterRule.total +
        stats.exceptionFilterRule.total +
        stats.basicElementHideRule.total +
        stats.exceptionElementHideRule.total +
        stats.commentRule);

    return os;
}

} // namespace adblock
