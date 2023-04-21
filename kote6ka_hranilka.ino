#include <SoftwareSerial.h>
#include <String.h>
#include <avr/interrupt.h>  
#include <Stepper.h>
#include <RTClib.h>
#include <Wire.h>

SoftwareSerial bt_serial(7, 6); // RX, TX for Bluetooth

// Define number of steps per rotation:
const int stepsPerRevolution = 2048;
Stepper myStepper(stepsPerRevolution, 2, 4, 3, 5);
RTC_DS1307 rtc;
int index_schedule;

void setup() 
{
  // Set the speed to 10 rpm:
  myStepper.setSpeed(14);
  bt_serial.begin(9600); // For Bluetooth
  Serial.begin(115200);

   //setup rtc
  Wire.begin();
  rtc.begin();
  Serial.println("in setup");
  index_schedule=0;
 
}
static int cnt=1;
int h=0;
void loop()
{
  while(h<=0 || h>24)
  {
    while(!bt_serial.available()); // Wait until data is received from Bluetooth module
     String input_str = bt_serial.readString();
      h = input_str.toInt();
      Serial.println("Received value from Bluetooth module: " + String(h));
      index_schedule=0;
  }
    if(index_schedule<6)
    {
      delay(h*1000);
    //delay(h*3600000);
      if (cnt%6==0)
      {
       bt_serial.println("there is no more food");
       myStepper.step(stepsPerRevolution/6+2);
       h=0;
      }
      else
        myStepper.step(stepsPerRevolution/6);
        index_schedule++;
        cnt++;
    }
}
