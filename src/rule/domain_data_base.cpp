#include "domain_data_base.hpp"

#include "core/string_range.hpp"
#include "core/uri.hpp"

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
getRegisteredDomain(Uri const& uri) const
{
    auto const& host = uri.host();
    if (host.empty()) return {};

    char const* begin = host.data();
    auto const len = host.size();
    char const* end = begin + len;

    char const* domain = ::getRegisteredDomain_(begin, len, m_tree);

    if (domain) {
        return { domain, end };
    }
    else {
        return {};
    }
}

} // namespace adblock
