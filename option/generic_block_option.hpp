#ifndef ADBLOCK_OPTION_GENERIC_BLOCK_OPTION_HPP
#define ADBLOCK_OPTION_GENERIC_BLOCK_OPTION_HPP

#include "type.hpp"
#include "option.hpp"
#include "type_option.hpp"

namespace adblock {

class Context;

class GenericBlockOption : public Option, public TypeOption
{
private:
    // @override Option
    bool doMatch(Uri const&, Context const&) const override
    {
        return true;
    }
};

} // namespace adblock

#endif // ADBLOCK_OPTION_GENERIC_BLOCK_OPTION_HPP
