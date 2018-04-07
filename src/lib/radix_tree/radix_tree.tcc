#include "radix_tree.hpp"

#include <cassert>
#include <map>
#include <ostream>
#include <string>

#include <boost/format.hpp>

namespace radix_tree {

template<typename K, typename V>
inline void RadixTree<K, V>::
insert(const Key &key, const Value &value)
{
    if (key.empty()) {
        m_root.appendValue(value);
    }
    else {
        auto &node = m_root.appendChild(m_factory, key);
        node.appendValue(value);
    }
}

template<typename K, typename V>
inline void RadixTree<K, V>::
clear()
{
    m_root.clear();
    m_factory.clear();
}

template<typename K, typename V>
inline size_t RadixTree<K, V>::
node_count() const
{
    auto result = 0u;
    m_root.traverse(
        [&result](const NodeType&, const size_t) {
            ++result;
        }
    );
    return result;
}

template<typename K, typename V>
inline size_t RadixTree<K, V>::
value_count() const
{
    auto result = 0u;
    m_root.traverse(
        [&result](const NodeType &node, const size_t) {
            if (node.hasValue()) {
                result += node.values().size();
            }
        }
    );
    return result;
}

template<typename K, typename V>
template<typename Visitor>
inline void RadixTree<K, V>::
traverse(const Key &query, Visitor &&visit) const
{
    if (m_root.hasValue()) {
        visit(m_root, m_root.key());
    }

    auto *node = &m_root;
    auto key = query;

    while (!key.empty()) {
        const auto child = node->findPrefixChild(key);
        if (child) {
            const auto &childKey = child->key();
            if (visit(*child, childKey)) return;

            key = Key { key.begin() + childKey.size(), key.end() };
            node = child;
        }
        else {
            return;
        }
    }
}

template<typename K, typename V>
inline boost::property_tree::ptree RadixTree<K, V>::
statistics() const
{
    size_t numBranch = 0, numLeaf = 0;
    std::map<size_t, size_t> childCounts;
    std::map<size_t, size_t> valueCounts;
    std::map<size_t, size_t> branchsByLevel;
    std::map<size_t, size_t> leavesByLevel;

    m_root.traverse(
        [&](const NodeType &node, const size_t level) {
            if (node.hasChild()) {
                ++numBranch;
                ++childCounts[node.childCount()];
                ++branchsByLevel[level];
            }
            else {
                ++numLeaf;
                ++leavesByLevel[level];
            }
            ++valueCounts[node.values().size()];
        }
    );

    namespace bpt = boost::property_tree;
    bpt::ptree result;

    result.put("Number of leaf", numLeaf);
    result.put("Number of branch", numBranch);
    result.put("Number of nodes", node_count());
    result.put("Number of values", value_count());

    bpt::ptree child;
    for (const auto &item: childCounts) {
        child.put(std::to_string(item.first), item.second);
    }
    result.put_child("Branches by children", child);

    child.clear();
    for (const auto &item: branchsByLevel) {
        child.put(std::to_string(item.first), item.second);
    }
    result.put_child("Branches by level", child);

    child.clear();
    for (const auto &item: leavesByLevel) {
        child.put(std::to_string(item.first), item.second);
    }
    result.put_child("Leaves by level", child);

    child.clear();
    for (const auto &item: valueCounts) {
        child.put(std::to_string(item.first), item.second);
    }
    result.put_child("Nodes by values", child);

    return result;
}

} // namespace radix_tree
