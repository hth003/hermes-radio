/*****************************************************************
hermes-radio.ino

Set up a software serial port to pass data between an XBee Shield
and the serial monitor.

Hardware Hookup:
  The XBee Shield makes all of the connections you'll need
  between Arduino and XBee. If you have the shield make
  sure the SWITCH IS IN THE "DLINE" POSITION. That will connect
  the XBee's DOUT and DIN pins to Arduino pins 2 and 3.
  The Adafruit GPS Break Out
  3 Pushbutton - buttonAlert, buttonSafe, buttonOff
  LED 
  Beeper

*****************************************************************/
// We'll use SoftwareSerial to communicate with the XBee:
#include <SoftwareSerial.h>

// XBee's DOUT (TX) is connected to pin 2 (Arduino's Software RX)
// XBee's DIN (RX) is connected to pin 3 (Arduino's Software TX)
const int buttonAlert = 4; // the number of the pushbutton pin
const int buttonSafe = 5;
SoftwareSerial XBee(2, 3); // RX, TX
const int buzzerPin = 12;
const int ledPin = 13;

void setup() {
  // put your setup code here, to run once:
  // Start the Arduino hardware serial port at 9600 baud
  Serial.begin(9600);
  Serial1.begin(9600);

  // Set up both ports at 9600 baud. This value is most important
  // for the XBee. Make sure the baud rate matches the config
  // setting of your XBee.
  XBee.begin(9600);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(ledPin, OUTPUT); // sets the digital pin as an output
  pinMode(buzzerPin, OUTPUT); // sets the digital pin as an output
  pinMode(buttonAlert, INPUT); // sets the digital pin as an input
  pinMode(buttonSafe, INPUT); // sets the digital pin as an input
}

void loop() {
  // put your main code here, to run repeatedly:
  // read the state of the pushbutton value:
  int buttonAlertState = digitalRead(buttonAlert);
  int buttonSafeState = digitalRead(buttonSafe);
  if (Serial1.available())
  { 
    Serial.write(Serial1.read());
    XBee.write(Serial1.read());
  }
  if (Serial.available())
  { // If data comes in from serial monitor, send it out to XBee
    XBee.write(Serial.read());
  }
  if (buttonAlertState == HIGH){
    Serial.print(F("Button Alert State:"));
    Serial.print(buttonAlertState);
    Serial.print("\n");
    XBee.write(buttonAlertState);
  }
  if (buttonSafeState == HIGH){
    Serial.print(F("Button Safe State:"));
    Serial.print(buttonSafeState);
    Serial.print("\n");
    digitalWrite(ledPin, HIGH); // turns the led on
    digitalWrite(buzzerPin, HIGH); // turns the buzzer on
    delay(5000); // waits 5 seconds
    digitalWrite(ledPin, LOW); // turns led off
    digitalWrite(buzzerPin, LOW); // turns the buzzer off
    delay(500); // waits half a second
    XBee.write(buttonSafeState);
  }  
  //Receive data from Hermes station
  if (XBee.available())
  {
    // If data comes in from XBee, send it out to serial monitor
    char getData = XBee.read();
    Serial.write(getData);
    if (getData == '1'){
      digitalWrite(ledPin, HIGH); // turns the led on
      digitalWrite(buzzerPin, HIGH); // turns the buzzer on
      delay(5000); // waits half a second
      digitalWrite(ledPin, LOW); // turns led off
      digitalWrite(buzzerPin, LOW); // turns the buzzer off
      delay(500); // waits half a second
    }
    else{
      digitalWrite(ledPin, LOW);
      digitalWrite(buzzerPin, LOW);
    }
  }
}


