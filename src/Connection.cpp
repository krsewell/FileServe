//
// Created by kris on 2019-04-13.
//

#include "include/Connection.h"


void DieWithError( char * errorMessage) {
  perror( errorMessage );
  exit(1);
}


using namespace FileServer;

Connection::Connection() {
  memset(host, 0, NI_MAXHOST);
  memset(svc, 0, NI_MAXSERV);
  memset(i_buffer, 0, BUFFER_SIZE);
  memset(o_buffer, 0, BUFFER_SIZE);

  m_port = 9000;
  local_address.sin_family = AF_INET;
  local_address.sin_port = htons(m_port);
  local_address.sin_addr.s_addr = htonl(INADDR_ANY);
  //inet_pton(AF_INET, "0.0.0.0", &local_address.sin_addr);

  remote_address.sin_family = AF_INET;
}

Connection::Connection(int p) : Connection() {
  m_port = p;
  local_address.sin_port = m_port;
}

void Connection::Socket() {
  listening = socket(AF_INET, SOCK_STREAM, 0);
  if (listening == -1) {
    DieWithError("ERROR opening socket");
  }
  setsockopt(listening, SOL_SOCKET, SO_REUSEPORT | SO_LINGER, &opt, sizeof(opt));
}

void Connection::Bind() {
  if ((bind(listening, reinterpret_cast<const sockaddr *>(&local_address), sizeof(local_address))) == -1) {
    DieWithError("ERROR binding socket");
  }
}

void Connection::Listen() {
  if (listen(listening, SOMAXCONN) == -1) {
    DieWithError("Error listening on socket");
  }
}

void Connection::Accept() {
  socklen_t remoteSize = sizeof(remote_address);
  connected = accept(listening,
      reinterpret_cast<sockaddr *>(&remote_address), &remoteSize);
  if (connected == -1) {
    DieWithError("ERROR client connecting");
  }
  close(listening);
  int result = getnameinfo(reinterpret_cast<const sockaddr *>(&remote_address), sizeof(remote_address),
      host, NI_MAXHOST, svc, NI_MAXSERV, 0);
  if (result)
  {
    std::cout << "Connected: " << host << ":" << svc << std::endl;
  } else {
    inet_ntop(AF_INET, &remote_address.sin_addr, host, NI_MAXHOST);
    std::cout << "Connected: " << host << ":" << ntohs(remote_address.sin_port) << std::endl;
  }
}

void Connection::Connect(const char *add, int port) {
  remote_address.sin_port = htons(port);
  inet_pton(AF_INET, add, &remote_address.sin_addr);
  if ((connect(connected, (struct sockaddr *) &remote_address, sizeof(remote_address)) < 0)) {
    DieWithError("ERROR connecting to port");
  }
}

bool Connection::Open() {
  return (connected > 0);
}

void Connection::setMessage(const unsigned char *message) {
  strcpy(reinterpret_cast<char *>(o_buffer), reinterpret_cast<const char *>(message));
}

unsigned char* Connection::Write() {
  send(connected, o_buffer, BUFFER_SIZE, 0);
  const auto var = o_buffer;
  return var;
}

unsigned char* Connection::Read() {
  memset(i_buffer, 0, sizeof(i_buffer));  // clear the buffer of old data before reading more.
  int bytesRecv = recv(connected, i_buffer, BUFFER_SIZE, 0);
  if (bytesRecv == -1) {
    DieWithError("ERROR connection receiving.");
  }

  if (bytesRecv == 0) {
    std::cout << "Client disconnected" << std::endl;
  }
  return i_buffer;
}

// EOF