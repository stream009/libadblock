#ifndef URL_API_ERROR_HPP
#define URL_API_ERROR_HPP

#include <adblock/database.hpp>

#include "core/json.hpp"
#include "core/uri.hpp"

#include <stream9/errors.hpp>

namespace adblock {

[[noreturn]] inline void
throw_url_parse_error(stream9::error const& e)
{
    auto ec = e.why();
    if (ec.category() == stream9::urls::parse_error()) {
        auto&& cxt = e.context();

        auto o_url = json::find_string(cxt, "url");
        auto o_location = json::find_int64(cxt, "location");

        if (o_url && o_location) {
            throw url_parse_error {
                *o_url,
                ec.message(),
                *o_location
            };
        }
    }

    stream9::rethrow_error(stream9::error_category());
}

} // namespace adblock

#endif // URL_API_ERROR_HPP
