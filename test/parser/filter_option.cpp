#include "parser/filter_option.hpp"
#include "option.hpp"

#include <memory>

#include <boost/lexical_cast.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/spirit/include/qi.hpp>

#include <gtest/gtest.h>

using namespace adblock;
namespace qi = boost::spirit::qi;

const static parser::FilterOption grammar;

TEST(FilterOptionParser, Script)
{
    {   // normal
        const auto &line = boost::as_literal("script");
        std::shared_ptr<Option> result;
        auto it = line.begin();
        const auto rv = qi::parse(it, line.end(), grammar, result);

        ASSERT_TRUE(rv && it == line.end());
        const auto &option = std::dynamic_pointer_cast<ScriptOption>(result);
        EXPECT_TRUE(!!option);
        EXPECT_FALSE(option->inverse());
    }
    {   // inverse
        const auto &line = boost::as_literal("~script");
        std::shared_ptr<Option> result;
        auto it = line.begin();
        const auto rv = qi::parse(it, line.end(), grammar, result);

        ASSERT_TRUE(rv && it == line.end());
        const auto &option = std::dynamic_pointer_cast<ScriptOption>(result);
        EXPECT_TRUE(!!option);
        EXPECT_TRUE(option->inverse());
    }
}

TEST(FilterOptionParser, Image)
{
    {   // normal
        const auto &line = boost::as_literal("image");
        std::shared_ptr<Option> result;
        auto it = line.begin();
        const auto rv = qi::parse(it, line.end(), grammar, result);

        ASSERT_TRUE(rv && it == line.end());
        const auto &option = std::dynamic_pointer_cast<ImageOption>(result);
        EXPECT_TRUE(!!option);
        EXPECT_FALSE(option->inverse());
    }
    {   // inverse
        const auto &line = boost::as_literal("~image");
        std::shared_ptr<Option> result;
        auto it = line.begin();
        const auto rv = qi::parse(it, line.end(), grammar, result);

        ASSERT_TRUE(rv && it == line.end());
        const auto &option = std::dynamic_pointer_cast<ImageOption>(result);
        EXPECT_TRUE(!!option);
        EXPECT_TRUE(option->inverse());
    }
}

TEST(FilterOptionParser, StyleSheet)
{
    {   // normal
        const auto &line = boost::as_literal("stylesheet");
        std::shared_ptr<Option> result;
        auto it = line.begin();
        const auto rv = qi::parse(it, line.end(), grammar, result);

        ASSERT_TRUE(rv && it == line.end());
        const auto &option = std::dynamic_pointer_cast<StyleSheetOption>(result);
        EXPECT_TRUE(!!option);
        EXPECT_FALSE(option->inverse());
    }
    {   // inverse
        const auto &line = boost::as_literal("~stylesheet");
        std::shared_ptr<Option> result;
        auto it = line.begin();
        const auto rv = qi::parse(it, line.end(), grammar, result);

        ASSERT_TRUE(rv && it == line.end());
        const auto &option = std::dynamic_pointer_cast<StyleSheetOption>(result);
        EXPECT_TRUE(!!option);
        EXPECT_TRUE(option->inverse());
    }
}

TEST(FilterOptionParser, Object)
{
    {   // normal
        const auto &line = boost::as_literal("object");
        std::shared_ptr<Option> result;
        auto it = line.begin();
        const auto rv = qi::parse(it, line.end(), grammar, result);

        ASSERT_TRUE(rv && it == line.end());
        const auto &option = std::dynamic_pointer_cast<ObjectOption>(result);
        EXPECT_TRUE(!!option);
        EXPECT_FALSE(option->inverse());
    }
    {   // inverse
        const auto &line = boost::as_literal("~object");
        std::shared_ptr<Option> result;
        auto it = line.begin();
        const auto rv = qi::parse(it, line.end(), grammar, result);

        ASSERT_TRUE(rv && it == line.end());
        const auto &option = std::dynamic_pointer_cast<ObjectOption>(result);
        EXPECT_TRUE(!!option);
        EXPECT_TRUE(option->inverse());
    }
}

TEST(FilterOptionParser, XMLHttpRequest)
{
    {   // normal
        const auto &line = boost::as_literal("xmlhttprequest");
        std::shared_ptr<Option> result;
        auto it = line.begin();
        const auto rv = qi::parse(it, line.end(), grammar, result);

        ASSERT_TRUE(rv && it == line.end());
        const auto &option = std::dynamic_pointer_cast<XmlHttpRequestOption>(result);
        EXPECT_TRUE(!!option);
        EXPECT_FALSE(option->inverse());
    }
    {   // inverse
        const auto &line = boost::as_literal("~xmlhttprequest");
        std::shared_ptr<Option> result;
        auto it = line.begin();
        const auto rv = qi::parse(it, line.end(), grammar, result);

        ASSERT_TRUE(rv && it == line.end());
        const auto &option = std::dynamic_pointer_cast<XmlHttpRequestOption>(result);
        EXPECT_TRUE(!!option);
        EXPECT_TRUE(option->inverse());
    }
}

TEST(FilterOptionParser, ObjectSubRequest)
{
    {   // normal
        const auto &line = boost::as_literal("object-subrequest");
        std::shared_ptr<Option> result;
        auto it = line.begin();
        const auto rv = qi::parse(it, line.end(), grammar, result);

        ASSERT_TRUE(rv && it == line.end());
        const auto &option = std::dynamic_pointer_cast<ObjectSubRequestOption>(result);
        EXPECT_TRUE(!!option);
        EXPECT_FALSE(option->inverse());
    }
    {   // inverse
        const auto &line = boost::as_literal("~object-subrequest");
        std::shared_ptr<Option> result;
        auto it = line.begin();
        const auto rv = qi::parse(it, line.end(), grammar, result);

        ASSERT_TRUE(rv && it == line.end());
        const auto &option = std::dynamic_pointer_cast<ObjectSubRequestOption>(result);
        EXPECT_TRUE(!!option);
        EXPECT_TRUE(option->inverse());
    }
}

TEST(FilterOptionParser, SubDocument)
{
    {   // normal
        const auto &line = boost::as_literal("subdocument");
        std::shared_ptr<Option> result;
        auto it = line.begin();
        const auto rv = qi::parse(it, line.end(), grammar, result);

        ASSERT_TRUE(rv && it == line.end());
        const auto &option = std::dynamic_pointer_cast<SubDocumentOption>(result);
        EXPECT_TRUE(!!option);
        EXPECT_FALSE(option->inverse());
    }
    {   // inverse
        const auto &line = boost::as_literal("~subdocument");
        std::shared_ptr<Option> result;
        auto it = line.begin();
        const auto rv = qi::parse(it, line.end(), grammar, result);

        ASSERT_TRUE(rv && it == line.end());
        const auto &option = std::dynamic_pointer_cast<SubDocumentOption>(result);
        EXPECT_TRUE(!!option);
        EXPECT_TRUE(option->inverse());
    }
}

TEST(FilterOptionParser, Other)
{
    {   // normal
        const auto &line = boost::as_literal("other");
        std::shared_ptr<Option> result;
        auto it = line.begin();
        const auto rv = qi::parse(it, line.end(), grammar, result);

        ASSERT_TRUE(rv && it == line.end());
        const auto &option = std::dynamic_pointer_cast<OtherOption>(result);
        EXPECT_TRUE(!!option);
        EXPECT_FALSE(option->inverse());
    }
    {   // inverse
        const auto &line = boost::as_literal("~other");
        std::shared_ptr<Option> result;
        auto it = line.begin();
        const auto rv = qi::parse(it, line.end(), grammar, result);

        ASSERT_TRUE(rv && it == line.end());
        const auto &option = std::dynamic_pointer_cast<OtherOption>(result);
        EXPECT_TRUE(!!option);
        EXPECT_TRUE(option->inverse());
    }
}

TEST(FilterOptionParser, ThirdParty)
{
    {   // normal
        const auto &line = boost::as_literal("third-party");
        std::shared_ptr<Option> result;
        auto it = line.begin();
        const auto rv = qi::parse(it, line.end(), grammar, result);

        ASSERT_TRUE(rv && it == line.end());
        const auto &option = std::dynamic_pointer_cast<ThirdPartyOption>(result);
        EXPECT_TRUE(!!option);
        EXPECT_FALSE(option->inverse());
    }
    {   // inverse
        const auto &line = boost::as_literal("~third-party");
        std::shared_ptr<Option> result;
        auto it = line.begin();
        const auto rv = qi::parse(it, line.end(), grammar, result);

        ASSERT_TRUE(rv && it == line.end());
        const auto &option = std::dynamic_pointer_cast<ThirdPartyOption>(result);
        EXPECT_TRUE(!!option);
        EXPECT_TRUE(option->inverse());
    }
}

TEST(FilterOptionParser, MatchCase)
{
    const auto &line = boost::as_literal("match-case");
    std::shared_ptr<Option> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    ASSERT_TRUE(rv && it == line.end());
    const auto &option = std::dynamic_pointer_cast<MatchCaseOption>(result);
    EXPECT_TRUE(!!option);
}

TEST(FilterOptionParser, Collapse)
{
    {   // normal
        const auto &line = boost::as_literal("collapse");
        std::shared_ptr<Option> result;
        auto it = line.begin();
        const auto rv = qi::parse(it, line.end(), grammar, result);

        ASSERT_TRUE(rv && it == line.end());
        const auto &option = std::dynamic_pointer_cast<CollapseOption>(result);
        EXPECT_TRUE(!!option);
        EXPECT_FALSE(option->inverse());
    }
    {   // inverse
        const auto &line = boost::as_literal("~collapse");
        std::shared_ptr<Option> result;
        auto it = line.begin();
        const auto rv = qi::parse(it, line.end(), grammar, result);

        ASSERT_TRUE(rv && it == line.end());
        const auto &option = std::dynamic_pointer_cast<CollapseOption>(result);
        EXPECT_TRUE(!!option);
        EXPECT_TRUE(option->inverse());
    }
}

TEST(FilterOptionParser, DoNotTrack)
{
    const auto &line = boost::as_literal("donottrack");
    std::shared_ptr<Option> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    ASSERT_TRUE(rv && it == line.end());
    const auto &option = std::dynamic_pointer_cast<DoNotTrackOption>(result);
    EXPECT_TRUE(!!option);
}

TEST(FilterOptionParser, PopUp)
{
    const auto &line = boost::as_literal("popup");
    std::shared_ptr<Option> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    ASSERT_TRUE(rv && it == line.end());
    const auto &option = std::dynamic_pointer_cast<PopUpOption>(result);
    EXPECT_TRUE(!!option);
}

TEST(FilterOptionParser, Media)
{
    {   // normal
        const auto &line = boost::as_literal("media");
        std::shared_ptr<Option> result;
        auto it = line.begin();
        const auto rv = qi::parse(it, line.end(), grammar, result);

        ASSERT_TRUE(rv && it == line.end());
        const auto &option = std::dynamic_pointer_cast<MediaOption>(result);
        EXPECT_TRUE(!!option);
        EXPECT_FALSE(option->inverse());
    }
    {   // inverse
        const auto &line = boost::as_literal("~media");
        std::shared_ptr<Option> result;
        auto it = line.begin();
        const auto rv = qi::parse(it, line.end(), grammar, result);

        ASSERT_TRUE(rv && it == line.end());
        const auto &option = std::dynamic_pointer_cast<MediaOption>(result);
        EXPECT_TRUE(!!option);
        EXPECT_TRUE(option->inverse());
    }
}
