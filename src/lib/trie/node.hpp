#ifndef TRIE_NODE_HPP
#define TRIE_NODE_HPP

#include <locale>
#include <memory>
#include <unordered_map>
#include <vector>

#include <boost/range/iterator_range.hpp>

namespace trie {

template<typename K, typename V>
class Node
{
public:
    using Key = K;
    using Value = V;
    using Children = std::unordered_map<Key, Node>;
    using Values = std::vector<Value>;
    using ValuesRange =
        boost::iterator_range<typename Values::const_iterator>;

    Node() = default;

    const Node *findChild(const Key&) const;
          Node *findChild(const Key&);

    Node *appendChild(const Key&);
    void appendValue(const Value&);

    bool hasChild() const;
    bool hasValue() const;

    ValuesRange values() const;
    size_t childCount() const;

    template<typename Visitor>
        void traverse(Visitor&&) const;

    template<typename Visitor>
        void traverse(Visitor&&, const Key&, const size_t level) const;

private:
    //Node *m_parent = nullptr;
    std::unique_ptr<Children> m_children;
    std::unique_ptr<Values> m_values;

    static std::locale m_locale;
};

} // namespace trie

#include "node.tcc"

#endif // TRIE_NODE_HPP
