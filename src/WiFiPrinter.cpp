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

WiFiPrinter::WiFiPrinter(uint16_t port) :
  _port(port),
  _server(nullptr),
  _client(nullptr) {}

WiFiPrinter::~WiFiPrinter() {
  if (_client) _client->close(true);
}

void WiFiPrinter::begin() {
  _server = new AsyncServer(_port);
  _server->onClient(_onClientConnect, this);
  _server->begin();
}

void WiFiPrinter::end() {
  delete _server;  // will also close connections
}

size_t WiFiPrinter::write(uint8_t byte) {
  return write(&byte, 1);
}

size_t WiFiPrinter::write(const uint8_t *buffer, size_t size) {
  if (_client) {
    if (_client->canSend() && _client->space() > size) {
      _client->write(reinterpret_cast<const char*>(buffer), size);
      return size;
    }
  }
  return 0;
}

void WiFiPrinter::_onClientConnect(void* arg, AsyncClient* client) {
  WiFiPrinter* i = reinterpret_cast<WiFiPrinter*>(arg);
  if (i->_client) {
    i->_client->close(true);
  }
  i->_client = client;
  i->_client->onDisconnect(_onClientDisconnect, i);
}

void WiFiPrinter::_onClientDisconnect(void* arg, AsyncClient* client) {
  WiFiPrinter* i = reinterpret_cast<WiFiPrinter*>(arg);
  if (i->_client) i->_client = nullptr;
}
