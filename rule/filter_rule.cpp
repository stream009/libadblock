#include "filter_rule.hpp"

#include "pattern/basic_match_pattern.hpp"
#include "pattern/begin_match_pattern.hpp"
#include "pattern/domain_match_pattern.hpp"
#include "pattern/end_match_pattern.hpp"
#include "pattern/regex_pattern.hpp"

#include <boost/algorithm/string/predicate.hpp>

bool FilterRule::
match(const Uri &url, const Context&) const
{
    return m_pattern->match(url);
}

void FilterRule::
parse(const StringRange &range)
{
    namespace ba = boost::algorithm;

    if (ba::starts_with(range, "||")) {
        const StringRange &subRange { range.begin() + 2, range.end() };
        m_pattern.reset(new DomainMatchPattern { subRange });
    }
    else if (range.front() == '|') {
        const StringRange &subRange { range.begin() + 1, range.end() };
        m_pattern.reset(new BeginMatchPattern { subRange });
    }
    else if (range.back() == '|') {
        const StringRange &subRange { range.begin(), range.end() - 1 };
        m_pattern.reset(new EndMatchPattern { subRange });
    }
    else if (range.front() == '/' && range.back() == '/') {
        const StringRange &subRange {
                std::next(range.begin()), std::prev(range.end()) };
        m_pattern.reset(new RegexPattern { subRange });
    }
    else {
        m_pattern.reset(new BasicMatchPattern { range });
    }
}
