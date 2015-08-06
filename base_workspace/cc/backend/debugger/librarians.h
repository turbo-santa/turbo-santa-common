#ifndef TURBO_SANTA_COMMON_BACK_END_DEBUGGER_LIBRARIANS_H_
#define TURBO_SANTA_COMMON_BACK_END_DEBUGGER_LIBRARIANS_H_

#include "backend/config.h"

#include <functional>
#include "backend/debugger/frames.h"
#include "backend/debugger/great_library.h"

namespace back_end {
namespace debugger {

typedef std::function<ConstFrameIterator(GreatLibrary*)> Librarian;

ConstFrameIterator MostRecentOldAddress(ConstFrameIterator iterator);
ConstFrameIterator MostRecentOldAddress(GreatLibrary* library);

ConstFrameIterator LastTimeExecuted(ConstFrameIterator iterator);

} // namespace debugger
} // namespace back_end

#endif // TURBO_SANTA_COMMON_BACK_END_DEBUGGER_LIBRARIANS_H_
