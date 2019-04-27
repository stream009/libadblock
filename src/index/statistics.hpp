#ifndef ADBLOCK_INDEX_STATISTICS_HPP
#define ADBLOCK_INDEX_STATISTICS_HPP

#include "core/json.hpp"

#include <boost/container/flat_map.hpp>

#include <radix_tree/radix_tree.hpp>

namespace adblock {

template<typename Key, typename Value>
json::object
radixTreeStatistics(radix_tree::tree<Key, Value> const& tree)
{
    size_t numBranch = 0, numLeaf = 0;
    boost::container::flat_map<size_t, size_t> childCounts;
    boost::container::flat_map<size_t, size_t> valueCounts;
    boost::container::flat_map<size_t, size_t> branchsByLevel;
    boost::container::flat_map<size_t, size_t> leavesByLevel;

    tree.traverse_all(
        [&](auto const& node, auto const level) {
            if (node.has_child()) {
                ++numBranch;
                ++childCounts[node.child_count()];
                ++branchsByLevel[level];
            }
            else {
                ++numLeaf;
                ++leavesByLevel[level];
            }
            ++valueCounts[node.values().size()];
        }
    );

    json::object result;

    result["Number of leaf"] = static_cast<double>(numLeaf);
    result["Number of branch"] = static_cast<double>(numBranch);
    result["Number of nodes"] = static_cast<double>(tree.node_count());
    result["Number of values"] = static_cast<double>(tree.value_count());

    json::object child;
    for (auto const& item: childCounts) {
        child[std::to_string(item.first)] = static_cast<double>(item.second);
    }
    result["Branches by children"] = child;

    child.clear();
    for (auto const& item: branchsByLevel) {
        child[std::to_string(item.first)] = static_cast<double>(item.second);
    }
    result["Branches by level"] = child;

    child.clear();
    for (auto const& item: leavesByLevel) {
        child[std::to_string(item.first)] = static_cast<double>(item.second);
    }
    result["Leaves by level"] = child;

    child.clear();
    for (auto const& item: valueCounts) {
        child[std::to_string(item.first)] = static_cast<double>(item.second);
    }
    result["Nodes by values"] = child;

    return result;
}

} // namespace adblock

#endif // ADBLOCK_INDEX_STATISTICS_HPP
