#include "statistics.hpp"

namespace adblock {

template<typename Key, typename Value>
json::object
radixTreeStatistics(radix_tree::RadixTree<Key, Value> const& tree)
{
    size_t numBranch = 0, numLeaf = 0;
    boost::container::flat_map<size_t, size_t> childCounts;
    boost::container::flat_map<size_t, size_t> valueCounts;
    boost::container::flat_map<size_t, size_t> branchsByLevel;
    boost::container::flat_map<size_t, size_t> leavesByLevel;

    tree.traverse(
        [&](NodeType const& node, size_t const level) {
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

    json::object result;

    result["Number of leaf"] = numLeaf;
    result["Number of branch"] = numBranch;
    result["Number of nodes"] = tree.node_count();
    result["Number of values"] = tree.value_count();

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
