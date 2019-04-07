#include "parser/parser.hpp"

#include "option.hpp"
#include "option/collapse_option.hpp"
#include "option/do_not_track_option.hpp"
#include "option/font_option.hpp"
#include "option/image_option.hpp"
#include "option/match_case_option.hpp"
#include "option/media_option.hpp"
#include "option/object_option.hpp"
#include "option/object_subrequest_option.hpp"
#include "option/other_option.hpp"
#include "option/popup_option.hpp"
#include "option/script_option.hpp"
#include "option/style_sheet_option.hpp"
#include "option/subdocument_option.hpp"
#include "option/third_party_option.hpp"
#include "option/xml_http_request_option.hpp"
#include "rule/filter_rule.hpp"
#include "type.hpp"

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

    auto const& options = filter->options();
    ASSERT_EQ(options.size(), 1);

    auto* const option = dynamic_cast<ScriptOption*>(options.front().get());
    ASSERT_TRUE(option);
    EXPECT_FALSE(option->inverse());
}

TEST(Parser_FilterOption, Script_Inverse)
{
    auto const& line = "adblock$~script"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& options = filter->options();
    ASSERT_EQ(options.size(), 1);

    auto* const option = dynamic_cast<ScriptOption*>(options.front().get());
    ASSERT_TRUE(option);
    EXPECT_TRUE(option->inverse());
}

TEST(Parser_FilterOption, Image_Normal)
{
    auto const& line = "adblock$image"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& options = filter->options();
    ASSERT_EQ(options.size(), 1);

    auto* const option = dynamic_cast<ImageOption*>(options.front().get());
    ASSERT_TRUE(option);
    EXPECT_FALSE(option->inverse());
}

TEST(Parser_FilterOption, Image_Inverse)
{
    auto const& line = "adblock$~image"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& options = filter->options();
    ASSERT_EQ(options.size(), 1);

    auto* const option = dynamic_cast<ImageOption*>(options.front().get());
    ASSERT_TRUE(option);
    EXPECT_TRUE(option->inverse());
}

TEST(Parser_FilterOption, StyleSheet_Normal)
{
    auto const& line = "adblock$stylesheet"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& options = filter->options();
    ASSERT_EQ(options.size(), 1);

    auto* const option = dynamic_cast<StyleSheetOption*>(options.front().get());
    ASSERT_TRUE(option);
    EXPECT_FALSE(option->inverse());
}

TEST(Parser_FilterOption, StyleSheet_Inverse)
{
    auto const& line = "adblock$~stylesheet"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& options = filter->options();
    ASSERT_EQ(options.size(), 1);

    auto* const option = dynamic_cast<StyleSheetOption*>(options.front().get());
    ASSERT_TRUE(option);
    EXPECT_TRUE(option->inverse());
}

TEST(Parser_FilterOption, Object_Normal)
{
    auto const& line = "adblock$object"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& options = filter->options();
    ASSERT_EQ(options.size(), 1);

    auto* const option = dynamic_cast<ObjectOption*>(options.front().get());
    ASSERT_TRUE(option);
    EXPECT_FALSE(option->inverse());
}

TEST(Parser_FilterOption, Object_Inverse)
{
    auto const& line = "adblock$~object"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& options = filter->options();
    ASSERT_EQ(options.size(), 1);

    auto* const option = dynamic_cast<ObjectOption*>(options.front().get());
    ASSERT_TRUE(option);
    EXPECT_TRUE(option->inverse());
}

TEST(Parser_FilterOption, XmlHttpRequest_Normal)
{
    auto const& line = "adblock$xmlhttprequest"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& options = filter->options();
    ASSERT_EQ(options.size(), 1);

    auto* const option = dynamic_cast<XmlHttpRequestOption*>(options.front().get());
    ASSERT_TRUE(option);
    EXPECT_FALSE(option->inverse());
}

TEST(Parser_FilterOption, XmlHttpRequest_Inverse)
{
    auto const& line = "adblock$~xmlhttprequest"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& options = filter->options();
    ASSERT_EQ(options.size(), 1);

    auto* const option = dynamic_cast<XmlHttpRequestOption*>(options.front().get());
    ASSERT_TRUE(option);
    EXPECT_TRUE(option->inverse());
}

TEST(Parser_FilterOption, ObjectSubRequest_Normal)
{
    auto const& line = "adblock$object-subrequest"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& options = filter->options();
    ASSERT_EQ(options.size(), 1);

    auto* const option = dynamic_cast<ObjectSubRequestOption*>(options.front().get());
    ASSERT_TRUE(option);
    EXPECT_FALSE(option->inverse());
}

TEST(Parser_FilterOption, ObjectSubRequest_Inverse)
{
    auto const& line = "adblock$~object-subrequest"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& options = filter->options();
    ASSERT_EQ(options.size(), 1);

    auto* const option = dynamic_cast<ObjectSubRequestOption*>(options.front().get());
    ASSERT_TRUE(option);
    EXPECT_TRUE(option->inverse());
}

TEST(Parser_FilterOption, SubDocument_Normal)
{
    auto const& line = "adblock$subdocument"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& options = filter->options();
    ASSERT_EQ(options.size(), 1);

    auto* const option = dynamic_cast<SubDocumentOption*>(options.front().get());
    ASSERT_TRUE(option);
    EXPECT_FALSE(option->inverse());
}

TEST(Parser_FilterOption, SubDocument_Inverse)
{
    auto const& line = "adblock$~subdocument"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& options = filter->options();
    ASSERT_EQ(options.size(), 1);

    auto* const option = dynamic_cast<SubDocumentOption*>(options.front().get());
    ASSERT_TRUE(option);
    EXPECT_TRUE(option->inverse());
}

TEST(Parser_FilterOption, Other_Normal)
{
    auto const& line = "adblock$other"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& options = filter->options();
    ASSERT_EQ(options.size(), 1);

    auto* const option = dynamic_cast<OtherOption*>(options.front().get());
    ASSERT_TRUE(option);
    EXPECT_FALSE(option->inverse());
}

TEST(Parser_FilterOption, Other_Inverse)
{
    auto const& line = "adblock$~other"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& options = filter->options();
    ASSERT_EQ(options.size(), 1);

    auto* const option = dynamic_cast<OtherOption*>(options.front().get());
    ASSERT_TRUE(option);
    EXPECT_TRUE(option->inverse());
}

TEST(Parser_FilterOption, ThirdParty_Normal)
{
    auto const& line = "adblock$third-party"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& options = filter->options();
    ASSERT_EQ(options.size(), 1);

    auto* const option = dynamic_cast<ThirdPartyOption*>(options.front().get());
    ASSERT_TRUE(option);
    EXPECT_FALSE(option->inverse());
}

TEST(Parser_FilterOption, ThirdParty_Inverse)
{
    auto const& line = "adblock$~third-party"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& options = filter->options();
    ASSERT_EQ(options.size(), 1);

    auto* const option = dynamic_cast<ThirdPartyOption*>(options.front().get());
    ASSERT_TRUE(option);
    EXPECT_TRUE(option->inverse());
}

TEST(Parser_FilterOption, MatchCase)
{
    auto const& line = "adblock$match-case"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& options = filter->options();
    ASSERT_EQ(options.size(), 1);

    auto* const option = dynamic_cast<MatchCaseOption*>(options.front().get());
    EXPECT_TRUE(option);
}

TEST(Parser_FilterOption, Collapse_Normal)
{
    auto const& line = "adblock$collapse"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& options = filter->options();
    ASSERT_EQ(options.size(), 1);

    auto* const option = dynamic_cast<CollapseOption*>(options.front().get());
    ASSERT_TRUE(option);
    EXPECT_FALSE(option->inverse());
}

TEST(Parser_FilterOption, Collapse_Inverse)
{
    auto const& line = "adblock$~collapse"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& options = filter->options();
    ASSERT_EQ(options.size(), 1);

    auto* const option = dynamic_cast<CollapseOption*>(options.front().get());
    ASSERT_TRUE(option);
    EXPECT_TRUE(option->inverse());
}

TEST(Parser_FilterOption, DoNotTrack)
{
    auto const& line = "adblock$donottrack"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& options = filter->options();
    ASSERT_EQ(options.size(), 1);

    auto* const option = dynamic_cast<DoNotTrackOption*>(options.front().get());
    EXPECT_TRUE(option);
}

TEST(Parser_FilterOption, PopUp)
{
    auto const& line = "adblock$popup"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& options = filter->options();
    ASSERT_EQ(options.size(), 1);

    auto* const option = dynamic_cast<PopUpOption*>(options.front().get());
    EXPECT_TRUE(option);
}

TEST(Parser_FilterOption, Media_Normal)
{
    auto const& line = "adblock$media"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& options = filter->options();
    ASSERT_EQ(options.size(), 1);

    auto* const option = dynamic_cast<MediaOption*>(options.front().get());
    ASSERT_TRUE(option);
    EXPECT_FALSE(option->inverse());
}

TEST(Parser_FilterOption, Media_Inverse)
{
    auto const& line = "adblock$~media"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& options = filter->options();
    ASSERT_EQ(options.size(), 1);

    auto* const option = dynamic_cast<MediaOption*>(options.front().get());
    ASSERT_TRUE(option);
    EXPECT_TRUE(option->inverse());
}

TEST(Parser_FilterOption, Font_Normal)
{
    auto const& line = "adblock$font"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& options = filter->options();
    ASSERT_EQ(options.size(), 1);

    auto* const option = dynamic_cast<FontOption*>(options.front().get());
    ASSERT_TRUE(option);
    EXPECT_FALSE(option->inverse());
}

TEST(Parser_FilterOption, Font_Inverse)
{
    auto const& line = "adblock$~font"_r;

    auto const rule = parser::parse(line);
    ASSERT_TRUE(rule);

    auto const filter = dynamic_cast<FilterRule*>(rule.get());
    ASSERT_TRUE(filter);

    auto const& options = filter->options();
    ASSERT_EQ(options.size(), 1);

    auto* const option = dynamic_cast<FontOption*>(options.front().get());
    ASSERT_TRUE(option);
    EXPECT_TRUE(option->inverse());
}
