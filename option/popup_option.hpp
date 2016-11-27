#ifndef ADBLOCK_OPTION_POPUP_OPTION_HPP
#define ADBLOCK_OPTION_POPUP_OPTION_HPP

#include "type.hpp"
#include "option.hpp"
#include "type_option.hpp"

namespace adblock {

class Context;

class PopUpOption : public Option, public TypeOption
{
private:
    bool doMatch(const Uri&, const Context&) const override;
};

} // namespace adblock

#endif // ADBLOCK_OPTION_POPUP_OPTION_HPP
