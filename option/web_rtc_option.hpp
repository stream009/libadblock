#ifndef ADBLOCK_OPTION_WEB_RTC_OPTION_HPP
#define ADBLOCK_OPTION_WEB_RTC_OPTION_HPP

#include "invertible_option.hpp"
#include "type.hpp"
#include "type_option.hpp"

namespace adblock {

class Context;

class WebRtcOption : public InvertibleOption, public TypeOption
{
public:
    WebRtcOption(const bool inverse) : InvertibleOption { inverse } {}

private:
    bool doMatch2(Uri const&, Context const&) const override;
};


} // namespace adblock

#endif // ADBLOCK_OPTION_WEB_RTC_OPTION_HPP
