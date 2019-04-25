#include "filter_list.hpp"

#include "parser/parser.hpp"
#include "pattern/basic_match_pattern.hpp"
#include "pattern/domain_match_pattern.hpp"
#include "pattern/regex_pattern.hpp"
#include "rule/basic_element_hide_rule.hpp"
#include "rule/basic_filter_rule.hpp"
#include "rule/comment_rule.hpp"
#include "rule/exception_element_hide_rule.hpp"
#include "rule/exception_filter_rule.hpp"
#include "rule/exception_filter_rule.hpp"
#include "rule/extended_element_hide_rule.hpp"
#include "rule/snippet_rule.hpp"

#include <regex>

#include <boost/property_tree/ptree.hpp>
#include <boost/range/adaptor/filtered.hpp>

namespace adblock {

using Rules = FilterList::Rules;
using RulesRange = FilterList::RulesRange;
using const_iterator = FilterList::const_iterator;

static StringRange
ltrim(StringRange const range)
{
    if (range.empty()) return {};

    auto it = range.begin(), end = range.end();
    for(; it != end; ++it) {
        if (*it != ' ') break;
    }

    return { it, end };
}

static StringRange
rtrim(StringRange const range)
{
    if (range.empty()) return {};

    auto it = range.end() - 1, begin = range.begin();
    for(; it != begin; --it) {
        if (*it != ' ') break;
    }

    return { begin, it + 1 };
}

static StringRange
trim(StringRange const range)
{
    return rtrim(ltrim(range));
}

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

    struct SnippetRule {
        size_t generic = 0;
        size_t domainSpecific = 0;
    };

    FilterRule      basicFilterRule;
    FilterRule      exceptionFilterRule;
    ElementHideRule basicElementHideRule;
    ElementHideRule exceptionElementHideRule;
    ElementHideRule extendedElementHideRule;
    SnippetRule     snippetRule;
    size_t          commentRule = 0u;

    Statistics(FilterList::Rules const&);

private:
    void countFilterRule(adblock::FilterRule const& rule,
                         FilterRule& stats);
    void countElementHideRule(adblock::ElementHideRule const& rule,
                              ElementHideRule& stats);
    void countSnippetRule(adblock::SnippetRule const& rule,
                          SnippetRule& stats);
};

Statistics::
Statistics(FilterList::Rules const& rules)
{
    for (auto const& ptr: rules) {
        assert(ptr);
        if (auto* const rule = dynamic_cast<BasicFilterRule*>(&*ptr)) {
            countFilterRule(*rule, basicFilterRule);
        }
        else if (auto* const rule =
                           dynamic_cast<ExceptionFilterRule*>(&*ptr))
        {
            countFilterRule(*rule, exceptionFilterRule);
        }
        else if (auto* const rule =
                            dynamic_cast<BasicElementHideRule*>(&*ptr))
        {
            countElementHideRule(*rule, basicElementHideRule);
        }
        else if (auto* const rule =
                           dynamic_cast<ExceptionElementHideRule*>(&*ptr)) //TODO ptr.get()
        {
            countElementHideRule(*rule, exceptionElementHideRule);
        }
        else if (auto* const rule =
                           dynamic_cast<ExtendedElementHideRule*>(ptr.get()))
        {
            countElementHideRule(*rule, extendedElementHideRule);
        }
        else if (auto* const rule =
            dynamic_cast<adblock::SnippetRule*>(ptr.get()))
        {
            countSnippetRule(*rule, snippetRule);
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
countFilterRule(adblock::FilterRule const& rule, FilterRule& stats)
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
                     ElementHideRule& stats)
{
    ++stats.total;
    if (rule.isDomainRestricted()) {
        ++stats.domainRestricted;
    }
    else {
        ++stats.basic;
    }
}

void Statistics::
countSnippetRule(adblock::SnippetRule const& rule,
                 SnippetRule& stats)
{
    if (rule.domains() != nullptr) {
        ++stats.domainSpecific;
    }
    else {
        ++stats.generic;
    }
}

} // unnamed namespace

// FilterList

FilterList::
FilterList(Path const& filePath)
    : m_file { filePath }
{
    parse(m_file.data(), m_file.size());
}

const_iterator FilterList::
begin() const
{
    return m_file.data();
}

const_iterator FilterList::
end() const
{
    return m_file.data() + m_file.size();
}

RulesRange FilterList::
rules() const
{
    return m_rules | boost::adaptors::indirected;
}

FilterList::Parameters FilterList::
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

void FilterList::
reload()
{
    m_file.reload();

    m_rules.clear();
    parse(m_file.data(), m_file.size());
}

boost::property_tree::ptree FilterList::
statistics() const
{
    using ptree = boost::property_tree::ptree;

    ptree basicFilterRule, exceptionFilterRule,
          basicElementHideRule, exceptionElementHideRule,
          snippetRule;

    Statistics stats { m_rules };

    auto populateFilterRule =
        [](Statistics::FilterRule const& rule, ptree &tree) {
            tree.put<size_t>("Basic match pattern", rule.basicMatchPattern);
            tree.put<size_t>("Domain match pattern", rule.domainMatchPattern);
            tree.put<size_t>("Regex pattern", rule.regexPattern);
            tree.put<size_t>("Total", rule.total);
        };

    populateFilterRule(stats.basicFilterRule, basicFilterRule);
    populateFilterRule(stats.exceptionFilterRule, exceptionFilterRule);

    auto populateElementHideRule =
        [](Statistics::ElementHideRule const& rule, ptree &tree) {
            tree.put<size_t>("Basic", rule.basic);
            tree.put<size_t>("Domain restricted", rule.domainRestricted);
            tree.put<size_t>("Total", rule.total);
        };

    populateElementHideRule(stats.basicElementHideRule, basicElementHideRule);
    populateElementHideRule(
                stats.exceptionElementHideRule, exceptionElementHideRule);
    populateElementHideRule(
                stats.extendedElementHideRule, exceptionElementHideRule);

    snippetRule.put<size_t>("Generic", stats.snippetRule.generic);
    snippetRule.put<size_t>("Domain specific", stats.snippetRule.domainSpecific);
    snippetRule.put<size_t>("Total",   stats.snippetRule.generic
                                     + stats.snippetRule.domainSpecific);

    ptree result, detail;

    result.put("Basic filter rule", stats.basicFilterRule.total);
    result.put("Exception filter rule", stats.exceptionFilterRule.total);
    result.put("Basic element hide rule", stats.basicElementHideRule.total);
    result.put("Exception element hide rule",
                                      stats.exceptionElementHideRule.total);
    result.put("Extended element hide rule",
                                      stats.extendedElementHideRule.total);
    result.put("Snippet rule",   stats.snippetRule.generic
                               + stats.snippetRule.domainSpecific);
    result.put("Comment rule", stats.commentRule);

    detail.put_child("Basic filter rule", basicFilterRule);
    detail.put_child("Exceptin filter rule", exceptionFilterRule);
    detail.put_child("Basic element hide rule", basicElementHideRule);
    detail.put_child("Exception element hide rule", exceptionElementHideRule);
    detail.put_child("Exception element hide rule", exceptionElementHideRule);
    detail.put_child("Snippet rule", snippetRule);

    result.put_child("detail", detail);

    return result;
}

void FilterList::
parse(char const* const buffer, size_t const size)
{
    StringRange const buf { buffer, buffer + size };
    m_rules = parser::parse(*this, buf);
}

} // namespace adblock
