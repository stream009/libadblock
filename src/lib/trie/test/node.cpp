#include "trie.hpp"
#include "node.hpp"

#include <gtest/gtest.h>

TEST(Node, Elementary)
{
    using Node = trie::Node<char, size_t>;
    Node node;

    EXPECT_FALSE(node.hasChild());
    EXPECT_FALSE(node.hasValue());
}

TEST(Node, Children)
{
    using Node = trie::Node<char, size_t>;
    Node node;
    EXPECT_FALSE(node.hasChild());
    //EXPECT_EQ(nullptr, node->parent());

    auto *child = node.appendChild('A');
    ASSERT_NE(nullptr, child);
    //EXPECT_EQ(node, child->parent());

    EXPECT_TRUE(node.hasChild());

    child = node.appendChild('B');
    ASSERT_NE(nullptr, child);
    //EXPECT_EQ(node, child->parent());

    EXPECT_FALSE(child->hasChild());

    auto *gChild = child->appendChild('C');
    ASSERT_NE(nullptr, gChild);
    //EXPECT_EQ(child, gChild->parent());
}

using Node = trie::Node<char, size_t>;
void buildTree(Node &root)
{
    // root-a-b-c-d-h
    //      |
    //      e-f
    //      |
    //      g
    auto *a = root.appendChild('a');
    auto *b = a->appendChild('b');
    auto *c = b->appendChild('c');
    auto *d = c->appendChild('d');
    auto *e = root.appendChild('e');
    /*auto *f =*/e->appendChild('f');
    /*auto *g =*/root.appendChild('g');
    /*auto *h =*/d->appendChild('h');
}

TEST(Node, FindChild)
{
    Node root;
    buildTree(root);

    auto *a = root.findChild('A');
    EXPECT_NE(nullptr, a);
    auto *e = root.findChild('E');
    EXPECT_NE(nullptr, e);
    EXPECT_NE(nullptr, root.findChild('G'));
    auto *b = a->findChild('B');
    EXPECT_NE(nullptr, b);
    auto *c = b->findChild('C');
    EXPECT_NE(nullptr, c);
    auto *d = c->findChild('D');
    EXPECT_NE(nullptr, d);
    EXPECT_NE(nullptr, d->findChild('H'));
    EXPECT_NE(nullptr, e->findChild('F'));
}

TEST(Node, Value)
{
    Node node;
    ASSERT_TRUE(node.values().empty());

    node.appendValue(10);
    auto values = node.values();
    ASSERT_EQ(1, values.size());
    EXPECT_EQ(10, values.back());

    node.appendValue(20);
    values = node.values();
    EXPECT_EQ(2, values.size());
    EXPECT_EQ(20, values.back());
}

TEST(Node, Traverse)
{
    Node root;

    buildTree(root);

    size_t branch = 0, leaf = 0;
    root.traverse(
        [&branch, &leaf](const Node &node, const Node::Key&, const size_t) {
            if (node.hasChild()) {
                ++branch;
            }
            else {
                ++leaf;
            }
        }
    );
    EXPECT_EQ(6, branch);
    EXPECT_EQ(3, leaf);
}

TEST(Node, CaseInsensitive)
{
    {
        Node node;
        EXPECT_EQ(nullptr, node.findChild('a'));

        node.appendChild('A');
        EXPECT_NE(nullptr, node.findChild('a'));
        EXPECT_NE(nullptr, node.findChild('A'));
    }
    {
        Node node;
        EXPECT_EQ(nullptr, node.findChild('a'));

        node.appendChild('a');
        EXPECT_NE(nullptr, node.findChild('a'));
        EXPECT_NE(nullptr, node.findChild('A'));
    }
}
