#ifndef ADBLOCK_PREFIX_MAP_HPP
#define ADBLOCK_PREFIX_MAP_HPP

#include <map>
#include <utility>

#include <boost/optional.hpp>
#include <boost/range/iterator_range.hpp>

namespace adblock {

template<typename Key, typename T>
class PrefixMap
{
    struct Compare {
        bool operator()(const Key &lhs, const Key &rhs) const;
    };
    using Map = std::map<Key, T, Compare>;
public:
    using IteratorRange = boost::iterator_range<typename Map::const_iterator>;

public:

    T &operator[](const Key &key) { return m_map[key]; }
    T &operator[](Key &&key) { return m_map[std::move(key)]; }

    boost::optional<IteratorRange>
        match(const Key &target) const;

private:
    Map m_map;
};

} // namespace adblock

#include "prefix_map.tcc"

#endif // ADBLOCK_PREFIX_MAP_HPP
