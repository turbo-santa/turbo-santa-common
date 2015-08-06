#ifndef TURBO_SANTA_COMMON_BACK_END_DEBUGGER_BLOON_FILTER_H_
#define TURBO_SANTA_COMMON_BACK_END_DEBUGGER_BLOON_FILTER_H_

#include "backend/config.h"

#include <unordered_set>

namespace back_end {
namespace debugger {

class BloonFilter {
 public:
  bool AddAndCheckIfVisited(unsigned short address) {
    auto iter = filter_.find(address);
    filter_.insert(address);
    return filter_.end() != iter;
  }

  void Clear() { filter_.clear(); }
 private:
  std::unordered_set<unsigned short> filter_;
};

} // namespace debugger
} // namespace back_end

#endif // TURBO_SANTA_COMMON_BACK_END_DEBUGGER_BLOON_FILTER_H_
