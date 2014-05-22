#ifndef MY_GL_WINDOW_H
#define MY_GL_WINDOW_H

//#pragma once

// Note: This header MUST be included before the QT widget header.
// Unhappy header file conflicts happen if glew.h is not included first.
// Also Note: I am not actually using anything in glew.h in my version of the QT 
// window, but other places in my code DO use glew.h, so to keep the compiler 
// happy, I have to include it before the QT window widget.  It's extra code that
// I don't need, so hopefully the link will clean things up.
#include <glew-1.10.0\include\GL\glew.h>
#include <C:/Qt/4.8.5/include/QtOpenGL/qglwidget>


#include <C:/Qt/4.8.5/include/QtGui/qmouseevent>

#include "my_shape.h"
#include "my_camera.h"

#include <vector>


// use this class to gain access to our gateway to the openGL API,
// then override things as necessay and add your own functionality 
// on top of that
class my_GL_window : public QGLWidget
{
public:
   // this destructor will destroy all the shape objects that were made
   ~my_GL_window();

   void render_frame();


protected:
   // OVERRIDES

   // override the widget's initialization method
   void initializeGL();

   // override the widget's "paint GL" method, which causes the GL API to 
   // start the GL pipeline and draw to this window
   void paintGL();

   // override the widget's "mouse move event"
   // this will ??do what??
   void mouseMoveEvent(QMouseEvent*);
   void mousePressEvent(QMouseEvent*);
   void mouseReleaseEvent(QMouseEvent*);

   // for keyboard inputs
   void keyPressEvent(QKeyEvent*);


private:
   std::vector<my_shape *> m_shape_ptrs_vector;
   std::vector<my_shape *>::iterator m_shape_ptrs_vector_it;
   my_camera m_camera;
};

#endif MY_GL_WINDOW_H