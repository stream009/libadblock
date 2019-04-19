#include "../parse_rule.hpp"

#include "core/element_hide_rule_base.hpp"
#include "core/filter_rule_base.hpp"
#include "rule/basic_element_hide_rule.hpp"

#include <gtest/gtest.h>

namespace adblock {

TEST(Option_GenericHide, Elementary)
{
    FilterRuleBase fb;
    ElementHideRuleBase rb { fb };

    auto const rule1 = parse_rule<BasicElementHideRule>("##div"_r);
    ASSERT_TRUE(rule1);

    rb.put(*rule1);

    { // elment hide rule "div" should be applied
        auto const& result = rb.query("http://www.adblock.org"_u);
        ASSERT_EQ("div { display: none !important } ", result);
    }

    auto const disabler = parse_rule<FilterRule>("@@||adblock.org$generichide"_r);
    ASSERT_TRUE(disabler);

    fb.put(*disabler);

    { // element hide rule "div" should be excluded by generichide rule
        auto const& result = rb.query("http://www.adblock.org"_u);
        ASSERT_TRUE(result.empty()) << result;
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
        auto const& result = rb.query("http://www.adblock.org"_u);
        ASSERT_EQ("div, table { display: none !important } ", result) << result;
    }

    auto const disabler = parse_rule<FilterRule>("@@||adblock.org$generichide"_r);
    ASSERT_TRUE(disabler);

    fb.put(*disabler);

    { // generic rule should be hidden but domained rule should remain
        auto const& result = rb.query("http://www.adblock.org"_u);
        ASSERT_EQ("table { display: none !important } ", result) << result;
    }
}

} // namespace adblock
