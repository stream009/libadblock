#ifndef RADIX_TREE_HPP
#define RADIX_TREE_HPP

#include "node.hpp"
#include "node_factory.hpp"

#include <iosfwd>
#include <type_traits>

#include <boost/property_tree/ptree.hpp>

namespace radix_tree {

template<typename K, typename V>
class RadixTree
{
public:
    using Key = K;
    using Value = V;
    using NodeType = Node<Key, Value>;

public:
    // modifier
    void insert(const Key&, const Value&);
    void clear();

    // query
    size_t node_count() const;
    size_t value_count() const;

    template<typename Visitor>
        void traverse(const Key&, Visitor&&) const;

    boost::property_tree::ptree statistics() const;

    void validate() const
    {
        m_root.traverse(
            [](const NodeType &node, const size_t) {
                node.validate();
            }
        );
    }

private:
    NodeFactory<NodeType> m_factory;
    NodeType m_root;
};

} // namespace radix_tree

#include "radix_tree.tcc"

#endif // RADIX_TREE_HPP

