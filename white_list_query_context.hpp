#ifndef ADBLOCK_WHITE_LIST_BLOCK_CONTEXT_HPP
#define ADBLOCK_WHITE_LIST_BLOCK_CONTEXT_HPP

#include "context.hpp"

namespace adblock {

class WhiteListQueryContext : public Context
{
public:
    virtual bool blockDisablerMode() const { return false; }
    virtual bool genericBlockDisablerMode() const { return false; }
    virtual bool hideDisablerMode() const { return false; }
    virtual bool genericHideDisablerMode() const { return false; }
};

} // namespace adblock

#endif // ADBLOCK_WHITE_LIST_BLOCK_CONTEXT_HPP
