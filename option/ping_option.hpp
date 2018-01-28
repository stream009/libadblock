#ifndef ADBLOCK_OPTION_PING_OPTION_HPP
#define ADBLOCK_OPTION_PING_OPTION_HPP

#include "invertible_option.hpp"
#include "type.hpp"
#include "type_option.hpp"

namespace adblock {

class Context;

class PingOption : public InvertibleOption, public TypeOption
{
public:
    PingOption(const bool inverse) : InvertibleOption { inverse } {}

private:
    bool doMatch2(Uri const&, Context const&) const override;
};

} // namespace adblock

#endif // ADBLOCK_OPTION_PING_OPTION_HPP
