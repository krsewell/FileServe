#include <iostream>
#include <unistd.h>
#include "include/Connection.h"

int main() {
  // create socket
  auto port = new FileServer::Connection();
  port->Socket();
  port->Bind();
  port->Listen();
  port->Accept();

  while (port->Open())
  {
    auto packet = port->Read();
    std::cout << packet;

//    for (int i = 0; i < FileServer::Connection::BUFFER_SIZE && packet[i] != '\0'; i++)
//    {
//      std::cout << packet[i];
//    }

    usleep(1000);
  }

  return 0;
}