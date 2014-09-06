#ifndef TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_PAGE_H_
#define TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_PAGE_H_

#include <memory>

namespace back_end {
namespace memory {

class MemoryPage {
    public:
        virtual void CopyTo(unsigned char* destination) {
            unsigned char* raw_memory = raw_memory_.get();
            for (int i = 0; i < length_; i++) {
                destination[i] = raw_memory[i];
            }
        }

        virtual void CopyFrom(unsigned char* source) {
            unsigned char* raw_memory = raw_memory_.get();
            for (int i = 0; i < length_; i++) {
                raw_memory[i] = source[i];
            }
        }

    private:
        std::unique_ptr<unsigned char> raw_memory_;
        long length_;
};

class PageManager {
    public:
        virtual void SelectPage(int page_number) {
            current_page_->CopyFrom(bank_);
            current_page_ = pages_[page_number].get();
            current_page_->CopyTo(bank_);
        }
        
    private:
        MemoryPage* current_page_;
        unsigned char* bank_;
        std::vector<std::unique_ptr<MemoryPage>> pages_;
};

} // namespace memory
} // namespace back_end

#endif // TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_PAGE_H_
