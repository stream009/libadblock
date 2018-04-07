#include "option.hpp"
#include "type.hpp"
#include "parser/white_list_option.hpp"
#include "option/document_option.hpp"
#include "option/elem_hide_option.hpp"

#include <memory>

#include <boost/range/algorithm.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/spirit/include/qi.hpp>

#include <gtest/gtest.h>

using namespace adblock;
namespace qi = boost::spirit::qi;

const static parser::WhiteListOption grammar;

TEST(WhiteListOptionParser, Document)
{
    {   // normal
        const auto &line = "document"_r;
        std::shared_ptr<Option> result;
        auto it = line.begin();
        const auto rv = qi::parse(it, line.end(), grammar, result);

        ASSERT_TRUE(rv && it == line.end());
        const auto &option = std::dynamic_pointer_cast<DocumentOption>(result);
        EXPECT_TRUE(!!option);
        EXPECT_FALSE(option->inverse());
    }
    {   // inverse
        const auto &line = "~document"_r;
        std::shared_ptr<Option> result;
        auto it = line.begin();
        const auto rv = qi::parse(it, line.end(), grammar, result);

        ASSERT_TRUE(rv && it == line.end());
        const auto &option = std::dynamic_pointer_cast<DocumentOption>(result);
        EXPECT_TRUE(!!option);
        EXPECT_TRUE(option->inverse());
    }
}

TEST(WhiteListOptionParser, ElemHide)
{
    {   // normal
        const auto &line = "elemhide"_r;
        std::shared_ptr<Option> result;
        auto it = line.begin();
        const auto rv = qi::parse(it, line.end(), grammar, result);

        ASSERT_TRUE(rv && it == line.end());
        const auto &option = std::dynamic_pointer_cast<ElemHideOption>(result);
        EXPECT_TRUE(!!option);
        EXPECT_FALSE(option->inverse());
    }
    {   // inverse
        const auto &line = "~elemhide"_r;
        std::shared_ptr<Option> result;
        auto it = line.begin();
        const auto rv = qi::parse(it, line.end(), grammar, result);

        ASSERT_TRUE(rv && it == line.end());
        const auto &option = std::dynamic_pointer_cast<ElemHideOption>(result);
        EXPECT_TRUE(!!option);
        EXPECT_TRUE(option->inverse());
    }
}
