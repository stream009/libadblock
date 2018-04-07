#ifndef RADIX_TREE_NODE_FACTORY_HPP
#define RADIX_TREE_NODE_FACTORY_HPP

#include <cassert>
#include <memory>
#include <vector>

#include <boost/make_unique.hpp>

namespace radix_tree {

template<typename NodeT>
class NodeFactory
{
public:
    NodeT &newNode(const typename NodeT::Key &key)
    {
        m_store.push_back(boost::make_unique<NodeT>(key));
        assert(m_store.back());
        assert(m_store.back().get());
        return *m_store.back();
    }

    void clear()
    {
        m_store.clear();
    }

private:
    std::vector<std::unique_ptr<NodeT>> m_store;
};

} // namespace radix_tree

#endif // RADIX_TREE_NODE_FACTORY_HPP
