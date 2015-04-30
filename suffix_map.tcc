#include "suffix_map.hpp"

#include <algorithm>

#include <boost/algorithm/string.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <boost/range/algorithm.hpp>

namespace adblock {

template<typename Key, typename T>
bool SuffixMap<Key, T>::Compare::
operator()(const Key &lhs, const Key &rhs) const
{
    // Compare string lexicographicaly in back to front order.
    // Return true when the rhs is prefix of ths lhs.
    namespace bad = boost::adaptors;
    const auto &rlhs = bad::reverse(lhs);
    const auto &rrhs = bad::reverse(rhs);

    auto        lIt = rlhs.begin(),  rIt = rrhs.begin();
    const auto lEnd = rlhs.end(),   rEnd = rrhs.end();
    for (; (lIt != lEnd) && (rIt != rEnd); ++lIt, ++rIt) {
        if (*lIt < *rIt) return true;
        if (*rIt < *lIt) return false;
    }

    return (rIt == rEnd) && (lIt != lEnd);
}

template<typename Key, typename T>
boost::optional<typename SuffixMap<Key, T>::IteratorRange> SuffixMap<Key, T>::
match(const Key &target) const
{
    using Value = typename Map::value_type;
    namespace bad = boost::adaptors;

    static const auto &lowerPred =
        [](const Value &lhs, const Key &rhs) {
            namespace ba = boost::algorithm;
            if (ba::ends_with(rhs, lhs.first)) {
                return false;
            }
            else {
                static Compare compare;
                return compare(lhs.first, rhs);
            }
        };

    static const auto &upperPred =
        [](const Key &lhs, const Value &rhs) {
            namespace ba = boost::algorithm;
            if (ba::ends_with(lhs, rhs.first)) {
                return false;
            }
            else {
                static Compare compare;
                return compare(lhs, rhs.first);
            }
        };

    const auto &lower = boost::lower_bound(m_map, target, lowerPred);

    if (lower != m_map.end()
            && boost::ends_with(target, lower->first))
    {
        const auto &upper =
                std::upper_bound(lower, m_map.end(), target, upperPred);

        return boost::make_iterator_range(lower, upper);
    }

    return boost::none;
}

} // namespace adblock
