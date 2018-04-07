#ifndef ADBLOCK_OPTION_THIRD_PARTY_OPTION_HPP
#define ADBLOCK_OPTION_THIRD_PARTY_OPTION_HPP

#include "type.hpp"
#include "invertible_option.hpp"
#include "restriction_option.hpp"

namespace adblock {

class ThirdPartyOption : public InvertibleOption, public RestrictionOption
{
    using Base = InvertibleOption;
public:
    ThirdPartyOption(const bool inverse) : Base { inverse } {}

private:
    bool doMatch2(const Uri&, const Context&) const override;
};

} // namespace adblock

#endif // ADBLOCK_OPTION_THIRD_PARTY_OPTION_HPP
