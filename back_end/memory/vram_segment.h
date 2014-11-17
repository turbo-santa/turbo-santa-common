#ifndef TURBO_SANTA_COMMON_BACK_END_MEMORY_VRAM_SEGMENT_H_
#define TURBO_SANTA_COMMON_BACK_END_MEMORY_VRAM_SEGMENT_H_

#include <vector>
#include <glog/logging.h>

#include "back_end/memory/memory_segment.h"

namespace back_end {
namespace memory {

class TileData;

class Tile {
 public:
  virtual unsigned char Get(unsigned int y, unsigned int x) {
    if (y >= 8 || x >= 8) {
      LOG(FATAL) << "x or y is out of range: x = " << x << " y = " << y;
    }
    y *= 2;
    unsigned char value = 0;
    unsigned char mask = 0b00000001;
    mask <<= x;
    
    if ((start_ptr_[y] & mask) != 0) {
      value += 0b00000001;
    }
    if ((start_ptr_[y + 1] & mask) != 0) {
      value += 0b00000010;
    }
    return value;
  }

  virtual void Set(unsigned int y, unsigned int x, unsigned char value) {
    if (y >= 8 || x >= 8) {
      LOG(FATAL) << "x or y is out of range: x = " << x << " y = " << y;
    }
    y *= 2;
    unsigned char mask = 0b00000001;
    mask <<= x;

    if ((value & 0b00000001) != 0) {
      start_ptr_[y] |= mask;
    } else {
      start_ptr_[y] &= ~mask;
    }
    if ((value & 0b00000010) != 0) {
      start_ptr_[y + 1] |= mask;
    } else {
      start_ptr_[y + 1] &= ~mask;
    }
  }

  static const int kTileSize = 16;
 private:
  unsigned char* start_ptr_ = nullptr;
  friend class TileData;
};

class TileData : public ContiguousMemorySegment {
 public:
  TileData(std::vector<unsigned char>* data, unsigned short start_address) :
      data_(data), start_address_(start_address) {}
  virtual unsigned char Read(unsigned short address) { return data_->at(address - lower_address_bound()); }
  virtual void Write(unsigned short address, unsigned char value) { data_->at(address - lower_address_bound()) = value; }
  virtual Tile* tile(unsigned char value) {
    tile_.start_ptr_ = data_->data() + static_cast<unsigned long>(value) * 16;
    return &tile_;
  }

  static const int kTileDataSize = 0x1000;
 protected:
  unsigned short lower_address_bound() { return start_address_; }
  unsigned short upper_address_bound() { return lower_address_bound() + kTileDataSize - 1; } // Bound is not length!!!
 private:
  std::vector<unsigned char>* data_;
  unsigned short start_address_;
  Tile tile_;
};

class LowerTileData : public TileData {
 public:
  LowerTileData(std::vector<unsigned char>* data) : TileData(data, 0x8000) {}
};

class UpperTileData : public TileData {
 public:
  UpperTileData(std::vector<unsigned char>* data) : TileData(data, 0x8800) {}

  virtual Tile* tile(unsigned char value) {
    // Actually value is signed so we have to shift it.
    return TileData::tile(value + 128);
  }
};

class BackgroundMap : public ContiguousMemorySegment {
 public:
  BackgroundMap(unsigned short start_address) :
      data_(kWidth * kHeight, 0x00), start_address_(start_address) {}
  virtual unsigned char Read(unsigned short address) { return data_[address - lower_address_bound()]; }
  virtual void Write(unsigned short address, unsigned char value) { data_[address - lower_address_bound()] = value; }

  virtual unsigned char Get(int y, int x) { return data_[x + y * kWidth]; }
  virtual void Set(int y, int x, unsigned char value) { data_[x + y * kWidth] = value; }

  static const int kHeight = 32;
  static const int kWidth = 32;
 protected:
  unsigned short lower_address_bound() { return start_address_; }
  unsigned short upper_address_bound() { return lower_address_bound() + kWidth * kHeight - 1; }
 private:
  std::vector<unsigned char> data_;
  unsigned short start_address_;
};

class VRAMSegment : public ContiguousMemorySegment {
 public:
  VRAMSegment() :
      raw_tile_data_(0x97ff - 0x8000 + 1, 0x00),
      lower_background_map_(0x9800),
      upper_background_map_(0x9c00),
      lower_tile_data_(&raw_tile_data_),
      upper_tile_data_(&raw_tile_data_) {}

  virtual unsigned char Read(unsigned short address) {
    if (lower_background_map_.InRange(address)) {
      return lower_background_map_.Read(address);
    } else if (upper_background_map_.InRange(address)) {
      return upper_background_map_.Read(address);
    } else if (lower_tile_data_.InRange(address)) {
      return lower_tile_data_.Read(address);
    } else if (upper_tile_data_.InRange(address)) {
      return upper_tile_data_.Read(address);
    } else {
      LOG(FATAL) << "Attempted Read outside of owned region: " << address;
    }
  }

 protected:
  unsigned short lower_address_bound() { return 0x8000; }
  unsigned short upper_address_bound() { return 0x9fff; }

 private:
  std::vector<unsigned char> raw_tile_data_;
  BackgroundMap lower_background_map_;
  BackgroundMap upper_background_map_;
  LowerTileData lower_tile_data_;
  UpperTileData upper_tile_data_;
};

class OAMSegment;

class SpriteAttribute {
 public:
  unsigned char y() { return data_[0]; }
  unsigned char x() { return data_[1]; }
  unsigned char tile_index() { return data_[2]; }
  bool over_background() { return bit(7); }
  bool y_flip() { return bit(6); }
  bool x_flip() { return bit(5); }
  bool palette() { return bit(4); }
 private:
  bool bit(int value) {
    unsigned char mask = 0b00000001 << value;
    return (data_[3] & mask) != 0;
  }

  unsigned char* data_ = nullptr;

  friend class OAMSegment;
};

// Object Attribute Memory (Sprite Attribute Table).
class OAMSegment : public ContiguousMemorySegment {
 public:
  static const int kStartAddress = 0xfe00;
  static const int kEndAddress = 0xfe9f;

  OAMSegment() : data_(kEndAddress - kStartAddress + 1, 0) {}

  virtual unsigned char Read(unsigned short address) { return data_[address - kStartAddress]; }
  virtual void Write(unsigned short address, unsigned char value) { data_[address - kStartAddress] = value; }

  virtual SpriteAttribute* sprite_attribute(unsigned int value) {
    if (value >= 40) {
      LOG(FATAL) << "Attempted to access sprite beyond 40: " << value;
    }
    sprite_attribute_.data_ = data_.data() + value * 4;
    return &sprite_attribute_;
  }

 protected:
  unsigned short lower_address_bound() { return kStartAddress; }
  unsigned short upper_address_bound() { return kEndAddress; }
 private:
  std::vector<unsigned char> data_;
  SpriteAttribute sprite_attribute_;
};

} // namespace memory
} // namespace back_end

#endif // TURBO_SANTA_COMMON_BACK_END_MEMORY_VRAM_SEGMENT_H_
