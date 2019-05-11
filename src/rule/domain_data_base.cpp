#include "domain_data_base.hpp"

#include "core/string_range.hpp"
#include "core/uri.hpp"

#include <cassert>

extern "C" {
#include "reg_dom/regdom.h"
}

namespace adblock {

class RegDom
{
public:
    RegDom()
    {
        m_tree = ::loadTldTree();
    }

    ~RegDom()
    {
        ::freeTldTree(m_tree);
    }

    char const* getRegisteredDomain(char const* begin, size_t len) const
    {
        return ::getRegisteredDomain_(begin, len, m_tree);
    }

private:
    void* m_tree;
};

static RegDom const&
db()
{
    static RegDom db;
    return db;
}

namespace domain_db {

StringRange
getRegisteredDomain(Uri const& uri)
{
    auto const& host = uri.host();
    if (host.empty()) return {};

    char const* begin = host.data();
    auto const len = host.size();
    char const* end = begin + len;

    char const* domain = db().getRegisteredDomain(begin, len);

    if (domain) {
        return { domain, end };
    }
    else {
        return {};
    }
}

} // namespace domain_db

} // namespace adblock
