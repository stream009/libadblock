#include "../parse_rule.hpp"

#include "core/element_hide_rule_base.hpp"
#include "core/filter_rule_base.hpp"
#include "core/string_range.hpp"
#include "core/uri.hpp"
#include "rule/basic_element_hide_rule.hpp"
#include "rule/filter_rule.hpp"

#include <gtest/gtest.h>

namespace adblock {

TEST(Option_ElemHide, Elementary)
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

    auto const disabler = parse_rule<FilterRule>("@@||adblock.org$elemhide"_r);
    ASSERT_TRUE(disabler);

    fb.put(*disabler);

    { // element hide rule "div" should be excluded by elemhide rule
        auto const& rules = rb.lookupRules("http://www.adblock.org"_u);
        ASSERT_TRUE(rules.empty());
    }
}

TEST(Option_ElemHide, Inversed)
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

    auto const disabler = parse_rule<FilterRule>("@@||adblock.org$~elemhide"_r);
    ASSERT_TRUE(disabler);

    fb.put(*disabler);

    { // inverted elemhide rule should be just ignored
        auto const& rules = rb.lookupRules("http://www.adblock.org"_u);
        ASSERT_EQ(1, rules.size());
        EXPECT_EQ(rule1.get(), rules[0]);
    }
}

} // namespace adblock
