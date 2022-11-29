#ifndef ADBLOCK_CORE_JSON_HPP
#define ADBLOCK_CORE_JSON_HPP

#include "namespace.hpp"

#include <stream9/json.hpp>

namespace adblock {

namespace json { using namespace stream9::json; }

template<typename T = double>
inline T
to_number(json::value const& v)
{
    return v.to_number<T>();
}

} // namespace adblock

#endif // ADBLOCK_CORE_JSON_HPP
