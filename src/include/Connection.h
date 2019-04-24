//
// Created by Kris on 2019-04-13.
//

#ifndef FILE_SERVER_CONNECTION_H
#define FILE_SERVER_CONNECTION_H

#include <iostream>
#include <string>

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>


namespace FileServer {
class Connection {


 public:
  const static int BUFFER_SIZE = 1500;

  void Socket();
  void Bind();
  void Connect(const char *, int);
  void Listen();
  void Accept();
  bool Open();

  void setMessage(const unsigned char *);
  unsigned char* Write();
  unsigned char* Read();

  Connection();
  Connection(int);

 private:
  int m_port;
  int opt = 1;
  int listening = 0;
  int connected;

  char host[NI_MAXHOST];
  char svc[NI_MAXSERV];

  unsigned char i_buffer[BUFFER_SIZE];
  unsigned char o_buffer[BUFFER_SIZE];

  struct sockaddr_in local_address;
  struct sockaddr_in remote_address;

};
}

#endif //FILE_SERVER_CONNECTION_H
