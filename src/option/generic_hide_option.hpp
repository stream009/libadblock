#ifndef ADBLOCK_OPTION_GENERIC_HIDE_OPTION_HPP
#define ADBLOCK_OPTION_GENERIC_HIDE_OPTION_HPP

#include "type.hpp"
#include "option.hpp"
#include "white_list_option.hpp"

namespace adblock {

class Context;

class GenericHideOption : public Option, public WhiteListOption
{
private:
    // @override Option
    bool doMatch(Uri const&, Context const&) const override;
};

} // namespace adblock

#endif // ADBLOCK_OPTION_GENERIC_HIDE_OPTION_HPP
