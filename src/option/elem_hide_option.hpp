#ifndef ADBLOCK_OPTION_ELEM_HIDE_OPTION_HPP
#define ADBLOCK_OPTION_ELEM_HIDE_OPTION_HPP

#include "type.hpp"
#include "invertible_option.hpp"
#include "white_list_option.hpp"

namespace adblock {

class Context;

class ElemHideOption : public InvertibleOption, public WhiteListOption
{
    using Base = InvertibleOption;
public:
    ElemHideOption(const bool inverse) : Base { inverse } {}

private:
    bool doMatch2(Uri const&, Context const&) const override;
};

} // namespace adblock

#endif // ADBLOCK_OPTION_ELEM_HIDE_OPTION_HPP
