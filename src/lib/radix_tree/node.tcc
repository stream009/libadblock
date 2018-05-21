#include "node.hpp"

#include <boost/range/algorithm.hpp>
#include <boost/algorithm/string.hpp>

namespace radix_tree {

template<typename K, typename V, typename T>
inline Node<K, V, T>::
Node(const Key &key)
    : m_key { key }
{}

template<typename K, typename V, typename T>
inline Node<K, V, T>::
~Node()
{
    clear();
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
inline Node<K, V, T> *Node<K, V, T>::
findPrefixChild(const Key &key) const
{
    namespace ba = boost::algorithm;

    if (!m_children) return nullptr;

    const auto it = findPartialPrefixChild(key);
    if (it != m_children->end() &&
                          ba::starts_with(key, it->key(), m_isEquals))
    {
        return &(*it);
    }
    else {
        return nullptr;
    }
}

template<typename K, typename V, typename T>
inline Node<K, V, T> *Node<K, V, T>::
findPrefixChild(const Key &key)
{
    return const_cast<const Node *>(this)->findChild(key);
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
    traverse(visit, 0);
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
inline Node<K, V, T> &Node<K, V, T>::
appendChild(NodeFactory<Node> &factory, const Key &key)
{
    assert(!key.empty());

    if (!m_children) {
        m_children.reset(new Children);
    }
    assert(m_children);

    auto it = findPartialPrefixChild(key);
    if (it == m_children->end()) {
        // Can't find prefix in children, so create a new child.
        auto &newChild = factory.newNode(key);
        const auto rv = m_children->insert(newChild);
        assert(rv.second); (void)rv;
        assert(&(*rv.first) == &newChild);
        return newChild;
    }
    else {
        const auto &childKey = it->key();
        auto &child = *it;

        const auto prefixLen = getPrefixLength(key, childKey, m_isEquals);
        assert(prefixLen > 0);

        // Duplicate key
        if (key.size() == childKey.size() && key.size() == prefixLen) {
            return child;
        }
        // Child's key is the prefix of the appending key
        // [before] [after appending "abcd"]
        //  abc      abc - b
        else if (prefixLen == childKey.size()) {
            const Key suffix { key.begin() + prefixLen, key.end() };
            return child.appendChild(factory, suffix);
        }
        // Appending key is the prefix of the child's key
        // [before] [after appending "ab"]
        //  abc      ab - c
        else if (prefixLen == key.size()) {
            const Key suffix {
                          childKey.begin() + prefixLen, childKey.end() };
            assert(!suffix.empty());

            auto &newChild = factory.newNode(key);

            auto num = m_children->erase(child);
            assert(num == 1); (void)num;
            child.m_key = suffix;

            newChild.m_children.reset(new Children);
            auto rv = newChild.m_children->insert(child);
            assert(rv.second);

            rv = m_children->insert(newChild);
            assert(rv.second);
            assert(&(*rv.first) == &newChild);

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
            assert(!prefix.empty());
            assert(!suffix.empty());

            auto num = m_children->erase(child);
            assert(num == 1); (void)num;
            child.m_key = suffix;

            auto &newBranch = factory.newNode(prefix);
            newBranch.m_children.reset(new Children);
            auto rv = newBranch.m_children->insert(child);
            assert(rv.second);
            assert(&(*rv.first) == &child);

            rv = m_children->insert(newBranch);
            assert(rv.second);
            assert(&(*rv.first) == &newBranch);

            // create a new node and append to the new branch
            const Key keySuffix { key.begin() + prefixLen, key.end() };
            auto &newChild = factory.newNode(keySuffix);
            rv = newBranch.m_children->insert(newChild);
            assert(rv.second);
            assert(&(*rv.first) == &newChild);

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
inline void Node<K, V, T>::
clear()
{
    if (m_children) {
        for (auto &child: *m_children) {
            if (child.hasChild()) {
                child.clear();
            }
        }

        m_children->clear();
        m_children.reset(nullptr);
    }

    m_values.reset(nullptr);
}

template<typename K, typename V, typename T>
template<typename Visitor>
inline void Node<K, V, T>::
traverse(Visitor &&visit, const size_t level) const
{
    // Pre-order traversal algorithm
    visit(*this, level);
    if (!m_children) return;

    for (const auto &child: *m_children) {
        child.traverse(visit, level + 1);
    }
}

template<typename K, typename V, typename T>
inline typename Node<K, V, T>::Children::iterator Node<K, V, T>::
findPartialPrefixChild(const Key &key) const
{
    assert(!key.empty());

    // Binary search by first charactor
    const auto it = m_children->lower_bound(key,
        [] (const Node &node, const Key &key) {
            const auto &nodeKey = node.key();

            return m_charCompare(nodeKey.front(), key.front());
        }
    );

    if (it == m_children->end() ||
            !m_isEquals(it->key().front(), key.front()))
    {
        return m_children->end();
    }
    else {
        // There has to be only one prefix item.
        auto next = std::next(it);
        assert(!(next != m_children->end()
            && m_isEquals(next->key().front(), key.front()))); (void)next;

        return it;
    }
}

template<typename K, typename V, typename T>
const typename Node<K, V, T>::CharEquals Node<K, V, T>::m_isEquals {};

template<typename K, typename V, typename T>
const typename Node<K, V, T>::CharCompare Node<K, V, T>::m_charCompare {};

} // namespace radix_tree
