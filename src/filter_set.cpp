#include "filter_set.hpp"

#include "parser/parser.hpp"
#include "pattern/basic_match_pattern.hpp"
#include "pattern/domain_match_pattern.hpp"
#include "pattern/regex_pattern.hpp"
#include "rule/basic_element_hide_rule.hpp"
#include "rule/basic_filter_rule.hpp"
#include "rule/comment_rule.hpp"
#include "rule/exception_element_hide_rule.hpp"
#include "rule/exception_filter_rule.hpp"

#include <iomanip>
#include <regex>

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

    Statistics(FilterSet::Rules const&);

private:
    void countFilterRule(adblock::FilterRule const& rule,
                         FilterRule &stats);
    void countElementHideRule(adblock::ElementHideRule const& rule,
                              ElementHideRule &stats);
};

Statistics::
Statistics(FilterSet::Rules const& rules)
{
    for (auto const& ptr: rules) {
        assert(ptr);
        if (auto const* const rule = dynamic_cast<BasicFilterRule*>(&*ptr)) {
            countFilterRule(*rule, basicFilterRule);
        }
        else if (auto const* const rule =
                           dynamic_cast<ExceptionFilterRule*>(&*ptr))
        {
            countFilterRule(*rule, exceptionFilterRule);
        }
        else if (auto const* const rule =
                            dynamic_cast<BasicElementHideRule*>(&*ptr))
        {
            countElementHideRule(*rule, basicElementHideRule);
        }
        else if (auto const* const rule =
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
countFilterRule(adblock::FilterRule const& rule, FilterRule &stats)
{
    ++stats.total;
    auto const& pattern = rule.pattern();
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
countElementHideRule(adblock::ElementHideRule const& rule,
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

char const* FilterSet::ParseError::
what() const noexcept
{
    return "Error: Fail to parse header.";
}

// FilterSet

FilterSet::
FilterSet(Path const& filePath)
    : m_file { filePath }
{
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

static StringRange
ltrim(StringRange const& range)
{
    if (range.empty()) return {};

    auto it = range.begin(), end = range.end();
    for(; it != end; ++it) {
        if (*it != ' ') break;
    }

    return { it, end };
}

static StringRange
rtrim(StringRange const& range)
{
    if (range.empty()) return {};

    auto it = range.end() - 1, begin = range.begin();
    for(; it != begin; --it) {
        if (*it != ' ') break;
    }

    return { begin, it + 1 };
}

static StringRange
trim(StringRange const& range)
{
    return rtrim(ltrim(range));
}

FilterSet::Parameters FilterSet::
parameters() const
{
    namespace bad = boost::adaptors;

    Parameters result;

    auto isCommentRule = [](auto const& rule) -> bool {
        return dynamic_cast<CommentRule*>(rule.get());
    };

    auto const& commentRules = m_rules | bad::filtered(isCommentRule);

    static std::regex const re {
        R"(^!\s*(checksum|redirect|homepage|title|version|expires)\s*:\s*(.*)$)",
        std::regex_constants::icase
    };

    for (auto const& rule: commentRules) {
        auto const& line = rule->line();

        std::cmatch m;
        auto const match = std::regex_match(
            line.begin(), line.end(),
            m,
            re
        );
        if (!match) continue;

        StringRange const key { m[1].first, m[1].second };
        StringRange const value { m[2].first, m[2].second };

        result.emplace(key, trim(value));
    }

    return result;
}

StringRange FilterSet::
supportedVersion()
{
    return "2.0"_r; //TODO configurable
}

void FilterSet::
reload()
{
    m_file.reload();

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

    auto const& populateFilterRule =
        [](Statistics::FilterRule const& rule, ptree &tree) {
            tree.put<size_t>("Basic match pattern", rule.basicMatchPattern);
            tree.put<size_t>("Domain match pattern", rule.domainMatchPattern);
            tree.put<size_t>("Regex pattern", rule.regexPattern);
            tree.put<size_t>("Total", rule.total);
        };
    populateFilterRule(stats.basicFilterRule, basicFilterRule);
    populateFilterRule(stats.exceptionFilterRule, exceptionFilterRule);

    auto const& populateElementHideRule =
        [](Statistics::ElementHideRule const& rule, ptree &tree) {
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
parse(char const* const buffer, size_t const size)
{
    auto const* begin = buffer, *end = buffer + size;

    // first line have to be version string
    auto const it = std::find(begin, end, '\n');
    assert(it != end);
    StringRange const firstLine { begin, it };
    auto const& version = parser::parseHeader(firstLine);
    if (version.empty()) {
        throw FilterSet::ParseError {};
    }
    else if (version != FilterSet::supportedVersion()) {
        std::cerr << "Warning: Unsupported version of filter set has detected: "
                  << version << "\n"
                  << "Continue processing anyway.\n";
    }

    StringRange const bufferR { it + 1, end };
    auto const num = boost::count(bufferR, '\n');
    m_rules.reserve(num);

    for (auto &&lineIt = boost::make_split_iterator(
                      bufferR, first_finder("\n", boost::is_equal()));
         !lineIt.eof();
         ++lineIt)
    {
        auto const& line = *lineIt;
        if (line.empty()) continue;

        auto &&rule = parser::parse(line);
        if (!rule) continue;

        rule->setFilterSet(*this);
        rule->setLine(line);

        m_rules.push_back(std::move(rule));
    }
}

} // namespace adblock
