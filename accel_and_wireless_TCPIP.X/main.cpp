#include <stdio.h>

extern "C"
{
#include <peripheral/system.h>
#include <peripheral/timer.h>
#include <peripheral/ports.h>
#include <peripheral/i2c.h>

#include "../../github_my_MX4cK_framework/my_C_TCPIP_framework.h"
#include "../../github_my_MX4cK_framework/my_CPP_I2C_handler.h"
#include "../../github_my_MX4cK_framework/my_function_queue.h"
}



// ------------------ Configuration Oscillators --------------------------------
// SYSCLK = 80 MHz (8MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK  = 40 MHz
// -----------------------------------------------------------------------------
#pragma config FPLLMUL = MUL_20
#pragma config FPLLIDIV = DIV_2
#pragma config FPLLODIV = DIV_1
#pragma config FWDTEN = OFF
#pragma config POSCMOD = HS
#pragma config FNOSC = PRIPLL
#pragma config FPBDIV = DIV_2
//#pragma config CP = OFF, BWP = OFF, PWP = OFF
//#pragma config UPLLEN = OFF


#define SYSTEM_CLOCK          80000000
#define PB_DIV                2
#define T1_PS                 64
#define T1_TOGGLES_PER_SEC    1000
#define T1_TICK_PR            SYSTEM_CLOCK/PB_DIV/T1_PS/T1_TOGGLES_PER_SEC
#define T1_OPEN_CONFIG        T1_ON | T1_SOURCE_INT | T1_PS_1_64

// this is the port number that we will be using
// Note: This can be any number, really, that is within the range of a 16 bit
// integer because the TCPIP stack that this program is using uses a WORD for
// the port type, which is a typedef of an unsigned short int.  Just make sure
// that your Tera Term connection also specifies the same port.
#define SERVER_PORT	5

// this is the size of our message buffer that we will use to receive data from
// and send data over the network via TCPIP
// Note: This is a #define so that it can be used in both the buffer
// declaration and when a maximum buffer size needs to be specified.
#define MESSAGE_BUFFER_SIZE_BYTES 64


unsigned int gMillisecondsInOperation;
BOOL g_TCPIP_service_can_start;
BOOL g_accel_init_good;

void TCPIP_send_receive(void);
void read_accel_and_send(void);

extern "C" void __ISR(_TIMER_1_VECTOR, IPL7AUTO) Timer1Handler(void)
{
   gMillisecondsInOperation++;

   if (0 == gMillisecondsInOperation % 50)
   {
      PORTToggleBits(IOPORT_B, BIT_11);

      if (g_TCPIP_service_can_start)
      {
         // service the TCPIP stack
         add_function_to_queue(TCPIP_keep_stack_alive);
      }
   }

   if (0 == gMillisecondsInOperation % 100)
   {
      if (g_accel_init_good)
      {
         // read the accelerometer and send it out to the PC
         add_function_to_queue(read_accel_and_send);
      }
   }

   // clear the interrupt flag
   mT1ClearIntFlag();
}

void delayMS(unsigned int milliseconds)
{
   unsigned int millisecondCount = gMillisecondsInOperation;
   while ((gMillisecondsInOperation - millisecondCount) < milliseconds);
}

void read_accel_and_send(void)
{
   if (g_accel_init_good)
   {
      char cls_message[20];
      ACCEL_DATA accel_data;

      my_i2c_handler& i2c_ref = my_i2c_handler::get_instance();
      i2c_ref.accel_read(I2C2, &accel_data);

      // the Z axis (the vertical axis) is not calibrated correctly, being
      // shifted 0.14 in the -Z direction, and it seems to be consistently off,
      // so magically account for it
      accel_data.Z += 0.14;



      snprintf(cls_message, CLS_LINE_SIZE, "x:%.2lf, y:%.2lf", accel_data.X, accel_data.Y);
      i2c_ref.CLS_write_to_line(I2C2, cls_message, 1);
      snprintf(cls_message, CLS_LINE_SIZE, "z:%.2lf", accel_data.Z);
      i2c_ref.CLS_write_to_line(I2C2, cls_message, 2);

      // only bother to send the data if there is a connection
      if (1 == TCPIP_is_there_a_connection_on_port(SERVER_PORT))
      {
         unsigned char tx_buffer[MESSAGE_BUFFER_SIZE_BYTES];
         memset(tx_buffer, 0, MESSAGE_BUFFER_SIZE_BYTES);

         // fill in the buffer with tightly packed accelerometer data
         float *f_ptr = (float *)tx_buffer;
         *f_ptr = accel_data.X;
         f_ptr++;
         *f_ptr = accel_data.Y;
         f_ptr++;
         *f_ptr = accel_data.Z;

         // send the accelerometer data to the PC
         //snprintf((char*)tx_buffer, MESSAGE_BUFFER_SIZE_BYTES, "X:%.2f, Y:%.2f, Z:%.2f", accel_data.X, accel_data.Y, accel_data.Z);
         TCPIP_basic_send(SERVER_PORT, tx_buffer, sizeof(ACCEL_DATA));
      }
   }
}

// -----------------------------------------------------------------------------
//                    Main
// -----------------------------------------------------------------------------
int main(void)
{
   int actual_pb_clock = 0;
   int generic_ret_val = 0;
   int byte_count = 0;
   UINT8 ip_1 = 0;
   UINT8 ip_2 = 0;
   UINT8 ip_3 = 0;
   UINT8 ip_4 = 0;
   char cls_message[CLS_LINE_SIZE + 1];
   unsigned char rx_buffer[MESSAGE_BUFFER_SIZE_BYTES];
   

   gMillisecondsInOperation = 0;
   g_TCPIP_service_can_start = FALSE;
   g_accel_init_good = FALSE;


   // do some cnfiguration to optimize the processor's performance with caching
   // and pipelining stuff
   // Note: This is not-well-documented black magic.  Just say, "Yes!" and
   // we'll move on.
   actual_pb_clock = SYSTEMConfig(SYSTEM_CLOCK, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

   // open the timer that will provide us with simple delay operations
   OpenTimer1(T1_OPEN_CONFIG, T1_TICK_PR);
   ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_2);

   // MUST be performed before initializing the TCPIP stack, which uses an SPI
   // interrupt to communicate with the wifi pmod
   INTEnableSystemMultiVectoredInt();
   INTEnableInterrupts();

   // ---------------------------- Setpu LEDs ---------------------------------
   PORTSetPinsDigitalOut(IOPORT_B, BIT_10 | BIT_11 | BIT_12 | BIT_13);
   PORTClearBits(IOPORT_B, BIT_10 | BIT_11 | BIT_12 | BIT_13);

   // initialize the I2C module and any pmods on it
   my_i2c_handler& i2c_ref = my_i2c_handler::get_instance();
   i2c_ref.I2C_init(I2C2, actual_pb_clock);
   i2c_ref.CLS_init(I2C2);
   snprintf(cls_message, CLS_LINE_SIZE, "I2C init good!");
   i2c_ref.CLS_write_to_line(I2C2, cls_message, 1);
   i2c_ref.accel_init(I2C2);
   snprintf(cls_message, CLS_LINE_SIZE, "accel good");
   i2c_ref.CLS_write_to_line(I2C2, cls_message, 1);
   g_accel_init_good = TRUE;

   // create the function queue
   function_queue_init();
   snprintf(cls_message, CLS_LINE_SIZE, "fnct queue good");
   i2c_ref.CLS_write_to_line(I2C2, cls_message, 1);


   //TCPIP_and_wifi_stack_init("my_wifi_network_ssid", "my_wifi_network_pass_phrase");
   TCPIP_and_wifi_stack_init("Christ-2.4", "Jesus is GOD!");
   snprintf(cls_message, CLS_LINE_SIZE, "TCPIP init good");
   i2c_ref.CLS_write_to_line(I2C2, cls_message, 1);
   g_TCPIP_service_can_start = TRUE;

   // wait for the wifi to connect
   while(1)
   {
      // I like to blink an LED to show how long this loop is executing
      // Note: This loop will get stuck afteronly a few loops because that is
      // when the TCPIP stack will start connecting to the network, and that
      // takes ~30 seconds, during which you should expect to not see this
      // light blinking.
      if ((gMillisecondsInOperation % 50) == 0)
      {
         PORTToggleBits(IOPORT_B, BIT_10);
      }

      // make sure to execute the function queue because the TCPIP servicing
      // function, which is what is responsible for connecting to a network, is
      // added to it via an interrupt
      execute_functions_in_queue();

      // break out and continue program once IP address stops being link local
      TCPIP_get_IP_address(&ip_1, &ip_2, &ip_3, &ip_4);
      if (ip_1 != 169 && ip_2 != 254)
      {
         // IP address is not link local, so assume that we are connected to
         // the router specified by the SSID in my TCPIP framework code
         break;
      }
      snprintf(cls_message, CLS_LINE_SIZE, "%d.%d.%d.%d", ip_1, ip_2, ip_3, ip_4);
      i2c_ref.CLS_write_to_line(I2C2, cls_message, 2);
   }

   // open a port on the connected network, and display the result (success or
   // fail) along with the new IP address
   generic_ret_val = TCPIP_open_socket(SERVER_PORT);
   if (generic_ret_val < 0)
   {
      // bad, so print a message and hang here forever (or reset or power loss,
      // whichever comes first)
      snprintf(cls_message, CLS_LINE_SIZE, "port %d fail on", SERVER_PORT);
      while(1);
   }
   else
   {
      snprintf(cls_message, CLS_LINE_SIZE, "port %d open on", SERVER_PORT);
   }
   i2c_ref.CLS_write_to_line(I2C2, cls_message, 1);

   // now display the new IP address
   // Note: The new IP address may be shorter, character-wise, than its default
   // address, so clear the CLS line before displaying the new one.
   memset(cls_message, (int)' ', CLS_LINE_SIZE);
   cls_message[CLS_LINE_SIZE - 1] = 0;
   i2c_ref.CLS_write_to_line(I2C2, cls_message, 2);
   snprintf(cls_message, CLS_LINE_SIZE, "%d.%d.%d.%d", ip_1, ip_2, ip_3, ip_4);
   i2c_ref.CLS_write_to_line(I2C2, cls_message, 2);

   // pause to show the new IP address
   delayMS(2000);

   // NOW begin your custom code
   while (1)
   {
      // give the LED a little leeway in which to trigger
      // Note: Given how long it may take to execute instructions in this loop,
      // don't check for exactly 50 millisecond intervals
      // (that is, X mod 50 == 0), but instead check if it is close to 0.
      if ((gMillisecondsInOperation % 50) < 2)
      {
         PORTToggleBits(IOPORT_B, BIT_10);
      }

      // ALWAYS do this on each loop to take care of anything that pops up
      execute_functions_in_queue();

      // I like to blink different LEDs depending on whether there is a
      // connection or not, and I can get away with it because I am only using
      // a single socket-port
      // Note: Blinking LEDs would be a poor way to check the connection status
      // of a multiple socket-port setup.
      if (1 == TCPIP_is_there_a_connection_on_port(SERVER_PORT))
      {
         if ((gMillisecondsInOperation % 50) < 2)
         {
            PORTClearBits(IOPORT_B, BIT_12);
            PORTToggleBits(IOPORT_B, BIT_13);
         }
      }
      else
      {
         if ((gMillisecondsInOperation % 50) < 2)
         {
            PORTClearBits(IOPORT_B, BIT_13);
            PORTToggleBits(IOPORT_B, BIT_12);
         }
      }

      byte_count = TCPIP_bytes_in_RX_FIFO(SERVER_PORT);
      if (byte_count > 0)
      {
         // get the data out of the RX FIFO so it does clog it up, then ignore
         // the data
         byte_count = TCPIP_basic_receive(SERVER_PORT, rx_buffer, MESSAGE_BUFFER_SIZE_BYTES);
      }

      // delay for a little bit so that the LED doesn't blink too fast to see
      //delayMS(50);
   }

   return 0;
}


