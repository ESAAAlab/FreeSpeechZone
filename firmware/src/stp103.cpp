/*

Free Speech Zone
Wifi-Connected Thermal Printer, censoring everything people say.
Copyright (c) 2016 Kamel Makhloufi. All rights reserved.

This project is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This project is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/

#include "stp103.h"

SoftwareSerial printerSerial(12, 13, false);

void setupPaperPrinter() {
  // Set the data rate for the SoftwareSerial port
  printerSerial.begin(9600);

  delay(500);

  // Initialize Printer
  printerSerial.write(init_printer,2);
  printerSerial.write(panelFeed_off,4);
  printerSerial.write(emphasize_on,3);
  printerSerial.write(smoothing_off,3);
  printerSerial.write(charsize_double,3);

  // Print INTERNET header
  printerSerial.write(header_1,21);
  printerSerial.println();
  printerSerial.write(header_2,21);
  printerSerial.println();
  printerSerial.write(header_3,21);
  printerSerial.println();
  printerSerial.println();
  printerSerial.println();

  // Setup printouts
  printerSerial.write(emphasize_off,3);
  printerSerial.write(smoothing_on,3);
}

void printPaper(String message) {
  printerSerial.println(message);
  // Feed a bit of paper
  printerSerial.println();
  printerSerial.println();
  printerSerial.println();
}
