#include <curses.h>

#include <algorithm>
#include <iostream>
#include <vector>
#include <thread>

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "backend/clocktroller/clocktroller.h"
#include "backend/memory/joypad_module.h"
// #include "backend/debugger/frames.h"
// #include "backend/debugger/deltas.h"
// #include "backend/debugger/great_library.h"
// #include "backend/debugger/librarians.h"
// #include "backend/graphics/screen.h"

using std::cout;
using std::endl;
using std::hex;
using std::dec;
using std::string;
using std::thread;
using std::vector;
using back_end::clocktroller::Clocktroller;
// using back_end::debugger::Frame;
// using back_end::debugger::RegisterDelta;
// using back_end::debugger::MemoryDelta;
// using back_end::debugger::GreatLibrary;
using back_end::graphics::DefaultRaster;
using back_end::graphics::Screen;
using back_end::graphics::ScreenRaster;
using back_end::memory::JoypadFlag;

static const int kNintendoLogoStartPosition = 0x104;

static const vector<unsigned char> kNintendoLogo = {
    0xce, 0xed, 0x66, 0x66, 0xcc, 0x0d, 0x00, 0x0b, 
    0x03, 0x73, 0x00, 0x83, 0x00, 0x0c, 0x00, 0x0d,
    0x00, 0x08, 0x11, 0x1f, 0x88, 0x89, 0x00, 0x0e,
    0xdc, 0xcc, 0x6e, 0xe6, 0xdd, 0xdd, 0xd9, 0x99,
    0xbb, 0xbb, 0x67, 0x63, 0x6e, 0x0e, 0xec, 0xcc,  
    0xdd, 0xdc, 0x99, 0x9f, 0xbb, 0xb9, 0x33, 0x3e
  }; 

class TerminalScreen : public Screen {
 public:
  virtual void Draw() {
    const int realized_width = std::min(COLS, LINES);
    for (int y = 0; y < ScreenRaster::kScreenHeight; y++) {
      for (int x = 0; x < ScreenRaster::kScreenWidth; x++) {
        unsigned char pixel_shade = raster_.Get(y, x);
        char real_pixel;
        if (pixel_shade <= 64) {
          real_pixel = ' ';
        } else if (pixel_shade <= 128) {
          real_pixel = '.';
        } else if (pixel_shade <= 192) {
          real_pixel = '*';
        } else {
          real_pixel = '#';
        }

        move(y * realized_width / ScreenRaster::kScreenHeight, x * 2 * realized_width / ScreenRaster::kScreenWidth);
        addch(real_pixel);
      }
    }
    refresh();
  }

  ScreenRaster* mutable_raster() { return &raster_; }

  const ScreenRaster& raster() { return raster_; }

 private:
  DefaultRaster raster_;
};

vector<unsigned char> BuildROM() {
  const int rom_size = kNintendoLogo.size() + kNintendoLogoStartPosition;
  vector<unsigned char> rom(rom_size, 0x00);
  for (size_t i = 0; i < kNintendoLogo.size(); i++) {
    rom[i + kNintendoLogoStartPosition] = kNintendoLogo[i];
  }
  return rom;
}

vector<unsigned char> ReadROM(string file_name) {
  vector<unsigned char> rom;
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
    rom.insert(rom.end(), buffer.begin(), buffer.end());
  } while (amount_read == buffer_size);

  fclose(file);
  return rom;
}

// void printFrame(Frame& frame) {
//   cout << "Event: " << frame.event() << endl;
//   cout << "Timestamp: " << dec << frame.timestamp() << endl;
//   cout << "Instruction executed before: " << frame.pc_delta().visited_before << endl;
//   cout << "Instruction: " << frame.str_instruction() << endl;
//   cout << "Raw: " << hex << frame.raw_instruction() << endl;
//   cout << "PC from " << hex << frame.pc_delta().old_value << " to " << hex << frame.pc_delta().new_value << endl;
// }

// void ViewHistory(GreatLibrary* great_library) {
//   auto iterator = great_library->end();
//   iterator--;
//   cout << "Size of GreatLibrary = " << great_library->last_frame().timestamp() << endl;
//   cout << "q quits" << endl;
//   cout << "n increments frame" << endl;
//   cout << "p decrements frame" << endl;
//   cout << "j X jumps to frame X" << endl;
//   cout << "l jumps to last instance of frame" << endl;
//   cout << "h jumps to last instance of repeated frame" << endl;
//   cout << "i prints out frame info" << endl;
//   cout << "m prints out memory changes" << endl;
//   cout << "r prints out register changes" << endl;
//   cout << "g X gets the current value of a register (This is jank)" << endl;
// 
//   long timestamp = iterator->timestamp();
//   while (true) {
//     string nextline;
//     getline(std::cin, nextline);
//     if (nextline.size() == 0) continue;
//     switch(nextline.at(0)) {
//       case 'q':
//         cout << "Quitting" << endl;
//         return;
//       case 'n':
//         {
//           int times = 1;
//           if (nextline.size() > 2) {
//             times = atoi(nextline.substr(2).c_str());
//           }
//           for (int i = 0; i < times; i++) {
//             iterator++;
//           }
//           cout << "Incremented to frame " << dec << iterator->timestamp() << endl;
//           break;
//         }
//       case 'p':
//         {
//           int times = 1;
//           if (nextline.size() > 2) {
//             times = atoi(nextline.substr(2).c_str());
//           }
//           for (int i = 0; i < times; i++) {
//             iterator--;
//           }
//           cout << "Decremented to frame " << dec << iterator->timestamp() << endl;
//           break;
//         }
//       case 'j':
//         {
//           int new_timestamp = atoi(nextline.substr(2).c_str());
//           if (new_timestamp > great_library->last_frame().timestamp()) {
//             cout << "Dat frame too big yo" << endl;
//             break;
//           }
//           int change = new_timestamp - timestamp;
//           if (change > 0) {
//             for (int i = 0; i < change; i++) {
//               iterator++;
//             }
//           } else {
//             for (int i = 0; i > change; i--) {
//               iterator--;
//             }
//           }
//           cout << "Jumped to frame " << dec << iterator->timestamp() << endl;
//           break;
//         }
//       case 'l':
//         iterator = back_end::debugger::LastTimeExecuted(iterator);
//         cout << "Jumped to frame " << dec << iterator->timestamp() << endl;
//         break;
//       case 'h':
//         iterator = back_end::debugger::MostRecentOldAddress(iterator);
//         cout << "Jumped to frame " << dec << iterator->timestamp() << endl;
//         break;
//       case 'i':
//         printFrame((Frame&)*iterator);
//         break;
//       case 'r':
//         for (RegisterDelta rd : iterator->register_deltas()) {
//           cout << "Register " << rd.GetName() << " changed " << hex << rd.old_value << " => " << hex << rd.new_value << endl;
//         }
//         break;
//       case 'g':
//         {
//           string register_name = nextline.substr(2);
//           bool finished = false;
// 
//           for (auto temp_iter = iterator; temp_iter->timestamp() > 0; temp_iter--) {
//             for (RegisterDelta rd: temp_iter->register_deltas()) {
//               if (rd.GetName() == register_name) {
//                 cout << "Register " << rd.GetName() << ": " << hex << rd.new_value << endl;
//                 finished = true;
//                 break;
//               }
//             }
//             if (finished) break;
//           }
//           break;
//         }
//       case 'm':
//         for (MemoryDelta md : iterator->memory_deltas()) {
//           cout << "Memory address " << md.address << " changed " << hex << md.old_value + 0x0000 << " => " << hex << md.new_value + 0x0000 << endl;
//         }
//         break;
//       default:
//         break;
//     }
//     const Frame& frame = *iterator;
//     timestamp = frame.timestamp();
//   }
// }

void HandleInput(Clocktroller* clocktroller) {
  JoypadFlag* joypad_flag = clocktroller->joypad_flag();
  cbreak();
  bool running = true;
  while (running) {
    switch (getch()) {
      case 'w':
        joypad_flag->set_up();
        break;
      case 'a':
        joypad_flag->set_left();
        break;
      case 's':
        joypad_flag->set_down();
        break;
      case 'd':
        joypad_flag->set_right();
        break;
      case 'u':
        joypad_flag->set_select();
        break;
      case 'i':
        joypad_flag->set_start();
        break;
      case 'j':
        joypad_flag->set_a();
        break;
      case 'k':
        joypad_flag->set_b();
        break;
      case '\\':
        clocktroller->Kill();
        running = false;
        break;
      default:
        break;
    }
  }
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Please provide the name of the ROM\n");
    return -1;
  }

  google::InstallFailureSignalHandler();

  TerminalScreen terminal_screen;
//   GreatLibrary great_library;
  vector<unsigned char> rom = ReadROM(argv[1]);
  LOG(INFO) << "Finished reading rom";
  Clocktroller clocktroller(&terminal_screen);
  LOG(INFO) << "Clocktroller built";

  initscr();
  clocktroller.Init(rom.data(), rom.size());
  clocktroller.Run();
  thread input_thread(HandleInput, &clocktroller);
  clocktroller.Wait();
  input_thread.join();
  endwin();
//   ViewHistory(&great_library);
  return 0;
};
