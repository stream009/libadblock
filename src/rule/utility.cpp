#include "utility.hpp"

#include "namespace.hpp"

#include <stream9/strings/ends_with.hpp>

namespace adblock {

bool
matchDomain(StringRange const host,
            std::vector<StringRange> const* const domains)
{
    if (!domains) return true;

    bool result = false;
    bool hasIncludeDomain = false;
    bool hasExcludeDomain = false;

    for (auto& dom: *domains) {
        // [[asserts: !d.empty()]]

        if (dom[0] == '~') {
            hasExcludeDomain = true;
            StringRange const d { dom.begin() + 1, dom.end() };
            if (str::ends_with(host, d)) {
                return false;
            }
        }
        else {
            hasIncludeDomain = true;
            if (str::ends_with(host, dom)) {
                result = true;
            }
        }
    }

    if (!hasIncludeDomain && hasExcludeDomain) {
        return true;
    }
    else {
        return result;
    }
}

} // namespace adblock
