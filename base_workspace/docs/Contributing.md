# Contributing to Turbo

## Version Control

#### Use Feature Branches.

Keep your pull-requests specific; if you are adding a new module to memory and
notice a style violation in graphics, please file separate pull-requests.

#### Don't Just Commit to Master; Use Pull-Requests.

All large changes should be reviewed by either Diego or Brendan (preferably
both).

## C++ Style Guide

Aside from the exceptions and additions below, we use
[Google's C++ Style Guide](https://google.github.io/styleguide/cppguide.html).

#### Use only lower case letters in hexadecimal values (i.e. `0xff` not `0xFF`).

#### Binary literal values (`0b00001111`) are highly encouraged when working with bit-masks.

#### Despite what Google's style guide says, `std::string` may be used wherever you like.

#### Use glog `CHECK` and `LOG(FATAL)` for assertions.

We use [glog](https://google-glog.googlecode.com/svn/trunk/doc/glog.html)
for logging. For assertions use either an if statement with `LOG(FATAL)` or
one of the various forms of `CHECK` (`CHECK` is preferred). **Do not use
`<cassert>` or `<assert.h>`.**

#### `std::shared_ptr` should not be used unless you have a very good reason.

#### C++ lambdas are okay as long as they *can* fit on one line.

This means that if the lambda could be *reasonably* put on one line (satisfying
the 80 character column limit), you *may* use it.

#### Avoid using implicit bool conversions.

Pointers and `unique_ptr`s are okay, numeric values are not.

#### Dynamic memory should always be managed by smart pointers, you should never have to call delete.

#### Avoid including C files within your header files as they tend to pollute the global namespace.

#### Report non-conformant code.

A lot of the code is very rough around the edges, if you see something that does
not obey what was said above, please file an issue on Github, or fix it if you
like. There are two caveats here, we are aware that
`backend/opcode_executor/opcode_handler.cc` is a mess and `test_harness/*` is a
total cluster fuck.

## Java Style Guide

We use the [Google Java Style Guide](https://google.github.io/styleguide/javaguide.html).

## Useful Stuff

Listed below are some links that will probably come in handy:

 - [Google's C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
 - [Google's Java Style Guide](https://google.github.io/styleguide/javaguide.html)
 - C/C++ standard libs - http://en.cppreference.com/w/
 - bazel - http://bazel.io/
 - Gameboy reference material:
   * http://marc.rawer.de/Gameboy/Docs/GBCPUman.pdf
   * http://gbdev.gg8.se/files/docs/mirrors/pandocs.html
   * http://gbdev.gg8.se/wiki/articles/Gameboy_Bootstrap_ROM
 - C++ vimrc (its formatting is conformant with the style guide) - https://github.com/vim-scripts/google.vim
 - I would also advise using YouCompleteMe - http://valloric.github.io/YouCompleteMe/
