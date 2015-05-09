#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "backend/decompiler/decompiler.h"
#include "backend/decompiler/decompiler_factory.h"
#include "backend/decompiler/rom_bridge.h"
#include "submodules/glog/src/glog/logging.h"

using std::filebuf;
using std::ostream;
using std::string;
using std::unique_ptr;
using std::vector;
using back_end::decompiler::Decompiler;
using back_end::decompiler::DecompilerFactory;
using back_end::decompiler::VectorROMBridge;

unique_ptr<vector<unsigned char>> ReadROM(string file_name) {
  unique_ptr<vector<uint8_t>> rom = unique_ptr<vector<uint8_t>>(new vector<uint8_t>());
  FILE* file = fopen(file_name.c_str(), "r");
  if (file == nullptr) {
    LOG(FATAL) << "Cannot read file " << file_name << ": " << strerror(errno);
  }

  const int buffer_size = 1024;
  vector<unsigned char> buffer(buffer_size);
  int amount_read = 0;
  do {
    amount_read = fread(buffer.data(), 1, buffer_size, file);
    // TODO(Brendan): Should not copy the entire buffer since buffer may not be
    // entirely filled.
    rom->insert(rom->end(), buffer.begin(), buffer.begin() + amount_read);
  } while (amount_read == buffer_size);

  fclose(file);
  return rom;
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    printf("Please provide the name of the ROM and the out file.\n");
    return -1;
  }

  unique_ptr<vector<uint8_t>> rom = ReadROM(argv[1]);
  DecompilerFactory factory;
  VectorROMBridge bridge(rom.get());
  factory.set_rom(&bridge);
  factory.set_type(DecompilerFactory::FORMATTED_ROM);
  Decompiler decompiler = factory.Build();
  decompiler.AddPathStart(0);
  decompiler.Decompile();

  std::filebuf fb;
  fb.open(argv[2], std::ios::out);
  ostream file(&fb);
  decompiler.PrintToStream(&file);
  fb.close();
  return 0;
}
