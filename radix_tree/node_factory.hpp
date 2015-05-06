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
    NodeT *newNode()
    {
        m_store.push_back(boost::make_unique<NodeT>());
        assert(m_store.back());
        assert(m_store.back().get());
        return m_store.back().get();
    }

private:
    std::vector<std::unique_ptr<NodeT>> m_store;
};

} // namespace radix_tree

#endif // RADIX_TREE_NODE_FACTORY_HPP
