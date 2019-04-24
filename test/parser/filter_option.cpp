#include "parser/parser.hpp"

#include "core/string_range.hpp"
#include "rule/filter_option.hpp"
#include "rule/filter_rule.hpp"

#include <memory>

#include <gtest/gtest.h>

using namespace adblock;

TEST(Parser_FilterOption, Script_Normal)
{
    auto const& line = "adblock$script"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(1, filter->numOptions());
    EXPECT_TRUE(filter->hasOption(FilterOption::Script));
}

TEST(Parser_FilterOption, Script_Inverse)
{
    auto const& line = "adblock$~script"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(1, filter->numOptions());
    EXPECT_TRUE(filter->hasOption(FilterOption::ScriptInv));
}

TEST(Parser_FilterOption, Image_Normal)
{
    auto const& line = "adblock$image"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(1, filter->numOptions());
    EXPECT_TRUE(filter->hasOption(FilterOption::Image));
}

TEST(Parser_FilterOption, Image_Inverse)
{
    auto const& line = "adblock$~image"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(1, filter->numOptions());
    EXPECT_TRUE(filter->hasOption(FilterOption::ImageInv));
}

TEST(Parser_FilterOption, StyleSheet_Normal)
{
    auto const& line = "adblock$stylesheet"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(1, filter->numOptions());
    EXPECT_TRUE(filter->hasOption(FilterOption::StyleSheet));
}

TEST(Parser_FilterOption, StyleSheet_Inverse)
{
    auto const& line = "adblock$~stylesheet"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(1, filter->numOptions());
    EXPECT_TRUE(filter->hasOption(FilterOption::StyleSheetInv));
}

TEST(Parser_FilterOption, Object_Normal)
{
    auto const& line = "adblock$object"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(1, filter->numOptions());
    EXPECT_TRUE(filter->hasOption(FilterOption::Object));
}

TEST(Parser_FilterOption, Object_Inverse)
{
    auto const& line = "adblock$~object"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(1, filter->numOptions());
    EXPECT_TRUE(filter->hasOption(FilterOption::ObjectInv));
}

TEST(Parser_FilterOption, XmlHttpRequest_Normal)
{
    auto const& line = "adblock$xmlhttprequest"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(1, filter->numOptions());
    EXPECT_TRUE(filter->hasOption(FilterOption::XmlHttpRequest));
}

TEST(Parser_FilterOption, XmlHttpRequest_Inverse)
{
    auto const& line = "adblock$~xmlhttprequest"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(1, filter->numOptions());
    EXPECT_TRUE(filter->hasOption(FilterOption::XmlHttpRequestInv));
}

TEST(Parser_FilterOption, ObjectSubrequest_Normal)
{
    auto const& line = "adblock$object-subrequest"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(1, filter->numOptions());
    EXPECT_TRUE(filter->hasOption(FilterOption::ObjectSubrequest));
}

TEST(Parser_FilterOption, ObjectSubrequest_Inverse)
{
    auto const& line = "adblock$~object-subrequest"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(1, filter->numOptions());
    EXPECT_TRUE(filter->hasOption(FilterOption::ObjectSubrequestInv));
}

TEST(Parser_FilterOption, Subdocument_Normal)
{
    auto const& line = "adblock$subdocument"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(1, filter->numOptions());
    EXPECT_TRUE(filter->hasOption(FilterOption::Subdocument));
}

TEST(Parser_FilterOption, Subdocument_Inverse)
{
    auto const& line = "adblock$~subdocument"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(1, filter->numOptions());
    EXPECT_TRUE(filter->hasOption(FilterOption::SubdocumentInv));
}

TEST(Parser_FilterOption, Other_Normal)
{
    auto const& line = "adblock$other"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(1, filter->numOptions());
    EXPECT_TRUE(filter->hasOption(FilterOption::Other));
}

TEST(Parser_FilterOption, Other_Inverse)
{
    auto const& line = "adblock$~other"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(1, filter->numOptions());
    EXPECT_TRUE(filter->hasOption(FilterOption::OtherInv));
}

TEST(Parser_FilterOption, ThirdParty_Normal)
{
    auto const& line = "adblock$third-party"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(1, filter->numOptions());
    EXPECT_TRUE(filter->hasOption(FilterOption::ThirdParty));
}

TEST(Parser_FilterOption, ThirdParty_Inverse)
{
    auto const& line = "adblock$~third-party"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(1, filter->numOptions());
    EXPECT_TRUE(filter->hasOption(FilterOption::SameOrigin));
}

TEST(Parser_FilterOption, MatchCase)
{
    auto const& line = "adblock$match-case"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(1, filter->numOptions());
    EXPECT_TRUE(filter->hasOption(FilterOption::MatchCase));
}

TEST(Parser_FilterOption, Collapse_Normal)
{
    auto const& line = "adblock$collapse"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(1, filter->numOptions());
    EXPECT_TRUE(filter->hasOption(FilterOption::AlwaysCollapse));
}

TEST(Parser_FilterOption, Collapse_Inverse)
{
    auto const& line = "adblock$~collapse"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(1, filter->numOptions());
    EXPECT_TRUE(filter->hasOption(FilterOption::NeverCollapse));
}
#if 0
TEST(Parser_FilterOption, DoNotTrack)
{
    auto const& line = "adblock$donottrack"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& options = filter->options();
    ASSERT_EQ(options.size(), 1);

    auto* const option = dynamic_cast<DoNotTrackOption const*>(options.front());
    EXPECT_TRUE(option);
}
#endif
TEST(Parser_FilterOption, Popup)
{
    auto const& line = "adblock$popup"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(1, filter->numOptions());
    EXPECT_TRUE(filter->hasOption(FilterOption::Popup));
}

TEST(Parser_FilterOption, Media_Normal)
{
    auto const& line = "adblock$media"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(1, filter->numOptions());
    EXPECT_TRUE(filter->hasOption(FilterOption::Media));
}

TEST(Parser_FilterOption, Media_Inverse)
{
    auto const& line = "adblock$~media"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(1, filter->numOptions());
    EXPECT_TRUE(filter->hasOption(FilterOption::MediaInv));
}

TEST(Parser_FilterOption, Font_Normal)
{
    auto const& line = "adblock$font"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(1, filter->numOptions());
    EXPECT_TRUE(filter->hasOption(FilterOption::Font));
}

TEST(Parser_FilterOption, Font_Inverse)
{
    auto const& line = "adblock$~font"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(1, filter->numOptions());
    EXPECT_TRUE(filter->hasOption(FilterOption::FontInv));
}

TEST(Parser_FilterOption, Multiple)
{
    auto const& line = "adblock$script,image"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(2, filter->numOptions());
    EXPECT_TRUE(filter->hasOption(FilterOption::Script));
    EXPECT_TRUE(filter->hasOption(FilterOption::Image));
}

TEST(Parser_FilterOption, Multiple_Inverse)
{
    auto const& line = "adblock$~script,~image"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    ASSERT_EQ(2, filter->numOptions());
    EXPECT_TRUE(filter->hasOption(FilterOption::ScriptInv));
    EXPECT_TRUE(filter->hasOption(FilterOption::ImageInv));
}

TEST(Parser_FilterOption, Rewrite)
{
    auto const line = "/https:\\/\\/foo.com\\/file1.txt/$rewrite=/file2.txt"_r;

    auto const rule = parser::parse(line);
    //TODO check error
    ASSERT_FALSE(rule);
}

//TODO $script,~image
//TODO $~script,image
//TODO $~script,script
//TODO $script,~script
