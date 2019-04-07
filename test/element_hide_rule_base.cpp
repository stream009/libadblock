#include "type.hpp"
#include "filter_rule_base.hpp"
#include "element_hide_rule_base.hpp"
#include "rule/basic_element_hide_rule.hpp"
#include "rule/exception_element_hide_rule.hpp"
#include "rule/extended_element_hide_rule.hpp"
#include "parser/parser.hpp"

#include <memory>

#include <boost/optional.hpp>

#include <gtest/gtest.h>

using namespace adblock;

using Domains = ElementHideRule::Domains;

template<typename T>
static std::shared_ptr<ElementHideRule>
make_rule(const StringRange &cssSelector,
          const boost::optional<Domains> &domains)
{
    return std::make_shared<T>(cssSelector, domains);
}

TEST(Main_ElementHideRuleBase, Elementary)
{
    const auto &rule =
                make_rule<BasicElementHideRule>("div"_r, boost::none);
    assert(rule);

    FilterRuleBase fb;
    ElementHideRuleBase ruleBase { fb };
    ruleBase.put(*rule);

    const auto &result = ruleBase.query("http://www.adblock.org"_u);
    EXPECT_EQ("div { display: none !important } ", result);
}

TEST(Main_ElementHideRuleBase, Domained)
{
    const auto &rule1 =
                make_rule<BasicElementHideRule>("div"_r, boost::none);
    assert(rule1);

    const auto &rule2 = make_rule<BasicElementHideRule>(
                                    "table"_r, Domains { "adblock.org"_r });
    assert(rule2);

    FilterRuleBase fb;
    ElementHideRuleBase ruleBase { fb };

    ruleBase.put(*rule1);
    ruleBase.put(*rule2);

    const auto &result = ruleBase.query("http://www.adblock.org"_u);
    // either is ok.
    //EXPECT_EQ("table, div { display: none !important }", result);
    EXPECT_EQ("div, table { display: none !important } ", result);
}

TEST(Main_ElementHideRuleBase, ExcludedByExceptionRule)
{
    const auto &rule1 =
                make_rule<BasicElementHideRule>("div"_r, boost::none);
    assert(rule1);

    const auto &rule2 = make_rule<ExceptionElementHideRule>(
                                    "div"_r, Domains { "adblock.org"_r });
    assert(rule2);

    FilterRuleBase fb;
    ElementHideRuleBase ruleBase { fb };

    ruleBase.put(*rule1);
    ruleBase.put(*rule2);

    const auto &result = ruleBase.query("http://www.adblock.org"_u);
    EXPECT_EQ("", result);
}

TEST(Main_ElementHideRuleBase, ExcludedByGenericExceptionRule)
{
    FilterRuleBase fb;
    ElementHideRuleBase ruleBase { fb };

    auto const& rule1 = std::dynamic_pointer_cast<ElementHideRule>(
                                 parser::parse("adblock.org##div"_r));
    assert(rule1);
    ruleBase.put(*rule1);

    {
        auto const& result = ruleBase.query("http://www.adblock.org"_u);
        EXPECT_EQ("div { display: none !important } ", result);
    }

    auto const& rule2 = std::dynamic_pointer_cast<ElementHideRule>(
                                 parser::parse("#@#div"_r));
    assert(rule2);
    ruleBase.put(*rule2);

    {
        auto const& result = ruleBase.query("http://www.adblock.org"_u);
        EXPECT_EQ("", result);
    }
}

TEST(Main_ElementHideRuleBase, DomainMatchWithExceptionRuleButSelectorIsnTSame)
{
    const auto &rule1 =
                make_rule<BasicElementHideRule>("div"_r, boost::none);
    assert(rule1);

    const auto &rule2 = make_rule<ExceptionElementHideRule>(
                                    "table"_r, Domains { "adblock.org"_r });
    assert(rule2);

    FilterRuleBase fb;
    ElementHideRuleBase ruleBase { fb };

    ruleBase.put(*rule1);
    ruleBase.put(*rule2);

    const auto &result = ruleBase.query("http://www.adblock.org"_u);
    EXPECT_EQ("div { display: none !important } ", result);
}

TEST(Main_ElementHideRuleBase, Clear)
{
    const auto &rule1 =
                make_rule<BasicElementHideRule>("div"_r, boost::none);
    assert(rule1);

    const auto &rule2 = make_rule<ExceptionElementHideRule>(
                                    "table"_r, Domains { "adblock.org"_r });
    assert(rule2);

    FilterRuleBase fb;
    ElementHideRuleBase ruleBase { fb };

    ruleBase.put(*rule1);
    ruleBase.put(*rule2);

    auto stats = ruleBase.statistics();
    EXPECT_EQ(2, stats.get<size_t>("Total"));

    ruleBase.clear();

    stats = ruleBase.statistics();
    EXPECT_EQ(0, stats.get<size_t>("Total"));
}

TEST(Main_ElementHideRuleBase, ExtendedRule)
{
    auto const& rule =
                make_rule<ExtendedElementHideRule>("div"_r, boost::none);
    assert(rule);

    FilterRuleBase fb;
    ElementHideRuleBase ruleBase { fb };
    ruleBase.put(*rule);

    auto const& uri = "http://www.adblock.org"_u;

    auto const& result = ruleBase.query(uri);
    EXPECT_EQ("", result);

    auto const& rules = ruleBase.lookupExtendedRule(uri);
    ASSERT_EQ(1, rules.size());

    auto const& selector = rules[0]->cssSelector();
    EXPECT_TRUE("div"_r == selector) << selector;
}

TEST(Main_ElementHideRuleBase, DomainedExtendedRule)
{
    auto const& rule1 =
                make_rule<ExtendedElementHideRule>("div"_r, boost::none);
    assert(rule1);

    auto const& rule2 = make_rule<ExtendedElementHideRule>(
                                    "table"_r, Domains { "adblock.org"_r });
    assert(rule2);

    FilterRuleBase fb;
    ElementHideRuleBase ruleBase { fb };

    ruleBase.put(*rule1);
    ruleBase.put(*rule2);

    auto const& uri = "http://www.adblock.org"_u;

    auto const& result = ruleBase.query(uri);
    EXPECT_EQ("", result);

    auto const& rules = ruleBase.lookupExtendedRule(uri);
    ASSERT_EQ(2, rules.size());

    EXPECT_TRUE("div"_r == rules[0]->cssSelector());
    EXPECT_TRUE("table"_r == rules[1]->cssSelector());
}

TEST(Main_ElementHideRuleBase, Extended_ExcludedByExceptionRule)
{
    auto const& rule1 =
                make_rule<ExtendedElementHideRule>("div"_r, boost::none);
    assert(rule1);

    auto const& rule2 = make_rule<ExceptionElementHideRule>(
                                    "div"_r, Domains { "adblock.org"_r });
    assert(rule2);

    FilterRuleBase fb;
    ElementHideRuleBase ruleBase { fb };

    ruleBase.put(*rule1);
    ruleBase.put(*rule2);

    auto const& uri = "http://www.adblock.org"_u;

    auto const& result = ruleBase.query(uri);
    EXPECT_EQ("", result);

    auto const& rules = ruleBase.lookupExtendedRule(uri);
    EXPECT_TRUE(rules.empty());
}

TEST(Main_ElementHideRuleBase, ExtendedRule_DomainMatchWithExceptionRuleButSelectorIsnTSame)
{
    auto const& rule1 =
                make_rule<ExtendedElementHideRule>("div"_r, boost::none);
    assert(rule1);

    auto const& rule2 = make_rule<ExceptionElementHideRule>(
                                    "table"_r, Domains { "adblock.org"_r });
    assert(rule2);

    FilterRuleBase fb;
    ElementHideRuleBase ruleBase { fb };

    ruleBase.put(*rule1);
    ruleBase.put(*rule2);

    auto const& uri = "http://www.adblock.org"_u;

    const auto &result = ruleBase.query(uri);
    EXPECT_TRUE(result.empty());

    auto const& rules = ruleBase.lookupExtendedRule(uri);
    ASSERT_EQ(1, rules.size());
    EXPECT_EQ("div"_r, rules[0]->cssSelector());
}

TEST(Main_ElementHideRuleBase, ExtendedRule_Clear)
{
    auto const& rule1 =
                make_rule<ExtendedElementHideRule>("div"_r, boost::none);
    assert(rule1);

    auto const& rule2 = make_rule<ExceptionElementHideRule>(
                                    "table"_r, Domains { "adblock.org"_r });
    assert(rule2);

    FilterRuleBase fb;
    ElementHideRuleBase ruleBase { fb };

    ruleBase.put(*rule1);
    ruleBase.put(*rule2);

    auto stats = ruleBase.statistics();
    EXPECT_EQ(2, stats.get<size_t>("Total"));

    ruleBase.clear();

    stats = ruleBase.statistics();
    EXPECT_EQ(0, stats.get<size_t>("Total"));
}
