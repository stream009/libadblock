#include "type.hpp"

#include <vector>

namespace adblock {

bool matchDomain(StringRange host, std::vector<StringRange> const* domains);

} // namespace adblock
