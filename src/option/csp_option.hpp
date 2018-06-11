#ifndef ADBLOCK_OPTION_CSP_OPTION_HPP
#define ADBLOCK_OPTION_CSP_OPTION_HPP

#include "option.hpp"
#include "type.hpp"
#include "type_option.hpp"

namespace adblock {

class Context;

class CspOption : public Option, public TypeOption
{
public:
    CspOption(StringRange const policy)
        : m_policy { policy }
    {}

    StringRange policy() const { return m_policy; }

private:
    bool doMatch(Uri const&, Context const&) const override;

    void validate()
    {
        assert(!m_policy.empty());
    }

private:
    StringRange m_policy;
};

} // namespace adblock

#endif // ADBLOCK_OPTION_CSP_OPTION_HPP
