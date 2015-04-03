#include "begin_match_pattern.hpp"

#include <boost/algorithm/string/predicate.hpp>
#include <boost/range/iterator_range.hpp>

bool BeginMatchPattern::
doMatch(const Range &target, const TokenRange &tokens) const
{
    namespace ba = boost::algorithm;

    static const Compare &compare {};

    const auto &firstToken = tokens.front();
    if (!ba::starts_with(target, firstToken, compare)) {
        return false;
    }

    if (tokens.size() == 1) return true;

    assert(target.size() >= firstToken.size());
    const auto &remaining = boost::make_iterator_range(
                       target.begin() + firstToken.size(), target.end());

    const auto &remainingTokens = boost::make_iterator_range(
                                  tokens.begin() + 1, tokens.end());

    return Base::doMatch(remaining, remainingTokens);
}
