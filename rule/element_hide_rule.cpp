#include "element_hide_rule.hpp"

#include <boost/xpressive/regex_actions.hpp>
#include <boost/xpressive/xpressive.hpp>

namespace xp = boost::xpressive;

struct emplace_back_impl {
    using result_type = void;

    template<typename Seq, typename... Args>
    void operator()(Seq &seq, Args&&... args) const {
        seq.emplace_back(std::forward<Args>(args)...);
    }
};
static xp::function<emplace_back_impl>::type const emplace_back = {{}};

static const xp::sregex &
subdomain()
{
    namespace xp = boost::xpressive;

    static xp::sregex letter, digit, let_dig, let_dig_hyp,
                      ldh_str, label, subdomain;

    if (subdomain.regex_id() != 0) return subdomain;

    // Domain name specification from RFC 1035
    letter      = xp::alpha;
    digit       = xp::digit;
    let_dig     = letter | digit;
    let_dig_hyp = let_dig | "-";
    ldh_str     = +let_dig_hyp;
    label       = letter >> !(!ldh_str >> let_dig);
    subdomain   = label >> *("." >> label);
    // static const xp::sregex domain = subdomain | " ";

    return subdomain;
}

void ElementHideRule::
parseDomain(const StringRange &range)
{
    namespace xp = boost::xpressive;

    if (range.empty()) return;

    xp::sregex include_domain, exclude_domain, domain, domains;
    const auto &subdomain = ::subdomain();

    include_domain = subdomain
                     [ emplace_back(xp::ref(m_includeDomains),
                                    xp::first(xp::_), xp::second(xp::_)) ];
    exclude_domain = "~" >> subdomain
                     [ emplace_back(xp::ref(m_excludeDomains),
                                    xp::first(xp::_), xp::second(xp::_)) ];
    domain         = include_domain | exclude_domain;
    domains        = domain >> *(","  >> domain);

    xp::smatch what;
    if (!xp::regex_match(range, what, domains)) {
        assert(false && "wrong domain"); //TODO more
    }
}
