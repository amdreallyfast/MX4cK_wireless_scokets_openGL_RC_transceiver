#pragma once

#include <QtCore\QObject>
#include <QtNetwork\QTcpSocket>

class my_scoket : public QObject
{
public:
   explicit my_scoket(QObject *parent = 0);

   ~my_scoket();

   // Note: Visual Studio will complain about not being able to resolve this 
   // symbol, so you have to run moc.exe manually before trying to build so that VS
   // can find the definition.  The definition to the function is defined in the 
   // moc-generated source file for this header file.
   // Also Note: If you try to define it yourself in this header file, such as 
   // making it blank (that is, "{ }"), then the moc will not be able to register 
   // it as a signal, so at runtime you will get an error reading,
   // "no signal called my_scoket::done_reading_sig(float, float, float)", or
   // something like that.  If you try to give it a definition yorself in the 
   // source file though, you will get a "duplicate definition" error.
   Q_SIGNAL void done_reading_sig(float X, float Y, float Z);

private:
   Q_OBJECT

   Q_SLOT void read_it();

   QTcpSocket *m_socket_ptr;

};

