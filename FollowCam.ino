#include <Wire.h>
#define BAUD_RATE 19200
#define CHAR_BUF 128
#include <Servo.h>
int ledIndicatorPin = 11; // select the pin for the LED
int pos_value_1 = 0;
int pos_value_2 = 0;
int pos_value_3 = 0;
int X_value = 0; // center x
int Y_value = 0; // center Y
int box_value = 0; // Size box (distance)
int x_pos = 90;
int y_pos = 90;
Servo servo_x;
Servo servo_y;
void setup() {
pinMode(ledIndicatorPin, OUTPUT);
servo_x.attach(8);
servo_y.attach(9);
Serial.begin(BAUD_RATE);
Serial.print("Test");
Wire.begin();
delay(1000); // Give the OpenMV Cam time to bootup.
}
void loop() {
int32_t temp = 0;
char buff[CHAR_BUF] = {0};
Wire.requestFrom(0x12, 2);

  if(Wire.available() == 2) { // got length?
    temp = Wire.read() | (Wire.read() << 8);
    delay(1); // Give some setup time...
    Wire.requestFrom(0x12, temp);
    if(Wire.available() == temp) { // got full message?
        temp = 0;
     while(Wire.available()) buff[temp++] = Wire.read();
        } else {
     while(Wire.available()) Wire.read(); // Toss garbage bytes.
        }
     } else {
     while(Wire.available()) Wire.read(); // Toss garbage bytes.
    }
String mystring(buff); // convert to string
//---------------------------------------------------------------------------------------------- POS 1 (X)
if (mystring.startsWith("POS_1")) {// check start of string
//Serial.println("POS 1 detected");
String pos1 = mystring.substring(8); // substring of last part, from position 8
pos_value_1 = pos1.toInt(); // substring to integer value
//Serial.println(pos_value_1);
}
//----------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------- POS 2 (Y)
if (mystring.startsWith("POS_2")) // check start of string
{
//Serial.println("POS 2 detected");
String pos2 = mystring.substring(8); // substring of last part, from position 8
pos_value_2 = pos2.toInt(); // substring to integer value
//Serial.println(pos_value_2);
}
//----------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------- POS 3 = POS 4 (L + H )
if (mystring.startsWith("POS_3")) // check start of string
{
//Serial.println("POS 3 detected");
String pos3 = mystring.substring(8); // substring of last part, from position 8
box_value = pos3.toInt(); // substring to integer value
//Serial.println(box_value);
}
//----------------------------------------------------------------------------------------------
//*********************************************************************************************************
X_value = pos_value_1 + (box_value / 2);
Y_value = pos_value_2 + (box_value / 2);
delay(1); // Don't loop to quickly.
//*********************************************************************************************************
  if ((X_value < 135) && (X_value > 105)&&(Y_value < 95) && (Y_value > 65)) {// 125-115 en 85-75 --> Is in center
    digitalWrite (ledIndicatorPin, HIGH);
    // x_pos = 90;
    // servo_x.write(x_pos);
    // y_pos = 90;
    // servo_x.write(x_pos);
    } else {
      digitalWrite (ledIndicatorPin, LOW);
    }
    if (X_value >= 135) {// Head right
        move_right(); // move right
    //delay(15);
    }
    if (X_value <= 105) {// Head left
        move_left(); // move left
        //delay(15);
    }
    if (Y_value >= 95) {// Head down
        move_down(); //move down
        //delay(15);
    }
    if (Y_value <= 65) {// Head up
        move_up(); // move up
        //delay(15);
    }
  }

void move_right() {
  if (x_pos >= 21) {// max = 20
    x_pos = x_pos - 1;
    servo_x.write(x_pos);
  }
}
void move_left() {
  if (x_pos <= 159) {// max = 160 
    x_pos = x_pos + 1;
    servo_x.write(x_pos);
  }
}
void move_down() {
  if (y_pos >= 71) {// max = 70
    y_pos = y_pos - 1;
    servo_y.write(y_pos);
  }
}
void move_up() {
  if (y_pos <= 134) { //max set to 135
    y_pos = y_pos + 1;
    servo_y.write(y_pos);
  }
}
