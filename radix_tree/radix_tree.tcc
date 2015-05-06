#include "radix_tree.hpp"

#include <cassert>
#include <ostream>

#include <boost/format.hpp>

namespace radix_tree {

template<typename Key, typename Value>
inline void RadixTree<Key, Value>::
insert(const Key &key, const Value &value)
{
    auto* const node = m_root.appendChild(m_factory, key);
    assert(node);
    node->appendValue(value);
}

template<typename Key, typename Value>
inline size_t RadixTree<Key, Value>::
node_count() const
{
    auto result = 0u;
    m_root.traverse(
        [&result](const NodeType&,
                  const typename NodeType::Key&, const size_t)
        {
            ++result;
        }
    );
    return result;
}

template<typename Key, typename Value>
inline size_t RadixTree<Key, Value>::
value_count() const
{
    auto result = 0u;
    m_root.traverse(
        [&result](const NodeType &node,
                  const typename NodeType::Key&, const size_t)
        {
            if (node.hasValue()) {
                result += node.values().size();
            }
        }
    );
    return result;
}

template<typename Key, typename Value>
template<typename Visitor>
inline void RadixTree<Key, Value>::
traverse(const Key &query, Visitor &&visit) const
{
    auto *node = &m_root;
    auto key = query;

    while (!key.empty()) {
        const auto rv = node->findChild(key);
        if (rv) {
            const auto &childKey = rv->first;
            const auto *child = rv->second;
            if (visit(*child, childKey)) return;

            key = Key { key.begin() + childKey.size(), key.end() };
            node = child;
        }
        else {
            return;
        }
    }
}

template<typename Key, typename Value>
inline void RadixTree<Key, Value>::
statistics(std::ostream &os) const
{
    size_t numBranch = 0, numLeaf = 0;
    std::map<size_t, size_t> childCounts;
    std::map<size_t, size_t> valueCounts;
    std::map<size_t, size_t> branchsByLevel;
    std::map<size_t, size_t> leavesByLevel;

    m_root.traverse(
        [&](const NodeType &node, const typename NodeType::Key&,
                                  const size_t level)
        {
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

    boost::format fmt { "%21s: %6s\n" };
    os << fmt % "Number of leaf" % numLeaf
       << fmt % "Number of branch" % numBranch
       << fmt % "Total number of nodes" % node_count();

    os << fmt % "Branches by child" % "";
    boost::format childsFmt { "%2s child" };
    for (const auto &item: childCounts) {
        os << fmt % (childsFmt % item.first) % item.second;
    }

    os << fmt % "Branches by level" % "";
    boost::format levelFmt { "Level %2s" };
    for (const auto &item: branchsByLevel) {
        os << fmt % (levelFmt % item.first) % item.second;
    }

    os << fmt % "Leaves by level" % "";
    for (const auto &item: leavesByLevel) {
        os << fmt % (levelFmt % item.first) % item.second;
    }

    os << fmt % "Number of values" % value_count();

    boost::format valueFmt { "%2s value node" };
    for (const auto &item: valueCounts) {
        os << fmt % (valueFmt % item.first) % item.second;
    }
}

} // namespace radix_tree
