#ifndef ADBLOCK_OPTION_INVERTIBLE_OPTION_HPP
#define ADBLOCK_OPTION_INVERTIBLE_OPTION_HPP

#include "option.hpp"
#include "type.hpp"

namespace adblock {

class Context;

class InvertibleOption : public Option
{
public:
    bool inverse() const { return m_inverse; }

protected:
    InvertibleOption(const bool inverse)
        : m_inverse { inverse }
    {}

private:
    // @override Option
    bool doMatch(Uri const&, Context const&) const override final;

    virtual bool doMatch2(Uri const&, Context const&) const;

private:
    bool m_inverse;
};

} // namespace adblock

#endif // ADBLOCK_OPTION_INVERTIBLE_OPTION_HPP
