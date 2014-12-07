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

ConstFrameIterator LastTimeExecuted(ConstFrameIterator iterator) {
  unsigned short current_address = iterator->pc_delta().old_value;
  for (iterator--; current_address != iterator->pc_delta().old_value; iterator--) {}
  return iterator;
}

} // namespace debugger
} // namespace back_end
