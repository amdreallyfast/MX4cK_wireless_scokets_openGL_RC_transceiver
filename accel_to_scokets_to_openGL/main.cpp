#include "scokets.h"

#include <iostream>
using std::cout;
using std::endl;

static const unsigned int BUFF_SIZE = 512;
static const unsigned int MAX_ACCEL_READS = 12;

int main(void)
{
   int num_accelerometer_reads = 0;
   char tx_buffer[BUFF_SIZE];
   char rx_buffer[BUFF_SIZE];
   my_scoket S;

   S.init("10.10.10.126", "5");
   //_snprintf(tx_buffer, BUFF_SIZE, "hi there! %d", 10);
   //S.send_data(tx_buffer, strlen(tx_buffer));

   while (1)
   {
      S.receive_data(rx_buffer, BUFF_SIZE);
      system("cls");
      cout << "'" << rx_buffer << "'" << endl;
   }

   cout << rx_buffer << endl;



   return 0;
}