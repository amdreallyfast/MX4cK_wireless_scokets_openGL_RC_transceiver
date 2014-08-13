#pragma once

// Note: This header MUST be included before the QT widget header.
// Unhappy header file conflicts happen if glew.h is not included first.
// Also Note: I am not actually using anything in glew.h in my version of the QT 
// window, but other places in my code DO use glew.h, so to keep the compiler 
// happy, I have to include it before the QT window widget.  It's extra code that
// I don't need, so hopefully the link will clean things up.
#include <glew-1.10.0\include\GL\glew.h>

#include <QtOpenGL\qglwidget>
#include <QtGui\qmouseevent>
#include <QtCore\qtimer>

#include "my_shape.h"
#include "my_camera.h"

#include <vector>


// use this class to gain access to our gateway to the openGL API,
// then override things as necessay and add your own functionality 
// on top of that
class my_GL_window : public QGLWidget
{
   Q_OBJECT

public:
   // this destructor will destroy all the shape objects that were made
   ~my_GL_window();

   void render_frame();


protected:
   // OVERRIDES

   // override the widget's initialization method
   void initializeGL();

   // override QGLWidget's automated painted event to do nothing
   void paintEvent();

   // override the widget's "paint GL" method, which causes the GL API to 
   // start the GL pipeline and draw to this window
   void paintGL();

   // override the widget's mouse and keyboard events to move the camera
   void mouseMoveEvent(QMouseEvent*);
   void mousePressEvent(QMouseEvent*);
   void mouseReleaseEvent(QMouseEvent*);
   void keyPressEvent(QKeyEvent*);

private:
   QTimer m_qt_timer;

   std::vector<my_shape *> m_shape_ptrs_vector;
   std::vector<my_shape *>::iterator m_shape_ptrs_vector_it;
   my_camera m_camera;

private slots:
   void timer_update(void);
   void receive_serial_data(float X, float Y, float Z);
};

