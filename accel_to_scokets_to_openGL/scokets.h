#ifndef SCOKETS_H
#define SCOKETS_H

//#pragma once

#include <string>
using std::string;

#include <WinSock2.h>

class my_scoket
{
public:
   my_scoket();
   ~my_scoket();

   int init(char *c_str_host_addr, char *c_str_port);
   int send_data(char *tx_buff, int tx_bytes_to_send);
   int receive_data(char *rx_buff, int rx_buff_len);

private:
   bool m_init_failed;
   SOCKET m_connection_socket;
   struct addrinfo *m_host_info_list_head;
   struct addrinfo *m_host_info_ptr;
};

#endif SCOKETS_H
