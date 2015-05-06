#ifndef RADIX_TREE_HPP
#define RADIX_TREE_HPP

#include "node.hpp"
#include "node_factory.hpp"

#include <iosfwd>

namespace radix_tree {

template<typename K, typename V>
class RadixTree
{
public:
    using Key = K;
    using Value = V;
    using NodeType = Node<Key, Value>;

public:
    RadixTree() = default;

    void insert(const Key&, const Value&);

    size_t node_count() const;
    size_t value_count() const;

    template<typename Visitor>
        void traverse(const Key&, Visitor&&) const;

    void statistics(std::ostream&) const;

private:
    NodeType m_root;
    NodeFactory<NodeType> m_factory;
};

} // namespace radix_tree

#include "radix_tree.tcc"

#endif // RADIX_TREE_HPP

