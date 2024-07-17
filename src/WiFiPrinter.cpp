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

#include <WiFiPrinter.h>

WiFiPrinterClass::WiFiPrinterClass(uint16_t port)
: _server(port, 1)
, _client()
, _onClientConnectCb(nullptr)
, _onClientDataCb(nullptr) {
  // empty
}

WiFiPrinterClass::~WiFiPrinterClass() {
  if (_client) _client.stop();
}

void WiFiPrinterClass::begin() {
  _server.begin();
}

void WiFiPrinterClass::end() {
  _client.stop();
  _server.stop();
}

size_t WiFiPrinterClass::write(uint8_t byte) {
  return write(&byte, 1);
}

size_t WiFiPrinterClass::write(const uint8_t* buffer, size_t size) {
  if (_client.connected()) {
    return _client.write(reinterpret_cast<const char*>(buffer), size);
  }
  return 0;
}

void WiFiPrinterClass::onClientConnect(onClientConnectCb cb) {
  _onClientConnectCb = cb;
}

void WiFiPrinterClass::onClientData(onClientDataCb cb) {
  _onClientDataCb = cb;
}

void WiFiPrinterClass::handle() {
  if (_server.hasClient()) {
    if (_client) {
      Serial.print("stopping existing client\n");
      _client.stop();
    }
    Serial.print("accepting client\n");
    _client = _server.accept();
    if (_onClientConnectCb) _onClientConnectCb();
  }
  if (_client.connected() && _client.available() && _onClientDataCb) {
    size_t len = _client.available();
    uint8_t* data = reinterpret_cast<uint8_t*>(malloc(len + 1));
    _client.read(data, len);
    data[len] = '\0';
    _onClientDataCb(data, len);
    free(data);
  }
}

WiFiPrinterClass WiFiPrinter(WIFIPRINTER_PORT);
