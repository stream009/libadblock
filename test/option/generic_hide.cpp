#include "../parse_rule.hpp"

#include "core/element_hide_rule_base.hpp"
#include "core/filter_rule_base.hpp"
#include "core/string_range.hpp"
#include "core/uri.hpp"
#include "rule/basic_element_hide_rule.hpp"

#include <gtest/gtest.h>

namespace adblock {

template<typename T, typename U>
static bool
contains(std::vector<T> const& vec, U const& value)
{
    return std::find_if(
        vec.begin(), vec.end(),
        [&](auto& v) { return v == value; }
    ) != vec.end();
}

TEST(Option_GenericHide, Elementary)
{
    FilterRuleBase fb;
    ElementHideRuleBase rb { fb };

    auto const rule1 = parse_rule<BasicElementHideRule>("##div"_r);
    ASSERT_TRUE(rule1);

    rb.put(*rule1);

    { // elment hide rule "div" should be applied
        auto const& rules = rb.lookupRules("http://www.adblock.org"_u);
        ASSERT_EQ(1, rules.size());
        EXPECT_EQ(rule1.get(), rules[0]);
    }

    auto const disabler = parse_rule<FilterRule>("@@||adblock.org$generichide"_r);
    ASSERT_TRUE(disabler);

    fb.put(*disabler);

    { // element hide rule "div" should be excluded by generichide rule
        auto const& rules = rb.lookupRules("http://www.adblock.org"_u);
        ASSERT_TRUE(rules.empty());
    }
}

TEST(Option_GenericHide, OnlyGenericShouldBeHidden)
{
    FilterRuleBase fb;
    ElementHideRuleBase rb { fb };

    auto const rule1 = parse_rule<BasicElementHideRule>("##div"_r);
    ASSERT_TRUE(rule1);

    rb.put(*rule1);

    auto const rule2 = parse_rule<BasicElementHideRule>("adblock.org##table"_r);
    ASSERT_TRUE(rule2);

    rb.put(*rule2);

    { // both generic and domain specific rule should be applied
        auto const& rules = rb.lookupRules("http://www.adblock.org"_u);
        ASSERT_EQ(2, rules.size());
        EXPECT_TRUE(contains(rules, rule1.get()));
        EXPECT_TRUE(contains(rules, rule2.get()));
    }

    auto const disabler = parse_rule<FilterRule>("@@||adblock.org$generichide"_r);
    ASSERT_TRUE(disabler);

    fb.put(*disabler);

    { // generic rule should be hidden but domained rule should remain
        auto const& rules = rb.lookupRules("http://www.adblock.org"_u);
        ASSERT_EQ(1, rules.size());
        EXPECT_TRUE(contains(rules, rule2.get()));
    }
}

} // namespace adblock
