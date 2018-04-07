#ifndef ADBLOCK_OPTION_COLLAPSE_OPTION_HPP
#define ADBLOCK_OPTION_COLLAPSE_OPTION_HPP

#include "invertible_option.hpp"

namespace adblock {

class CollapseOption : public InvertibleOption //TODO unimplemented
{
    using Base = InvertibleOption;
public:
    CollapseOption(const bool inverse) : Base { inverse } {}
};

} // namespace adblock

#endif // ADBLOCK_OPTION_COLLAPSE_OPTION_HPP
