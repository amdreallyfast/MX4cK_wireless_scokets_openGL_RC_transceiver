//#pragma comment (lib, "opengl32.lib")
//#pragma comment (lib, "C:/Qt/4.8.5/lib/QtCored4.lib")
//#pragma comment (lib, "C:/Qt/4.8.5/lib/QtGuid4.lib")
//#pragma comment (lib, "C:/Qt/4.8.5/lib/QtOpenGLd4.lib")
//#pragma comment (lib, "C:/open_gl_stuff/glew-1.10.0/lib/Release/Win32/glew32.lib")

#include <C:/Qt/4.8.5/include/Qt/qapplication.h>

// I MUST include my socket stuff, which includes some winsock stuff, before I include QT's GL widget, which apparently also includes some winsock stuff.
// I don't know what they did, but QT's way of including winsock stuff does not work if I include it before I include my winsock stuff.
#include "scokets.h"
#include "my_GL_window.h"


#include <iostream>
using std::cout;
using std::endl;

static const unsigned int BUFF_SIZE = 512;
static const unsigned int MAX_ACCEL_READS = 12;

int main(int argc, char **argv)
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


   //int app_return_val = 0;

   //QApplication app(argc, argv);

   //my_GL_wingdow window_for_doing_gl_stuff;
   //window_for_doing_gl_stuff.show();

   //app_return_val = app.exec();

   //return app_return_val;
   return 0;
}