#include "node.hpp"

#include <boost/range/algorithm.hpp>
#include <boost/algorithm/string.hpp>

namespace radix_tree {

template<typename K, typename V, typename T>
inline boost::optional<const typename Node<K, V, T>::Children::value_type>
Node<K, V, T>::
findChild(const Key &key) const
{
    namespace ba = boost::algorithm;
    namespace br = boost::range;

    if (!m_children) return boost::none;

    const auto it = br::find_if(*m_children, //TODO linear?
        [&key](const std::pair<Key, Node*> &item) {
            const auto &itemKey = item.first;
            return ba::starts_with(key, itemKey, CharEquals {});
        }
    );
    if (it == m_children->end()) {
        return boost::none;
    }
    else {
        return *it;
    }
}

template<typename K, typename V, typename T>
inline boost::optional<typename Node<K, V, T>::Children::value_type>
Node<K, V, T>::
findChild(const Key &key)
{
    const auto rv = const_cast<const Node*>(this)->findChild(key);
    if (rv) {
        return typename Children::value_type {
                        rv->first, const_cast<Node*>(rv->second) };
    }
    else {
        return rv;
    }

}

template<typename Key, typename Compare>
inline size_t
getPrefixLength(const Key &lhs, const Key &rhs, const Compare &isEquals)
{
    auto it1 = lhs.begin(), it2 = rhs.begin();
    const auto end1 = lhs.end(), end2 = rhs.end();
    auto result = 0u;
    for (; it1 != end1 && it2 != end2; ++result, ++it1, ++it2) {
        if (!isEquals(*it1, *it2)) break;
    }

    return result;
}

template<typename K, typename V, typename T>
inline Node<K, V, T> *Node<K, V, T>::
appendChild(NodeFactory<Node> &factory, const Key &key)
{
    if (!m_children) {
        m_children.reset(new Children);
    }
    assert(m_children);

    const CharEquals isEquals {};

    auto it = findPrefixChild(key, isEquals);
    if (it == m_children->end()) {
        // Can't find prefix in children, so create a new child.
        auto *newChild = factory.newNode();
        assert(newChild);
        const auto rv = m_children->emplace(key, newChild);
        assert(rv.second);
        assert(rv.first->second == newChild);
        return newChild;
    }
    else {
        const auto &childKey = it->first;
        auto *child = it->second;

        namespace ba = boost::algorithm;
        if (ba::equals(key, childKey, isEquals)) { // duplicate key
            return child;
        }

        const auto prefixLen = getPrefixLength(key, childKey, isEquals);

        // Child's key is the prefix of the appending key
        // [before] [after appending "abcd"]
        //  abc      abc - b
        if (prefixLen == childKey.size()) {
            const Key suffix { key.begin() + prefixLen, key.end() };
            return child->appendChild(factory, suffix);
        }
        // Appending key is the prefix of the child's key
        // [before] [after appending "ab"]
        //  abc      ab - c
        else if (prefixLen == key.size()) {
            const Key suffix {
                          childKey.begin() + prefixLen, childKey.end() };
            auto *newChild = factory.newNode();
            assert(newChild);

            newChild->m_children.reset(new Children);
            auto rv = newChild->m_children->emplace(suffix, child);
            assert(rv.second);

            m_children->erase(it);
            rv = m_children->emplace(key, newChild);
            assert(rv.second);
            assert(rv.first->second == newChild);

            return newChild;
        }
        // Appending key and child's key have common prefix
        // [before] [after appending "abd"]
        //  abc      ab-+--c
        //              +--d
        else {
            assert(key.size() > prefixLen && childKey.size() > prefixLen);

            // split the child into a new branch and its child
            const auto suffixIt = childKey.begin() + prefixLen;
            const Key prefix { childKey.begin(), suffixIt },
                      suffix { suffixIt, childKey.end() };

            auto *newBranch = factory.newNode();
            newBranch->m_children.reset(new Children);
            auto rv = newBranch->m_children->emplace(suffix, child);
            assert(rv.second);
            assert(rv.first->second == child);

            m_children->erase(it);

            rv = m_children->emplace(prefix, newBranch);
            assert(rv.second);
            assert(rv.first->second == newBranch);

            // create a new node and append to the new branch
            const Key keySuffix { key.begin() + prefixLen, key.end() };
            auto *newChild = factory.newNode();
            rv = newBranch->m_children->emplace(keySuffix, newChild);
            assert(rv.second);
            assert(rv.first->second == newChild);

            return newChild;
        }
    }
}

template<typename K, typename V, typename T>
inline void Node<K, V, T>::
appendValue(const Value &value)
{
    if (!m_values) {
        m_values.reset(new Values);
    }
    m_values->push_back(value);
}

template<typename K, typename V, typename T>
inline bool Node<K, V, T>::
hasChild() const
{
    return static_cast<bool>(m_children);
}

template<typename K, typename V, typename T>
inline bool Node<K, V, T>::
hasValue() const
{
    return static_cast<bool>(m_values);
}

template<typename K, typename V, typename T>
inline typename Node<K, V, T>::ValuesRange Node<K, V, T>::
values() const
{
    if (!m_values) {
        return {};
    }
    else {
        return *m_values;
    }
}

template<typename K, typename V, typename T>
inline size_t Node<K, V, T>::
childCount() const
{
    return m_children ? m_children->size() : 0;
}

template<typename K, typename V, typename T>
template<typename Visitor>
inline void Node<K, V, T>::
traverse(Visitor &&visit) const
{
    traverse(visit, Key {}, 0);
}

template<typename K, typename V, typename T>
template<typename Visitor>
inline void Node<K, V, T>::
traverse(Visitor &&visit, const Key &key, const size_t level) const
{
    // Pre-order traversal algorithm
    visit(*this, key, level);
    if (!m_children) return;

    for (auto &item: *m_children) {
        auto *child = item.second;
        assert(child);

        child->traverse(visit, item.first, level + 1);
    }
}

template<typename K, typename V, typename T>
inline typename Node<K, V, T>::Children::iterator Node<K, V, T>::
findPrefixChild(const Key &key, const CharEquals &comp) const
{
    namespace ba = boost::algorithm;

    // Binary search by first charactor
    const Key firstChar { key.begin(), key.begin() + 1 };
    const auto it = m_children->lower_bound(firstChar);

    if (it == m_children->end() ||
            !ba::starts_with(it->first, firstChar, comp))
    {
        return m_children->end();
    }
    else {
        // There has to be only one prefix item.
        auto next = std::next(it);
        assert(!(next != m_children->end()
            && ba::starts_with(next->first, firstChar, comp)));

        return it;
    }
}

} // namespace radix_tree
