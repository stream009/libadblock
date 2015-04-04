#ifndef RULE_HPP
#define RULE_HPP

#include <string>

#include <boost/network/uri/uri.hpp>
#include <boost/noncopyable.hpp>
#include <boost/range/iterator_range.hpp>

class Rule : boost::noncopyable
{
public:
    using String = std::string;
    using StringRange = boost::iterator_range<String::const_iterator>;
    using Uri = boost::network::uri::uri;

public:
    virtual ~Rule() = default;
};

#include <utility>

#endif // RULE_HPP
