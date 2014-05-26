
// I like to include the library files here so that they are clearly visible in the
// code and so that I don't have to set them up in the project build file.
// Note: I still have to specify "C:/open_gl_stuff", "C:/Qt/4.8.5/include", and
// "$(ProjectDir)" though in my project build file's "addition include directories" 
// option.  I don't know why.
// Note: Although it's dirty, I had to copy the dll files for QtCored4, QtGuid4, 
// and QtOpenGLd4 into my project's Debug folder in order to make the program work.
// The program will build without them, but it won't run without them.
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "C:/Qt/4.8.5/lib/QtCored4.lib")
#pragma comment (lib, "C:/Qt/4.8.5/lib/QtNetworkd4.lib")
#pragma comment (lib, "C:/Qt/4.8.5/lib/QtGuid4.lib")
#pragma comment (lib, "C:/Qt/4.8.5/lib/QtOpenGLd4.lib")
#pragma comment (lib, "C:/open_gl_stuff/glew-1.10.0/lib/Release/Win32/glew32.lib")

#include <QtGui\QApplication>
#include <QtCore\QObject>

// I MUST include my socket stuff, which includes some winsock stuff, before I include QT's GL widget, which apparently also includes some winsock stuff.
// I don't know what they did, but QT's way of including winsock stuff does not work if I include it before I include my winsock stuff.
#include "my_scoket.h"
#include "my_GL_window.h"

#include <iostream>
using std::cout;
using std::endl;

static const unsigned int BUFF_SIZE = 512;
static const unsigned int MAX_ACCEL_READS = 12;

// for vector typing
#include "my_glm_hpp_include.h"
#include <glm\glm\gtc\matrix_transform.hpp>

int main(int argc, char **argv)
{
   int app_return_val = 0;
   QApplication app(argc, argv);

   my_scoket S;
   my_GL_window window_for_doing_gl_stuff;
   //window_for_doing_gl_stuff.show();

   QObject::connect(
      &S,
      SIGNAL(done_reading_sig(float, float, float)),
      &window_for_doing_gl_stuff,
      SLOT(receive_serial_data(float, float, float)));

   app_return_val = app.exec();




   return app_return_val;
}