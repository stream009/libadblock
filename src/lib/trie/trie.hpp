#ifndef TRIE_TRIE_HPP
#define TRIE_TRIE_HPP

#include "node.hpp"

#include <string>

namespace trie {

template<typename K, typename V>
class Trie
{
public:
    using Key = K;
    using Value = V;
    using Char = typename Key::value_type;
    using NodeType = Node<Char, Value>;

public:
    Trie();

    void insert(const Key&, const Value&);

    size_t node_count() const { return m_nodeCount; }
    size_t value_count() const { return m_valueCount; }

    NodeType &rootNode() { return m_root; }

    template<typename Visitor>
        void traverse(const Key&, Visitor&&) const;

    void statistics(std::ostream&) const;

private:
    NodeType m_root;
    size_t m_nodeCount = 1;
    size_t m_valueCount = 0;
};

} // namespace trie

#include "trie.tcc"

#endif // TRIE_TRIE_HPP
