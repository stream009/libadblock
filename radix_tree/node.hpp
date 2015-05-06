#ifndef RADIX_TREE_NODE_HPP
#define RADIX_TREE_NODE_HPP

#include "node_factory.hpp"

#include <locale>
#include <memory>
#include <map>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/optional.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/container/flat_map.hpp>
#include <boost/intrusive/set.hpp>

namespace radix_tree {

template<typename Key>
struct CaseInsensitiveNodeTraits
{
    struct Compare {
        bool operator()(const Key &lhs, const Key &rhs) const {
            namespace ba = boost::algorithm;
            return ba::ilexicographical_compare(lhs, rhs);
        }
    };
    using CharEquals = boost::algorithm::is_iequal;
};

template<typename K, typename V, typename T = CaseInsensitiveNodeTraits<K>>
class Node
{
public:
    using Key = K;
    using Value = V;
    using Compare = typename T::Compare;
    using CharEquals = typename T::CharEquals;
    //using Children = std::map<Key, Node*, Compare>;
    using Children = boost::container::flat_map<Key, Node*, Compare>;
    using Values = std::vector<Value>;
    using ValuesRange =
        boost::iterator_range<typename Values::const_iterator>;

    Node() = default;

    boost::optional<const typename Children::value_type>
        findChild(const Key&) const;
    boost::optional<typename Children::value_type>
        findChild(const Key&);

    Node *appendChild(NodeFactory<Node>&, const Key&);
    void appendValue(const Value&);

    bool hasChild() const;
    bool hasValue() const;

    ValuesRange values() const;
    size_t childCount() const;

    template<typename Visitor>
        void traverse(Visitor&&) const;

private:
    template<typename Visitor>
        void traverse(Visitor&&, const Key&, const size_t level) const;

    typename Children::iterator
        findPrefixChild(const Key&, const CharEquals&) const;

private:
    std::unique_ptr<Children> m_children;
    std::unique_ptr<Values> m_values;
};

} // namespace radix_tree

#include "node.tcc"

#endif // RADIX_TREE_NODE_HPP
