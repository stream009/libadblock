#include "filter_set.hpp"

#include "parser/parser.hpp"
#include "pattern/basic_match_pattern.hpp"
#include "pattern/domain_match_pattern.hpp"
#include "pattern/regex_pattern.hpp"
#include "rule/basic_filter_rule.hpp"
#include "rule/exception_filter_rule.hpp"
#include "rule/basic_element_hide_rule.hpp"
#include "rule/exception_element_hide_rule.hpp"
#include "rule/comment_rule.hpp"

#include <iomanip>

#include <boost/property_tree/ptree.hpp>

namespace adblock {

using Rules = FilterSet::Rules;
using RulesRange = FilterSet::RulesRange;
using const_iterator = FilterSet::const_iterator;

namespace {

struct Statistics
{
    struct FilterRule {
        size_t total = 0u;
        size_t basicMatchPattern = 0u;
        size_t domainMatchPattern = 0u;
        size_t regexPattern = 0u;
    };

    struct ElementHideRule {
        size_t total = 0u;
        size_t basic = 0u;
        size_t domainRestricted = 0u;
    };

    FilterRule      basicFilterRule;
    FilterRule      exceptionFilterRule;
    ElementHideRule basicElementHideRule;
    ElementHideRule exceptionElementHideRule;
    size_t          commentRule = 0u;

    Statistics(const FilterSet::Rules&);

private:
    void countFilterRule(const adblock::FilterRule &rule,
                         FilterRule &stats);
    void countElementHideRule(const adblock::ElementHideRule &rule,
                              ElementHideRule &stats);
};

Statistics::
Statistics(const FilterSet::Rules &rules)
{
    for (const auto &ptr: rules) {
        assert(ptr);
        if (const auto *rule = dynamic_cast<BasicFilterRule*>(&*ptr)) {
            countFilterRule(*rule, basicFilterRule);
        }
        else if (const auto *rule =
                           dynamic_cast<ExceptionFilterRule*>(&*ptr))
        {
            countFilterRule(*rule, exceptionFilterRule);
        }
        else if (const auto *rule =
                            dynamic_cast<BasicElementHideRule*>(&*ptr))
        {
            countElementHideRule(*rule, basicElementHideRule);
        }
        else if (const auto *rule =
                           dynamic_cast<ExceptionElementHideRule*>(&*ptr))
        {
            countElementHideRule(*rule, exceptionElementHideRule);
        }
        else if (dynamic_cast<CommentRule*>(&*ptr)) {
            ++commentRule;
        }
        else {
            assert(false && "unknown type of rule");
        }
    }
}

void Statistics::
countFilterRule(const adblock::FilterRule &rule, FilterRule &stats)
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

void Statistics::
countElementHideRule(const adblock::ElementHideRule &rule,
                    ElementHideRule &stats)
{
    ++stats.total;
    if (rule.isDomainRestricted()) {
        ++stats.domainRestricted;
    }
    else {
        ++stats.basic;
    }
}

} // unnamed namespace

const char*FilterSet::ParseError::
what() const noexcept
{
    return "Error: Fail to parse header.";
}

FilterSet::
FilterSet(const Path &filePath)
    : m_path { filePath }
{
    m_file.open(filePath);
    assert(m_file.is_open());
    assert(!m_path.empty());

    parse(m_file.data(), m_file.size());
}

const_iterator FilterSet::
begin() const
{
    return m_file.data();
}

const_iterator FilterSet::
end() const
{
    return m_file.data() + m_file.size();
}

RulesRange FilterSet::
rules() const
{
    return m_rules | boost::adaptors::indirected;
}

const StringRange FilterSet::
supportedVersion()
{
    static const StringRange &result = "2.0"_r; //TODO configurable
    return result;
}

void FilterSet::
reload()
{
    m_file.close();
    m_file.open(m_path);
    m_rules.clear();
    parse(m_file.data(), m_file.size());
}

boost::property_tree::ptree FilterSet::
statistics() const
{
    using ptree = boost::property_tree::ptree;

    ptree basicFilterRule, exceptionFilterRule,
          basicElementHideRule, exceptionElementHideRule;

    Statistics stats { m_rules };

    const auto &populateFilterRule =
        [](const Statistics::FilterRule &rule, ptree &tree) {
            tree.put<size_t>("Basic match pattern", rule.basicMatchPattern);
            tree.put<size_t>("Domain match pattern", rule.domainMatchPattern);
            tree.put<size_t>("Regex pattern", rule.regexPattern);
            tree.put<size_t>("Total", rule.total);
        };
    populateFilterRule(stats.basicFilterRule, basicFilterRule);
    populateFilterRule(stats.exceptionFilterRule, exceptionFilterRule);

    const auto &populateElementHideRule =
        [](const Statistics::ElementHideRule &rule, ptree &tree) {
            tree.put<size_t>("Basic", rule.basic);
            tree.put<size_t>("Domain restricted", rule.domainRestricted);
            tree.put<size_t>("Total", rule.total);
        };
    populateElementHideRule(stats.basicElementHideRule, basicElementHideRule);
    populateElementHideRule(
                stats.exceptionElementHideRule, exceptionElementHideRule);

    ptree result, detail;

    result.put("Basic filter rule", stats.basicFilterRule.total);
    result.put("Exception filter rule", stats.exceptionFilterRule.total);
    result.put("Basic element hide rule", stats.basicElementHideRule.total);
    result.put("Exception element hide rule",
                                      stats.exceptionElementHideRule.total);
    result.put("Comment rule", stats.commentRule);

    detail.put_child("Basic filter rule", basicFilterRule);
    detail.put_child("Exceptin filter rule", exceptionFilterRule);
    detail.put_child("Basic element hide rule", basicElementHideRule);
    detail.put_child("Exception element hide rule", exceptionElementHideRule);

    result.put_child("detail", detail);

    return result;
}

void FilterSet::
parse(const char *buffer, const size_t size)
{
    const auto *begin = buffer, *end = buffer + size;

    // first line have to be version string
    const auto it = std::find(begin, end, '\n');
    assert(it != end);
    const StringRange firstLine { begin, it };
    const auto &version = parser::parseHeader(firstLine);
    if (version.empty()) {
        throw FilterSet::ParseError {};
    }
    else if (version != FilterSet::supportedVersion()) {
        std::cerr << "Warning: Unsupported version of filter set is detected: "
                  << version << "\n"
                  << "Continuing process anyway.\n";
    }

    const StringRange bufferR { it + 1, end };
    const auto num = boost::count(bufferR, '\n');
    m_rules.reserve(num);

    for (auto &&lineIt = boost::make_split_iterator(
                      bufferR, first_finder("\n", boost::is_equal()));
         !lineIt.eof();
         ++lineIt)
    {
        const auto &line = *lineIt;
        if (line.empty()) continue;

        auto &&rule = parser::parse(line);
        rule->setFilterSet(*this);
        rule->setLine(line);

        m_rules.push_back(std::move(rule));
    }
}

} // namespace adblock
