#include "domain_data_base.hpp"

#include <cassert>

extern "C" {
#include "reg_dom/regdom.h"
}

namespace adblock {

void *DomainDataBase::m_tree = nullptr;

DomainDataBase::
DomainDataBase()
{
    if (!m_tree) m_tree = ::loadTldTree();
    assert(m_tree);
}

StringRange DomainDataBase::
query(const Uri &uri) const
{
    if (!uri.is_valid()) return {};

    const auto &host = uri.host();
    if (host.empty()) return {};
    const char *begin = host.data();
    const auto len = host.size();
    const char *end = begin + len;

    const char *domain = ::getRegisteredDomain_(begin, len, m_tree);

    if (domain) {
        return { domain, end };
    }
    else {
        return {};
    }
}

} // namespace adblock
