package com.turbosanta.backend.logging.streams;

import com.turbosanta.backend.logging.streams.InStreamManipulators.Process;
import java.io.FileWriter;
import java.io.IOException;

public class StringPrinterSinkProcess implements Process<String, Void> {
  private FileWriter fileWriter;

  public StringPrinterSinkProcess(String fileName) {
    try {
      this.fileWriter = new FileWriter(fileName);
    } catch (IOException exception) {
      exception.printStackTrace();
      System.exit(-1);
    }
  }

  @Override
  public InStream<Void> apply(InStream<String> inputStream) {
    return inputStream.forEach((String string) -> {
      try {
        fileWriter.append(string);
        fileWriter.write("\n");
      } catch (IOException exception) {
        // All logging ultimately gets written out here so if this fails, we're
        // fucked.
      }
    });
  }
}
