#ifndef ADBLOCK_DOMAIN_DATA_BASE_HPP
#define ADBLOCK_DOMAIN_DATA_BASE_HPP

#include "type.hpp"

namespace adblock {

class DomainDataBase
{
public:
    DomainDataBase();

    StringRange query(const Uri&) const;

private:
    static void *m_tree;
};

} // namespace adblock

#endif // ADBLOCK_DOMAIN_DATA_BASE_HPP
