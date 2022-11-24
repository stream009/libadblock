#ifndef ADBLOCK_CORE_JSON_HPP
#define ADBLOCK_CORE_JSON_HPP

#include <exception>

#include <boost/lexical_cast.hpp>

#include <stream9/json.hpp>

namespace adblock {

namespace json { using namespace stream9::json; }

inline double
to_number(json::value const& v)
{
    if (v.is_int64()) {
        return static_cast<double>(v.get_int64());
    }
    else if (v.is_uint64()) {
        return static_cast<double>(v.get_int64());
    }
    else if (v.is_double()) {
        return v.get_double();
    }
    else if (v.is_string()) {
        return boost::lexical_cast<double>(v.get_string());
    }
    else {
        throw std::bad_cast(); //TODO different error
    }
}

} // namespace adblock

#endif // ADBLOCK_CORE_JSON_HPP
