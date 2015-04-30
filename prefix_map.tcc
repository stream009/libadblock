#include "prefix_map.hpp"

#include <algorithm>

#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm.hpp>

namespace adblock {

template<typename Key, typename T>
bool PrefixMap<Key, T>::Compare::
operator()(const Key &lhs, const Key &rhs) const
{
    // Compare string lexicographicaly.
    // Return true when the rhs is prefix of ths lhs.

    auto        lIt = lhs.begin(),  rIt = rhs.begin();
    const auto lEnd = lhs.end(),   rEnd = rhs.end();
    for (; (lIt != lEnd) && (rIt != rEnd); ++lIt, ++rIt) {
        static boost::algorithm::is_iless iLess;
        if (iLess(*lIt, *rIt)) return true;
        if (iLess(*rIt, *lIt)) return false;
    }

    //return (rIt == rEnd) && (lIt != lEnd);
    return (lIt == lEnd) && (rIt != rEnd);
}

template<typename Key, typename T>
boost::optional<typename PrefixMap<Key, T>::IteratorRange> PrefixMap<Key, T>::
match(const Key &target) const
{
    using Value = typename Map::value_type;

    static const auto &lowerPred =
        [](const Value &lhs, const StringRange &rhs) {
            namespace ba = boost::algorithm;
            std::cout << lhs.first << " < " << rhs << "\t\t";
            if (ba::istarts_with(rhs, lhs.first)) {
                std::cout << " : starts_with\n";
                return false;
            }
            else {
                static Compare compare;
                const auto rv = compare(lhs.first, rhs);
                std::cout << " : " << rv << "\n";
                return rv;
            }
        };

    static const auto &upperPred =
        [](const StringRange &lhs, const Value &rhs) {
            namespace ba = boost::algorithm;
            std::cout << lhs << " < " << rhs.first << "\t\t";
            if (ba::istarts_with(lhs, rhs.first)) {
                std::cout << " : starts_with\n";
                return false;
            }
            else {
                //return ba::ilexicographical_compare(lhs, rhs.first);
                static Compare compare;
                const auto rv = compare(lhs, rhs.first);
                std::cout << " : " << rv << "\n";
                return rv;
            }
        };

    std::cout << "----\n";
    for (const auto &item: m_map) {
        std::cout << item.first << "\n";
    }
    std::cout << "----\n";
    std::cout << "[lower_bound]\n";
    const auto &lower = boost::lower_bound(m_map, target, lowerPred);
    std::cout << "**" << lower->first << "\n";

    if (lower != m_map.end() && boost::istarts_with(target, lower->first)) {
        std::cout << "[upper_bound] " << lower->first << "\n";
        const auto &upper =
                std::upper_bound(lower, m_map.end(), target, upperPred);

        std::cout << "----\n";
        return boost::make_iterator_range(lower, upper);
    }
    std::cout << "----\n";

    return boost::none;
}

} // namespace adblock
