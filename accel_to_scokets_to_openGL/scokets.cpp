#include "scokets.h"

#include <iostream>
using std::cout;
using std::endl;

#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment (lib, "Ws2_32.lib")

my_scoket::my_scoket() 
   : m_init_failed(false), 
   m_connection_socket(INVALID_SOCKET),
   m_host_info_list_head(NULL),
   m_host_info_ptr(NULL)
{
}

my_scoket::~my_scoket()
{
   int result_val = 0;

   // shut down the connection for sending and receiving
   result_val = shutdown(m_connection_socket, SD_BOTH);
   if (SOCKET_ERROR == result_val)
   {
      cout << "shutdown failed: '" << WSAGetLastError() << "'" << endl;
   }

   // close the socketed (if it was opened)
   if (m_connection_socket != INVALID_SOCKET)
   {
      closesocket(m_connection_socket);
      m_connection_socket = INVALID_SOCKET;
   }

   // free the memory containing the linked list of "host address info" structures
   freeaddrinfo(m_host_info_list_head);

   // clean up anything left over in WS2_32.dll
   WSACleanup();
}

int my_scoket::init(char *c_str_host_addr, char *c_str_port)
{
   WSADATA wsa_data;
   int result_val = 0;
   int this_ret_val = 0;
   struct addrinfo server_search_hints;

   // check for input shenanigans
   if (0 == c_str_host_addr) { this_ret_val = -1; }
   else if (0 == c_str_port) { this_ret_val = -2; }

   // startup with version 2.2, and give it a pointer to an instantiated "WSADATA"
   // object so that it's happy
   result_val = WSAStartup(MAKEWORD(2, 2), &wsa_data);
   if (result_val != 0)
   {
      cout << "WSAStartup(...) failed, returning '" << this_ret_val << "'" << endl;
      this_ret_val = -3;
   }

   if (0 == this_ret_val)
   {
      // WSAStartup(...) did not fail, so create the socket

      ZeroMemory(&server_search_hints, sizeof(server_search_hints));
      server_search_hints.ai_family = AF_UNSPEC;
      server_search_hints.ai_socktype = SOCK_STREAM;
      server_search_hints.ai_protocol = IPPROTO_TCP;

      // resolve the server address and port and save the list of host info information as a member
      result_val = getaddrinfo(
         c_str_host_addr, 
         c_str_port, 
         &server_search_hints, 
         &m_host_info_list_head);

      if (result_val != 0)
      {
         cout << "getaddrinfo(...) failed, returning '" << result_val << "'" << endl;
         this_ret_val = -4;
      }
   }

   if (0 == this_ret_val)
   {
      // create a windows socket to connect to the server, starting with the first entry in the list of "host info" structures
      m_host_info_ptr = m_host_info_list_head;
      m_connection_socket = socket(
         m_host_info_ptr->ai_family, 
         m_host_info_ptr->ai_socktype, 
         m_host_info_ptr->ai_protocol);

      if (INVALID_SOCKET == m_connection_socket)
      {
         cout << "error at socket(...): '" << WSAGetLastError() << "'" << endl;
         this_ret_val = -5;
      }
   }

   if (0 == this_ret_val)
   {
      // open the connection
      result_val = connect(m_connection_socket, m_host_info_ptr->ai_addr, (int)(m_host_info_ptr->ai_addrlen));
      if (SOCKET_ERROR == result_val)
      {
         // we should consider trying the next address in the list rather than giving up entirely

         cout << "unable to connect to server" << endl;
         cout << "error at connect(...): '" << WSAGetLastError() << "'" << endl;
         this_ret_val = -6;
      }
   }

   if (0 != this_ret_val)
   {
      m_init_failed = true;
   }

   return this_ret_val;
}


int my_scoket::send_data(char *tx_buff, int tx_bytes_to_send)
{
   int result_val = 0;
   int this_ret_val = 0;

   // check for initialization problems and input shenanigans
   if (m_init_failed) { this_ret_val = -1; }
   else if (0 == tx_buff) { this_ret_val = -2; }
   else if (tx_bytes_to_send < 1) { this_ret_val = -3; }

   if (0 == this_ret_val)
   {
      // connection did not fail, so send the buffer

      // cross your fingers and hope that it is a null-terminated string
      cout << "sending '" << tx_buff << "', length '" << tx_bytes_to_send << "'" << endl;
      result_val = send(m_connection_socket, tx_buff, tx_bytes_to_send, 0);
      if (SOCKET_ERROR == result_val)
      {
         cout << "send failed: '" << WSAGetLastError() << "'" << endl;
         this_ret_val = -5;
      }
      else
      {
         cout << "bytes send: '" << result_val << "'" << endl;
      }
   }


   return this_ret_val;
}


int my_scoket::receive_data(char *rx_buff, int rx_buff_len)
{
   int rx_byte_count = 0;
   int this_ret_val = 0;

   // check for initialization problems and input shenanigans
   if (m_init_failed) { this_ret_val = -1; }
   else if (0 == rx_buff) { this_ret_val = -2; }
   else if (rx_buff_len < 1) { this_ret_val = -3; }

   if (0 == this_ret_val)
   {
      // get the incoming data
      // Note: Hopefully, the data won't get chopped off midstream because of the
      // speed difference between my computer's cpu and the microcontroller's processor
      rx_byte_count = recv(m_connection_socket, rx_buff, rx_buff_len, 0);
      if (rx_byte_count > 0)
      {
         rx_buff[rx_byte_count] = 0;
         cout << "bytes received: '" << rx_byte_count << "'" << endl;
      }
      else if (0 == rx_byte_count)
      {
         cout << "connection closed" << endl;
      }
      else
      {
         // "received byte count" < 0
         cout << "receive failed: '" << WSAGetLastError() << "'" << endl;
         this_ret_val = -4;
      }
   }

   return this_ret_val;
}
