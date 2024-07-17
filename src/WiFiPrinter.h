/* WiFiPrinter

Copyright 2020 Bert Melis

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#pragma once

#include <Arduino.h>

#include <Print.h>
#include <WiFiClient.h>
#include <WiFiServer.h>

#ifndef WIFIPRINTER_PORT
#define WIFIPRINTER_PORT 23
#endif

class WiFiPrinterClass : public Print {
 public:
  explicit WiFiPrinterClass(uint16_t port = 23);
  ~WiFiPrinterClass();

  void begin();
  void end();

  size_t write(uint8_t byte);
  size_t write(const uint8_t* buffer, size_t size);

  typedef std::function<void(void)> onClientConnectCb;
  void onClientConnect(onClientConnectCb cb);
  typedef std::function<void(const uint8_t* data, size_t len)> onClientDataCb;
  void onClientData(onClientDataCb cb);

  void handle();

 protected:
  WiFiServer _server;
  WiFiClient _client;
  onClientConnectCb _onClientConnectCb;
  onClientDataCb _onClientDataCb;
};

extern WiFiPrinterClass WiFiPrinter;
