#include "type.hpp"
#include "radix_tree/radix_tree.hpp"

#include <string>

#include <gtest/gtest.h>

using RadixTree = radix_tree::RadixTree<std::string, size_t>;

TEST(RadixTree, Elementary)
{
    RadixTree t;

    // root
    EXPECT_EQ(1, t.node_count());
    // root-(foo)
    t.insert("foo", 0);
    EXPECT_EQ(2, t.node_count());
    EXPECT_EQ(1, t.value_count());

    auto cnt = 0u;
    std::string childKey;
    t.traverse("foo",
        [&](const RadixTree::NodeType&, const RadixTree::Key &key) {
            ++cnt;
            childKey = key;
            return false;
        }
    );

    EXPECT_EQ(1, cnt);
    EXPECT_EQ("foo", childKey);
}

TEST(RadixTree, AppendSuffixNode)
{
    RadixTree t;

    // root-(abc)
    t.insert("abc", 0);
    // root-(abc)-(d)
    t.insert("abcd", 1);
    EXPECT_EQ(3, t.node_count());
    EXPECT_EQ(2, t.value_count());

    auto cnt = 0u;
    std::vector<std::string> childKeys;
    t.traverse("abcd",
        [&](const RadixTree::NodeType&, const RadixTree::Key &key) {
            ++cnt;
            childKeys.push_back(key);
            return false;
        }
    );

    EXPECT_EQ(2, cnt);
    EXPECT_EQ(2, childKeys.size());
    EXPECT_EQ("abc", childKeys[0]);
    EXPECT_EQ("d", childKeys[1]);
}

TEST(RadixTree, SplitNode)
{
    RadixTree t;

    // root-(abcd)
    t.insert("abcd", 1);
    EXPECT_EQ(2, t.node_count());
    EXPECT_EQ(1, t.value_count());

    // root-(abc)-(d)
    t.insert("abc", 0);
    EXPECT_EQ(3, t.node_count());
    EXPECT_EQ(2, t.value_count());

    auto cnt = 0u;
    std::vector<std::string> childKeys;
    t.traverse("abcd",
        [&](const RadixTree::NodeType&, const RadixTree::Key &key) {
            ++cnt;
            childKeys.push_back(key);
            return false;
        }
    );

    EXPECT_EQ(2, cnt);
    EXPECT_EQ(2, childKeys.size());
    EXPECT_EQ("abc", childKeys[0]);
    EXPECT_EQ("d", childKeys[1]);
}

TEST(RadixTree, SplitPrefixNode)
{
    // root
    RadixTree t;

    // root-(abcd)
    t.insert("abcd", 0);
    EXPECT_EQ(2, t.node_count());
    EXPECT_EQ(1, t.value_count());

    // root-(abc)-(d)
    t.insert("abc", 1);
    EXPECT_EQ(3, t.node_count());
    EXPECT_EQ(2, t.value_count());

    // root-a-+-(bc)-(d)
    //        +-(de)
    t.insert("ade", 2);
    EXPECT_EQ(5, t.node_count());
    EXPECT_EQ(3, t.value_count());

    {
        auto cnt = 0u;
        std::vector<std::string> childKeys;
        t.traverse("abcd",
            [&](const RadixTree::NodeType&, const RadixTree::Key &key) {
                ++cnt;
                childKeys.push_back(key);
                return false;
            }
        );

        EXPECT_EQ(3, cnt);
        EXPECT_EQ(3, childKeys.size());
        EXPECT_EQ("a", childKeys[0]);
        EXPECT_EQ("bc", childKeys[1]);
        EXPECT_EQ("d", childKeys[2]);
    }
    {
        auto cnt = 0u;
        std::vector<std::string> childKeys;
        t.traverse("ade",
            [&](const RadixTree::NodeType&, const RadixTree::Key &key) {
                ++cnt;
                childKeys.push_back(key);
                return false;
            }
        );

        EXPECT_EQ(2, cnt);
        EXPECT_EQ(2, childKeys.size());
        EXPECT_EQ("a", childKeys[0]);
        EXPECT_EQ("de", childKeys[1]);
    }
}

TEST(RadixTree, CaseInsensitive)
{
    {
        RadixTree t;

        t.insert("FOO", 0);

        auto cnt = 0u;
        std::string childKey;
        t.traverse("foo",
            [&](const RadixTree::NodeType&, const RadixTree::Key &key) {
                ++cnt;
                childKey = key;
                return false;
            }
        );

        ASSERT_EQ(1, cnt);
        EXPECT_EQ("FOO", childKey);
    }
    {
        RadixTree t;

        t.insert("foo", 0);

        auto cnt = 0u;
        std::string childKey;
        t.traverse("FOO",
            [&](const RadixTree::NodeType&, const RadixTree::Key &key) {
                ++cnt;
                childKey = key;
                return false;
            }
        );

        ASSERT_EQ(1, cnt);
        EXPECT_EQ("foo", childKey);
    }
    {
        RadixTree t;

        t.insert("foo", 0);
        t.insert("FOO", 1);

        auto cnt = 0u;
        std::string childKey;
        t.traverse("foo",
            [&](const RadixTree::NodeType&, const RadixTree::Key &key) {
                ++cnt;
                childKey = key;
                return false;
            }
        );

        ASSERT_EQ(1, cnt);
        EXPECT_EQ("foo", childKey);
    }
}

TEST(RadixTree, DuplicatedKey)
{
    RadixTree t;

    // root---(a)---b-+-(c)
    //                +-(d)
    t.insert("a", 0);
    t.insert("abc", 1);
    t.insert("abd", 2);
    EXPECT_EQ(5, t.node_count());
    EXPECT_EQ(3, t.value_count());

    // root--(a)--(b)-+-(c)
    //                +-(d)
    t.insert("ab", 3);
    EXPECT_EQ(5, t.node_count());
    EXPECT_EQ(4, t.value_count());
}

TEST(RadixTree, UseRangeAsKey)
{
    using namespace adblock;
    using RadixTreeR = radix_tree::RadixTree<StringRange, size_t>;

    // root
    RadixTreeR t;

    // root-(abcd)
    t.insert("abcd"_r, 0);
    EXPECT_EQ(2, t.node_count());
    EXPECT_EQ(1, t.value_count());

    // root-(abc)-(d)
    t.insert("abc"_r, 1);
    EXPECT_EQ(3, t.node_count());
    EXPECT_EQ(2, t.value_count());

    // root-a-+-(bc)-(d)
    //        +-(de)
    t.insert("ade"_r, 2);
    EXPECT_EQ(5, t.node_count());
    EXPECT_EQ(3, t.value_count());

    {
        auto cnt = 0u;
        std::vector<RadixTreeR::Key> childKeys;
        t.traverse("abcd"_r,
            [&](const RadixTreeR::NodeType&, const RadixTreeR::Key &key) {
                ++cnt;
                childKeys.push_back(key);
                return false;
            }
        );

        EXPECT_EQ(3, cnt);
        EXPECT_EQ(3, childKeys.size());
        EXPECT_EQ("a"_r, childKeys[0]);
        EXPECT_EQ("bc"_r, childKeys[1]);
        EXPECT_EQ("d"_r, childKeys[2]);
    }
    {
        auto cnt = 0u;
        std::vector<RadixTreeR::Key> childKeys;
        t.traverse("ade"_r,
            [&](const RadixTreeR::NodeType&, const RadixTreeR::Key &key) {
                ++cnt;
                childKeys.push_back(key);
                return false;
            }
        );

        EXPECT_EQ(2, cnt);
        EXPECT_EQ(2, childKeys.size());
        EXPECT_EQ("a"_r, childKeys[0]);
        EXPECT_EQ("de"_r, childKeys[1]);
    }
}
