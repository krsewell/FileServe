//
// Created by kris on 2019-04-23.
//
#include <iostream>
#include <string.h>
#include "include/Connection.h"

int main() {
  const unsigned char var[] = "A bunch of CoCoNuts!\r\n";
  auto host = "127.0.0.1";

  auto port = new FileServer::Connection(8999);
  port->Connect(host,9000);
  port->setMessage(var);
  port->Write();

  return 0;
}
