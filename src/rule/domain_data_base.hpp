#ifndef ADBLOCK_DOMAIN_DATA_BASE_HPP
#define ADBLOCK_DOMAIN_DATA_BASE_HPP

namespace adblock {

class StringRange;
class Uri;

namespace domain_db {

StringRange getRegisteredDomain(Uri const&);


} // namespace domain_db

} // namespace adblock

#endif // ADBLOCK_DOMAIN_DATA_BASE_HPP
