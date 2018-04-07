#include "node.hpp"

namespace trie {

template<typename Key, typename Value>
std::locale Node<Key, Value>::m_locale {};

template<typename Key, typename Value>
const Node<Key, Value> *Node<Key, Value>::
findChild(const Key &key) const
{
    if (!m_children) return nullptr;

    const auto lowerKey = std::tolower(key, m_locale);

    const auto it = m_children->find(lowerKey);
    if (it == m_children->end()) {
        return nullptr;
    }
    else {
        return &(it->second);
    }
}

template<typename Key, typename Value>
Node<Key, Value> *Node<Key, Value>::
findChild(const Key &key)
{
    return const_cast<Node*>(
        const_cast<const Node*>(this)->findChild(key)
    );
}

template<typename Key, typename Value>
Node<Key, Value> *Node<Key, Value>::
appendChild(const Key &key)
{
    if (!m_children) {
        m_children.reset(new Children);
    }

    const auto lowerKey = std::tolower(key, m_locale);

    const auto it = m_children->emplace(lowerKey, Node {});
    if (it.second) {
        auto &newNode = it.first->second;
        //newNode.m_parent = this;
        return &newNode;
    }
    else {
        return nullptr;
    }
}

template<typename Key, typename Value>
void Node<Key, Value>::
appendValue(const Value &value)
{
    if (!m_values) {
        m_values.reset(new Values);
    }
    m_values->push_back(value);
}

template<typename Key, typename Value>
bool Node<Key, Value>::
hasChild() const
{
    return static_cast<bool>(m_children);
}

template<typename Key, typename Value>
bool Node<Key, Value>::
hasValue() const
{
    return static_cast<bool>(m_values);
}

template<typename K, typename V>
typename Node<K, V>::ValuesRange Node<K, V>::
values() const
{
    if (!m_values) {
        return {};
    }
    else {
        return *m_values;
    }
}

template<typename K, typename V>
size_t Node<K, V>::
childCount() const
{
    return m_children ? m_children->size() : 0;
}

template<typename Key, typename Value>
template<typename Visitor>
void Node<Key, Value>::
traverse(Visitor &&visit) const
{
    traverse(visit, Key {}, 0);
}

template<typename Key, typename Value>
template<typename Visitor>
void Node<Key, Value>::
traverse(Visitor &&visit, const Key &key, const size_t level) const
{
    // Pre-order traversal algorithm
    visit(*this, key, level);
    if (!m_children) return;

    for (auto &item: *m_children) {
        auto &child = item.second;

        child.traverse(visit, item.first, level + 1);
    }
}

} // namespace trie
