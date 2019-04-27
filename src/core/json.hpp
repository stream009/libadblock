#ifndef ADBLOCK_CORE_JSON_HPP
#define ADBLOCK_CORE_JSON_HPP

#include "json_fwd.hpp"

#include <exception>

#include <json/json.hpp>
#include <json/value.hpp>
#include <json/object.hpp>
#include <json/array.hpp>

#include <boost/lexical_cast.hpp>

namespace adblock {

inline double
to_number(json::value const& v)
{
    if (v.is_number()) {
        return v.get_number();
    }
    else if (v.is_string()) {
        return boost::lexical_cast<double>(v.get_string());
    }
    else {
        throw std::bad_cast();
    }
}

} // namespace adblock

#endif // ADBLOCK_CORE_JSON_HPP
