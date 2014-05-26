#include "my_scoket.h"

#include <QtCore\QDebug>

#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::string;

#define HOST_ADDR "10.10.10.126"
#define PORT_NUM 5

// include the .cpp file made by the meta-object compiler for the header file 
// associated with this source file
#include "my_moc_source_files/moc_my_scoket.cpp"

my_scoket::my_scoket(QObject *parent) :
QObject(parent),
m_scoket(this)
{
   // connect to the the thing that you want to connect to
   // Note: In this case, it is my MX4cK microcontroller, which is connected to my
   // residence's wireless network.
   m_scoket.connectToHost(HOST_ADDR, PORT_NUM);
   if (!(m_scoket.waitForConnected(5000)))
   {
      cout << "could not connect to host '" << HOST_ADDR << "' on port '" << PORT_NUM << "'" << endl;
   }
   else
   {
      cout << "connected to host '" << HOST_ADDR << "' on port '" << PORT_NUM << "'" << endl;
   }

   // register the "read it" function to be called when the Qt application emits a
   // signal that says that serial data is ready to read
   QObject::connect(&m_scoket, SIGNAL(readyRead()), this, SLOT(read_it()));
}

my_scoket::~my_scoket()
{
   m_scoket.close();
}

void my_scoket::read_it()
{
   QByteArray rx_buff;
   qint64 bytes_available = m_scoket.bytesAvailable();

   if (bytes_available > 0)
   {
      const float *f_ptr;
      float accel_x = 0.0f;
      float accel_y = 0.0f;
      float accel_z = 0.0f;

      // read the data and get a pointer to it
      rx_buff = m_scoket.readAll();
      const char *c_ptr = rx_buff.data();
      //cout << "Reading '" << bytes_available << "' bytes: '" << c_ptr << "'" << endl;
      f_ptr = reinterpret_cast<const float*>(c_ptr);
      
      // now extract the data and send it to the receiver, whoever it is
      // Note: The accelerometer data on the pmod has Z being the verical axis and
      // Y being a horizontal axis along with X.  OpenGL, on the other, considers
      // Y to be the vertical axis.  To account for this, simply, shove the Y data
      // from the microcontroller into the Z data and vice-versa for the Z data 
      // from the microcontroller.
      accel_x = *f_ptr;
      f_ptr++;
      accel_z = *f_ptr;
      f_ptr++;
      accel_y = *f_ptr;

      emit done_reading_sig(accel_x, accel_y, accel_z);


      //QString in_data;
      //in >> in_data;

      //qDebug() << in_data;
   }
   else
   {
      qDebug() << "no data";
   }
}

