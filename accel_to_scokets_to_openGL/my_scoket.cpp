#include "my_scoket.h"

#include <QtCore\QDebug>

#include <iostream>
using std::cout;
using std::endl;


// include the .cpp file made by the meta-object compiler for the header file 
// associated with this source file
#include "my_moc_source_files/moc_my_scoket.cpp"

my_scoket::my_scoket(QObject *parent) :
QObject(parent),
m_socket_ptr(0)
{
   // ??try initializing as an object??
   m_socket_ptr = new QTcpSocket(this);

   // connect to the the thing that you want to connect to
   // Note: In this case, it is my MX4cK microcontroller, which is connected to my
   // residence's wireless network.
   m_socket_ptr->connectToHost("10.10.10.126", 5);
   if (!(m_socket_ptr->waitForConnected(5000)))
   {
      cout << "could not connect" << endl;
      delete m_socket_ptr;
   }
   else
   {
      cout << "connected!" << endl;
   }

   // register the "read it" function to be called when the Qt application emits a
   // signal that says that serial data is ready to read
   QObject::connect(m_socket_ptr, SIGNAL(readyRead()), this, SLOT(read_it()));
}

my_scoket::~my_scoket()
{
   if (m_socket_ptr != 0)
   {
      // pointer points to something, so assume that it is an open socket, close it,
      // then delete the pointer to call the socket's destructor and free the 
      // memory
      m_socket_ptr->close();
      delete m_socket_ptr;
   }
}

void my_scoket::read_it()
{
   QByteArray rx_buff;
   qint64 bytes_available = m_socket_ptr->bytesAvailable();

   // ??check against 25 exactly??
   if (bytes_available > 0)
   {
      static float x = 0.0;
      static float y = 0.0;
      static float z = 0.0;

      rx_buff = m_socket_ptr->readAll();
      const char *c_ptr = rx_buff.data();
      //cout << "Reading '" << bytes_available << "' bytes: '" << c_ptr << "'" << endl;
      x += 1.1;
      y += 2.2;
      z += 3.3;

      emit done_reading_sig(x, y, z);


      //QString in_data;
      //in >> in_data;

      //qDebug() << in_data;
   }
   else
   {
      qDebug() << "no data";
   }
}

