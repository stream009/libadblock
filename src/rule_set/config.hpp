#ifndef ADBLOCK_RULE_SET_CONFIG_HPP
#define ADBLOCK_RULE_SET_CONFIG_HPP

#include "type.hpp"

//#define USE_TRIE
#define USE_RADIX_TREE

#if defined(USE_TRIE)
#include "trie/trie.hpp"
#elif defined(USE_RADIX_TREE)
#include <radix_tree/radix_tree.hpp>
#else
#error "either USE_TRIE or USE_RADIX_TREE must be defined"
#endif

namespace adblock { namespace rule_set {

#if defined(USE_TRIE)
template<typename Key, typename Value>
using Rules = trie::Trie<Key, Value>;
#elif defined(USE_RADIX_TREE)
template<typename Key, typename Value>
using Rules = radix_tree::RadixTree<Key, Value>;
#else
#error "either USE_TRIE or USE_RADIX_TREE must be defined"
#endif

}} // namespace adblock::rule_set

#endif // ADBLOCK_RULE_SET_CONFIG_HPP
