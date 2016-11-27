#ifndef ADBLOCK_OPTION_STYLE_SHEET_OPTION_HPP
#define ADBLOCK_OPTION_STYLE_SHEET_OPTION_HPP

#include "type.hpp"
#include "invertible_option.hpp"
#include "type_option.hpp"

namespace adblock {

class Context;

class StyleSheetOption : public InvertibleOption, public TypeOption
{
    using Base = InvertibleOption;
public:
    StyleSheetOption(const bool inverse) : Base { inverse } {}

private:
    bool doMatch2(const Uri&, const Context&) const override;
};

} // namespace adblock

#endif // ADBLOCK_OPTION_STYLE_SHEET_OPTION_HPP
