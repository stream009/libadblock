#include "trie.hpp"

#include <gtest/gtest.h>

using namespace trie;

TEST(Trie, Elementary)
{
    Trie<std::string, size_t> trie;

    trie.insert("ab", 0);

    EXPECT_EQ(3, trie.node_count());
}

TEST(Trie, Traverse)
{
    using TrieT = Trie<std::string, size_t>;
    TrieT trie;
    using NodeType = TrieT::NodeType;

    trie.insert("abc", 0);

    std::string path;
    trie.traverse("abcd",
        [&path](const NodeType&, const NodeType::Key &key) {
            path += key;
            return false;
        }
    );
    EXPECT_EQ("abc", path);
}

TEST(Trie, CaseInsensitive)
{
    using TrieT = Trie<std::string, size_t>;
    TrieT trie;
    using NodeType = TrieT::NodeType;

    trie.insert("abc", 0);

    std::string path;
    trie.traverse("ABC",
        [&path](const NodeType&, const NodeType::Key &key) {
            path += key;
            return false;
        }
    );
    EXPECT_EQ("ABC", path);
}

TEST(Trie, InsertMore)
{
    using TrieT = Trie<std::string, size_t>;
    TrieT trie;

    EXPECT_EQ(1, trie.node_count());
    EXPECT_EQ(0, trie.value_count());

    trie.insert("abc", 0);
    EXPECT_EQ(4, trie.node_count());
    EXPECT_EQ(1, trie.value_count());

    trie.insert("abd", 1);
    EXPECT_EQ(5, trie.node_count());
    EXPECT_EQ(2, trie.value_count());

    trie.insert("cba", 2);
    EXPECT_EQ(8, trie.node_count());
    EXPECT_EQ(3, trie.value_count());

    trie.insert("ABC", 4);
    EXPECT_EQ(8, trie.node_count());
    EXPECT_EQ(4, trie.value_count());
}

#include <boost/range/iterator_range.hpp>
using StringRange = boost::iterator_range<const char*>;

inline StringRange
operator"" _r(const char *str, const size_t len)
{
    return { str, str + len };
}

TEST(Trie, IteratorRange)
{
    using Trie = trie::Trie<StringRange, size_t>;

    Trie trie;
    trie.insert("abcdefg"_r, 0);
    EXPECT_EQ(8, trie.node_count());
}
