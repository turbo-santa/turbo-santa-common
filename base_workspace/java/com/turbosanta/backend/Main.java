package com.turbosanta.backend;

import java.io.File;
import java.io.FileInputStream;

import static java.lang.System.out;

public class Main {
  public static void main(String[] args) throws Exception {
    // if (args.length < 2) {
    //   out.println("Must provide path to ROM file.");
    //   return;
    // }
    String romPath = "roms/tetris.gb"; // args[1];
    byte[] rom = readROMFile(romPath);

    Clocktroller clocktroller = new Clocktroller();
    clocktroller.init(rom, rom.length);
    clocktroller.run();
    clocktroller.waitUntilDone();
  }

  public static byte[] readROMFile(String fileName) throws Exception {
    File file = new File(fileName);
    long size = file.length();
    byte[] rom = new byte[(int) size];
    FileInputStream stream = new FileInputStream(file);
    stream.read(rom);
    stream.close();
    return rom;
  }
}
