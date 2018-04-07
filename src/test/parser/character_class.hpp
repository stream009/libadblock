#ifndef CHARACTER_CLASS_HPP
#define CHARACTER_CLASS_HPP

#include <limits>

#include <boost/range/algorithm.hpp>

namespace character_class {

std::string lower()
{
    return "abcdefghijklmnopqrstuvwxyz";
}

std::string upper()
{
    return "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
}

std::string alpha()
{
    return lower() + upper();
}

std::string number()
{
    return "0123456789";
}

std::string blank()
{
    return "\t ";
}

std::string space()
{
    return "\n\v\f\r\t ";
}

std::string cntrl()
{
    std::string result;
    for (char c = 000; c <= 037; ++c) {
        result.push_back(c);
    }
    result.push_back(0177);

    return result;
}

std::string punct()
{
    return R"(!"#$%&'()*+,-./:;<=>?@[\]^_`{|}~)";
}

std::string alnum()
{
    return alpha() + number();
}

std::string graph()
{
    return alnum() + punct();
}

std::string print()
{
    return alnum() + punct() + space();
}

std::string all()
{
    std::string result;
    for (char c = std::numeric_limits<char>::min();
         c != std::numeric_limits<char>::max();
         ++c)
    {
        result.push_back(c);
    }
    return result;
}

std::string utf8()
{
    std::string ascii = print() + cntrl();
    std::string all_ = all();
    std::string result;
    boost::sort(all_); boost::sort(ascii);
    boost::set_difference(all_, ascii, std::back_inserter(result));

    return result;
}

} // namespace character_class

#endif // CHARACTER_CLASS_HPP
