#include "end_match_pattern.hpp"

#include <boost/algorithm/string/predicate.hpp>
#include <boost/range/iterator_range.hpp>

namespace adblock {

bool EndMatchPattern::
doMatch(const UriRange &target, const TokenRange &tokens) const
{
    namespace ba = boost::algorithm;

    static const Compare &compare {};

    const auto &lastToken = tokens.back();
    if (!ba::ends_with(target, lastToken, compare)) {
        return false;
    }

    if (tokens.size() == 1) return true;

    assert(target.size() >= lastToken.size());
    const auto &remaining = boost::make_iterator_range(
                       target.begin(), target.end() - lastToken.size());

    const auto &remainingTokens = boost::make_iterator_range(
                                  tokens.begin(), tokens.end() - 1);

    return Base::doMatch(remaining, remainingTokens);
}

} // namespace adblock
