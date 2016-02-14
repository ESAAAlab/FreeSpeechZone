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

#ifndef INCLUDE_STP103_H_
#define INCLUDE_STP103_H_

#include <Arduino.h>
#include <SoftwareSerial.h>

// INTERNET ASCII TYPE HEADER
const byte header_1[] = {203,201,187,201,203,187,201,205,187,203,205,187,201,187,201,201,205,187,201,203,187};
const byte header_2[] = {186,186,186,186,186,' ',186,185,' ',204,203,188,186,186,186,186,185,' ',' ',186,' '};
const byte header_3[] = {202,188,200,188,202,' ',200,205,188,202,200,205,188,200,188,200,205,188,' ',202,' '};

const byte init_printer[] = {0x1B,0x40};

const byte emphasize_on[] = {0x1B,0x45,0xFF};
const byte emphasize_off[] = {0x1B,0x45,0x80};

const byte panelFeed_off[] = {0x1B,0x63,0x35,0xFF};
const byte panelFeed_on[] = {0x1B,0x63,0x35,0x80};

const byte upsidedown_on[] = {0x1B,0x7B,0xFF};
const byte upsidedown_off[] = {0x1B,0x7B,0x80};

const byte reverse_on[] = {0x1D,0x42,0xFF};
const byte reverse_off[] = {0x1D,0x42,0x80};

const byte charset_french[] = {0x1B,0x52,0x01};

const byte charcode_canadianfrench[] = {0x1B,0x74,0x04};

const byte charsize_standard[] = {0x1D,0x21,0x00};
const byte charsize_doubleheight[] = {0x1D,0x21,0x01};
const byte charsize_doublewidth[] = {0x1D,0x21,0x10};
const byte charsize_double[] = {0x1D,0x21,0x11};

const byte smoothing_on[] = {0x1D,0x62,0xFF};
const byte smoothing_off[] = {0x1D,0x62,0x80};

void setupPaperPrinter();
void printPaper(String message);

#endif
