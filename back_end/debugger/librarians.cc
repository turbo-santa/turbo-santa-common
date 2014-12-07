#include "back_end/debugger/librarians.h"

namespace back_end {
namespace debugger {

ConstFrameIterator MostRecentOldAddress(ConstFrameIterator iterator) {
  for (; !iterator->pc_delta().visited_before; iterator--) {}
  return iterator;
}

ConstFrameIterator MostRecentOldAddress(GreatLibrary* library) {
  return MostRecentOldAddress(library->end());
}

} // namespace debugger
} // namespace back_end
