#include "trie.hpp"

#include <cassert>
#include <map>

#include <boost/format.hpp>

namespace trie {

template<typename Key, typename Value>
Trie<Key, Value>::
Trie()
{}

template<typename Key, typename Value>
void Trie<Key, Value>::
insert(const Key &key, const Value &value)
{
    auto *node = &m_root;
    for (const auto &c: key) {

        auto* const child = node->findChild(c);
        if (!child) {
            auto* const newNode = node->appendChild(c);
            assert(newNode);

            ++m_nodeCount;
            node = newNode;
        }
        else {
            node = child;
        }
    }
    node->appendValue(value);
    ++m_valueCount;
}

template<typename Key, typename Value>
template<typename Visitor>
void Trie<Key, Value>::
traverse(const Key &key, Visitor &&visit) const
{
    auto *node = &m_root;
    for (const auto &c: key) {
        auto* const child = node->findChild(c);
        if (child) {
            if (visit(*child, c)) return;
            node = child;
        }
        else {
            return;
        }
    }
}

template<typename Key, typename Value>
void Trie<Key, Value>::
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

    boost::format fmt { "%20s: %6s\n" };
    os << fmt % "Number of leaf" % numLeaf
       << fmt % "Number of branch" % numBranch
       << fmt % "Total number of node" % m_nodeCount;

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

    os << fmt % "Number of values" % m_valueCount;

    boost::format valueFmt { "%2s value node" };
    for (const auto &item: valueCounts) {
        os << fmt % (valueFmt % item.first) % item.second;
    }
}

} // namespace trie
