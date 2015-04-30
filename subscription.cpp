#include "subscription.hpp"

#include "parser/parser.hpp"

namespace adblock {

using Rules = Subscription::Rules;
using RulesRange = Subscription::RulesRange;
using const_iterator = Subscription::const_iterator;

static Rules
parse(const char *buffer, const size_t size)
{
    const auto *begin = buffer, *end = buffer + size;

    // first line have to be version string
    const auto it = std::find(begin, end, '\n');
    assert(it != end);
    //TODO proper error handling
    assert(boost::equals(StringRange(begin, it), "[Adblock Plus 2.0]"));

    const StringRange bufferR { it + 1, end };
    const auto num = boost::count(bufferR, '\n');
    Rules results;
    results.reserve(num);

    for (auto &&lineIt = boost::make_split_iterator(
                      bufferR, first_finder("\n", boost::is_equal()));
         !lineIt.eof();
         ++lineIt)
    {
        const auto &line = *lineIt;
        if (line.empty()) continue;

        results.push_back(parser::parse(line));
    }

    return results;
}


Subscription::
Subscription(const Path &filePath)
    : m_filePath { filePath },
      m_file { m_filePath },
      m_rules { parse(m_file.data(), m_file.size()) }
{}

const_iterator Subscription::
begin() const
{
    return m_file.data();
}

const_iterator Subscription::
end() const
{
    return m_file.data() + m_file.size();
}

RulesRange Subscription::
rules() const
{
    return m_rules;
}

} // namespace adblock
