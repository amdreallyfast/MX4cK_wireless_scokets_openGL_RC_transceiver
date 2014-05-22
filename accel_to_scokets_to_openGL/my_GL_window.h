#ifndef MY_GL_WINDOW_H
#define MY_GL_WINDOW_H

//#pragma once

#include <C:/Qt/4.8.5/include/QtOpenGL/qglwidget>
#include <C:/Qt/4.8.5/include/QtGui/qmouseevent>

#include "my_shape.h"
#include <vector>

// use this class to gain access to our gateway to the openGL API,
// then override things as necessay and add your own functionality 
// on top of that
class my_GL_window : public QGLWidget
{
protected:
   // OVERRIDES

   // override the widget's initialization method
   void initializeGL();

   // override the widget's "paint GL" method, which causes the GL API to 
   // start the GL pipeline and draw to this window
   void paintGL();


   // YOUR STUFF

   // this destructor will destroy all the shape objects that were made
   ~my_GL_window();

   void render_frame();


private:
   std::vector<my_shape *> m_shapes_vector;
};

#endif MY_GL_WINDOW_H