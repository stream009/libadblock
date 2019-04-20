#ifndef ADBLOCK_DOMAIN_DATA_BASE_HPP
#define ADBLOCK_DOMAIN_DATA_BASE_HPP

namespace adblock {

class StringRange;
class Uri;

class DomainDataBase
{
public:
    DomainDataBase();

    StringRange query(Uri const&) const;

private:
    static void* m_tree;
};

} // namespace adblock

#endif // ADBLOCK_DOMAIN_DATA_BASE_HPP
