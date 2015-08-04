package com.turbosanta.backend;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

public class BackendUtils {
  private BackendUtils() {}

  public static byte[] readROMFile(String fileName) throws IOException {
    File file = new File(fileName);
    long size = file.length();
    byte[] rom = new byte[(int) size];
    FileInputStream stream = new FileInputStream(file);
    stream.read(rom);
    stream.close();
    return rom;
  }
}
