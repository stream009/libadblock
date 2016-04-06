#ifndef RADIX_TREE_NODE_HPP
#define RADIX_TREE_NODE_HPP

#include "node_factory.hpp"

#include <memory>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/intrusive/set.hpp>

namespace radix_tree {

template<typename Key>
struct CaseInsensitiveNodeTraits
{
    using Char = typename Key::value_type;

    struct CharEquals {
        bool operator()(const Char lhs, const Char rhs) const {
            static const auto &ct =
                std::use_facet<std::ctype<Char>>(std::locale {});
            return ct.toupper(lhs) == ct.toupper(rhs);
        }
    };

    struct CharCompare {
        bool operator()(const Char lhs, const Char rhs) const {
            static const auto &ct =
                std::use_facet<std::ctype<Char>>(std::locale {});
            return ct.toupper(lhs) < ct.toupper(rhs);
        }
    };
};

namespace bi = boost::intrusive;

template<typename K, typename V, typename T = CaseInsensitiveNodeTraits<K>>
class Node : public bi::set_base_hook<bi::optimize_size<true>>
{
public:
    using Key = K;
    using Value = V;
    using CharEquals = typename T::CharEquals;
    using CharCompare = typename T::CharCompare;

    struct ChildCompare {
        bool operator()(const Node &lhs, const Node &rhs) const {
            static const CharCompare compare {};
            return compare(lhs.key().front(), rhs.key().front());
        }
    };
    using Children = bi::set<Node, bi::compare<ChildCompare>>;
    using Values = std::vector<Value>;
    using ValuesRange =
        boost::iterator_range<typename Values::const_iterator>;
public:
    // constructor / destructor
    explicit Node(const Key&);
    Node() = default;
    ~Node();

    // accessor
    const Key &key() const { return m_key; }

    ValuesRange values() const;

    // query
    Node *findPrefixChild(const Key&) const;
    Node *findPrefixChild(const Key&);

    bool hasChild() const;
    bool hasValue() const;
    size_t childCount() const;

    template<typename Visitor>
        void traverse(Visitor&&) const;

    void validate() const
    {
        assert(!m_key.empty());

        if (m_children) {
            auto curr = m_children->begin(), next = std::next(curr);
            const auto end = m_children->end();
            for (; next != end; ++curr, ++next) {
                assert(!m_isEquals(curr->key().front(), next->key().front()));
            }
        }
    }

    // modifier
    Node &appendChild(NodeFactory<Node>&, const Key&);
    void appendValue(const Value&);

    void clear();

private:
    template<typename Visitor>
        void traverse(Visitor&&, const size_t level) const;

    typename Children::iterator
        findPartialPrefixChild(const Key&) const;

private:
    Key m_key;
    std::unique_ptr<Children> m_children;
    std::unique_ptr<Values> m_values;

    static const CharEquals m_isEquals;
    static const CharCompare m_charCompare;
};

} // namespace radix_tree

#include "node.tcc"

#endif // RADIX_TREE_NODE_HPP
