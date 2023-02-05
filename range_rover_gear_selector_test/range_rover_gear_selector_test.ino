#include "variant.h"
#include <due_can.h>

//Leave defined if you use native port, comment if using programming port
#define Serial SerialUSB

const int CAN_SPEED = 500000; // CAN bus speed in baud
const int SEND_DELAY = 20; // send message every 10ms
const int MESSAGE_LENGTH = 8; // 8 byte long message

int Scycle = 0x82;
int Fcycle = 0x90;
int b0 = 0x5C;
int b1 = 0x66;
int b4 = 0xFF;
int b5 = 0x7F ;
int b6 = 0x00;
int b7 = 0x80;
int counter = 0x82;
int counter1 = 0x00;

void setup()
{
  Serial.begin(115200);

  // Initialize CAN0 and set the baud rate
  Can0.begin(CAN_SPEED);
  Can0.watchFor();
}

void loop()
{
  static unsigned long lastTime = 0;


  if (Can0.available() > 0)
  {
    CAN_FRAME incoming;
    Can0.read(incoming);

    // Check if the incoming message has byte 2 equal to 0x00
    if (incoming.id = 0x312)
    {
      switch (incoming.data.bytes[2])
      {
        case 0x8F:// Park
          b0 = 0x5C;
          b1 = 0x66;
          b4 = 0xFF;
          b5 = 0x7F ;
          b6 = 0x00;
          b7 = 0x80;
          Scycle = 0x82;
          Fcycle = 0x90;
          counter1 = counter - 130;
          break;

        case 0x8E: // R
          b0 = 0x7C;
          b1 = 0x24;
          b4 = 0xFE;
          b5 = 0xFF ;
          b6 = 0x01;
          b7 = 0x00;
          Scycle = 0x03;
          Fcycle = 0x11;
          counter1 = counter - 3;
          break;

        case 0x8D: // N
          b0 = 0x7C;
          b1 = 0x25;
          b4 = 0xFD;
          b5 = 0xFF ;
          b6 = 0x02;
          b7 = 0x00;
          Scycle = 0x04;
          Fcycle = 0x12;
          counter1 = counter - 4;
          break;

          case 0x8C: // D
          b0 = 0x7C;
          b1 = 0x24;
          b4 = 0xFB;
          b5 = 0xFF;
          b6 = 0x04;
          b7 = 0x00;
          Scycle = 0x06;
          Fcycle = 0x14;
          counter1 = counter - 6;
          break;

          case 0x88: // S
          b0 = 0x7C;
          b1 = 0x24;
          b4 = 0xFA;
          b5 = 0xFF;
          b6 = 0x05;
          b7 = 0x00;
          Scycle = 0x07;
          Fcycle = 0x15;
          counter1 = counter - 7;
          break;

      }
    }
  }
  // Send a new message every SEND_DELAY milliseconds
  if (millis() - lastTime >= SEND_DELAY)
  {
    CAN_FRAME msg;
    msg.id = 0x3F3; // message ID
    msg.length = MESSAGE_LENGTH;
    msg.extended = 0; // standard frame format

    // Set the data in the message
    for (int i = 0; i < MESSAGE_LENGTH; i++)
    {
      msg.data.bytes[i] = 0;
    }
    msg.data.bytes[0] = b0;
    msg.data.bytes[1] = b1;
    msg.data.bytes[2] = counter1;
    msg.data.bytes[3] = counter;
    counter++;
    if (counter > Fcycle)
    {
      counter = Scycle;
    }
    msg.data.bytes[4] = b4;
    msg.data.bytes[5] = b5;
    msg.data.bytes[6] = b6;
    msg.data.bytes[7] = b7;

    //* Send the message
    Can0.sendFrame(msg);

    /*
      static int counter3 = 0x60;
      static int counter4 = 0x03;
      CAN_FRAME msg2;
      msg2.id = 0x2A0;
      msg2.length = 8;
      msg2.extended = 0; // standard frame format

      // Set the data in the second message
      msg2.data.bytes[0] = 0x00;
      msg2.data.bytes[1] = counter3;
      counter3++;
      if (counter3 > 0x6E)
      {
      counter3 = 0x60;
      }
      msg2.data.bytes[2] = counter4;
      counter4++;
      if (counter4 > 0x11)
      {
      counter4 = 0x03;
      }
      msg2.data.bytes[3] = 0x01;
      msg2.data.bytes[4] = 0x00;
      msg2.data.bytes[5] = 0xFE;
      msg2.data.bytes[6] = 0xFF;
      msg2.data.bytes[7] = 0x21;
      Can0.sendFrame(msg2);
    */
    // Record the time the message was sent
    lastTime = millis();


  }
}
