# Decompiler

## Overview

By itself, the decompiler is a static decompiler; this means that it will
attempt to decompile a binary without executing any of the instructions. The
core algorithm it implements is quite simple:

```
stack = [START_ADDRESS]
for address in stack:
  instruction = ReadInstructionAt(address)
  SaveInstruction(instruction)
  if IsJump(instruction):
    stack.push(instruction.jump_address)
  endif
  stack.push(address + instruction.instr_width)
endfor
```

## Detailed Explanation

In principle, it does a depth first search through the code; however,
difficulties arise in practice. The Gameboy mixes data with instructions and
instructions are not fixed with; this means that anything following an
unconditional jump may not be a valid instruction. Even worse, not all jumps are
to fixed positions; these jump addresses are computed at runtime. Thus, it may
not be possible to statically decompile an entire binary. Nevertheless, I made
the decompiler to be restartable, so that it could be given new valid addresses
while executing the binary and, in theory, decompile an entire binary.
Thus, decompiler and emulator together are also a dynamic decompiler.

In addition to decompiling binaries into more readable assembly formats, the
decompiler also serves as Turbo's instruction parser during emulation; thus,
when the Opcode Executor requests an instruction at some address, instead of
parsing an instruction at some location in normal memory, it looks up a
decompiled instruction associated with that address; aside from making the
Opcode Executor more predictable, this also makes the emulator more
efficient as it only has to perform the logic to parse an instruction once and
in theory would allow for partial binary translation in the future.

## Organization and Usage

As far as the organization of the decompiler, it resides in
`backend/decompiler.` The algorithm, which I described above, resides in
`decompiler.h` and `decompiler.cc`. The decompiler represents parsed
instructions as the Instruction object which is defined in `instruction.h`.
Individual instructions are parsed using `InstructionFactory`s which are defined
in `instruction_factory.h`. One `InstructionFactory` is created for each type of
instruction; this collection of `InstructionFactory`s is created in
`instruction_map.h` and `instruction_map.cc` using helper functions (defined in
`instruction_factory_helpers.h`) which create an almost DSL of sorts (for
example: 

```cpp
instr(LD, 0xea, 16, ptr(BIT_16), val(A));
```

creates a factory for parsing the instruction 

```
LD (0x1234), A)
```

The `DecompilerFactory` (`decompiler_factory.h`)
is used for creating decompilers with various settings, for example:

```cpp
DecompilerFactory factory;
VectorROMBridge bridge(rom.get());
factory.set_rom(&bridge);
factory.set_type(DecompilerFactory::FORMATTED_ROM);
unique_ptr<Decompiler> decompiler = factory.Build();
```

creates a decompiler using the binary contained in ROM, such that it will
attempt to parse it as if it had the usual Nintendo ROM layout.

## Building

A stand-alone static decompiler is implemented in "main.cc", it compiles to a
program named "decompile" which may be built:

```
$ bazel build //cc/backend/decompiler:all
```

And executed as:

```
$ decompile path/to/bin.gb path/to/out.s
```
