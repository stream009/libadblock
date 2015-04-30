#ifndef ADBLOCK_SUFFIX_MAP_HPP
#define ADBLOCK_SUFFIX_MAP_HPP

#include <map>
#include <utility>

#include <boost/optional.hpp>
#include <boost/range/iterator_range.hpp>

namespace adblock {

template<typename Key, typename T>
class SuffixMap
{
    struct Compare {
        bool operator()(const Key &lhs, const Key &rhs) const;
    };

    using Map = std::map<Key, T, Compare>;
public:
    using Iterator = typename Map::const_iterator;
    using IteratorRange = boost::iterator_range<typename Map::const_iterator>;

public:

    // accessor
    T &operator[](const Key &key) { return m_map[key]; }
    T &operator[](const Key &&key) { return m_map[std::move(key)]; }

    Iterator begin() const { return m_map.cbegin(); }
    Iterator end() const { return m_map.cend(); }

    // query
    boost::optional<IteratorRange>
        match(const Key &target) const;

private:
    Map m_map;
};

} // namespace adblock

#include "suffix_map.tcc"

#endif // ADBLOCK_SUFFIX_MAP_HPP
