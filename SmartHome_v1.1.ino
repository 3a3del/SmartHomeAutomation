#include "Wire.h"                          //special dec for lcd
#include "LiquidCrystal_I2C.h"            //special dec for lcd
LiquidCrystal_I2C lcd(0x27,16,2);        //special dec for lcd
#include <Adafruit_LiquidCrystal.h>     //lcd library
Adafruit_LiquidCrystal lcd_1(0);       //special dec for lcd
bool maintheme = true;
int x = 1;                             //itiration counter 
unsigned long prevTime = millis();         //variable to store prev time


#include <Servo.h>                    //servo library
Servo myservo;                        //create servo object to control a servo
int pos = 0;                          //create servo object to control motion of servo
#define butin 1                       //inside button
#define butout 1                       //inside button
#define irin    1                      //declaration for IR Sensor inside home
#define irout    1                     //declaration for IR Sensor outside home
#define buzzer    1                    //declaration for Doorbell

#define tempsensor 1                   // Attach temp sensor to Arduino Analog Pin, The Tmep sensor float variable * 0.48828125;
#define motorpin1   1                  // Attach h-bridge pins to motor
#define motorpin2 1
#define motorpinen 1

#define ldr        1                   //ldr to detect if dark take over, leds outside turn on
#define ledout1 1
#define ledout2 1
#define ledout3 1
#define ledout4 1

#define gassensor 1                    // Attach Gas sensor to Arduino Analog Pin, (upon gas detection buzzer rings, leds blink and door opens)
int ledin = 1;                    

#define tvir 1                         //detection ir if someone is sitting on the sofa
#define tvled 1                        //Led acting as a tv to turn on in tvir detects someone

#define winir1 1                       //ir sensor on window to detect if someone enters through a window
#define winir2 1                       //ir sensor on window to detect if someone enters through a window

#define watersensor 1                  

void setup() {
  lcd.init();                      //initz lcd
  lcd.backlight();                 //initz lcd
  lcd.clear();

  myservo.attach(9);
  myservo.write(0);
  
  pinMode(butin,INPUT);
  pinMode(butout,INPUT);
  pinMode(irin,INPUT);
  pinMode(irout,INPUT);
  pinMode(buzzer,OUTPUT);
  
  pinMode(tempsensor,INPUT);
  pinMode(motorpin1,OUTPUT);
  pinMode(motorpin2,OUTPUT);
  pinMode(motorpinen,OUTPUT);

  pinMode(ldr,INPUT);
  pinMode(ledout1,OUTPUT);
  pinMode(ledout2,OUTPUT);
  pinMode(ledout3,OUTPUT);
  pinMode(ledout4,OUTPUT);

  pinMode(gassensor,INPUT);
  pinMode(ledin,OUTPUT);

  pinMode(winir1,INPUT);
  pinMode(winir2,INPUT);
  pinMode(tvir,INPUT);
  pinMode(tvled,OUTPUT);

  pinMode(watersensor,INPUT);
}

void opendoor()
{
  for(int i=pos;i<=90;i++)
  {
    myservo.write(i);
     pos = i;
  }
}

void closedoor()
{
  for(int i=pos;i>=0;i--)
  {
     myservo.write(i);
     pos = i;
  }
}

void Blink(int led)
{  
  digitalWrite(led,HIGH);
  delay(200);
  digitalWrite(led,LOW);
  delay(200);
}

void loop() {
//************************LCD*************************************************
  if(maintheme)
  {
    lcd.begin(16, 2);
    lcd.setCursor(3,0);
    lcd.print("Welcome To");
    lcd.setCursor(1,1);
    lcd.print("Our Smart Home!");
    maintheme = false;
  }
  
//*******************Opening the door when bell rings*************************
  int doorsignal = 0;
  int access = 0;
  int dooropened = 0;
  if(digitalRead(butout) == 1)
    doorsignal = 1;

  while(digitalRead(butout) == 1)
    digitalWrite(buzzer,HIGH);
  digitalWrite(buzzer,LOW);
  
  if(digitalRead(butin) == 1 && doorsignal == 1)
  {
    access = 1;
    doorsignal = 0;
  }

  if(access == 1 && digitalRead(irout) == 1)
  {
    opendoor();
    dooropened = 1;
  }

  if(digitalRead(irin) == 1 && digitalRead(irout) == 0 && dooropened == 1 && access == 1)
  {
    closedoor();
    dooropened = 0;
  }

//*************************Security on windows****************
  while(digitalRead(winir1) == 1 || digitalRead(winir2) == 1)
    digitalWrite(buzzer,HIGH);
  digitalWrite(buzzer,LOW);

//************************TV turns on when tvir reads*********
  if(digitalRead(tvir) == 1)
    digitalWrite(tvled,HIGH);
  else
    digitalWrite(tvled,LOW);
//*************************Fan and Temerature*****************

  float roomtemp = analogRead(tempsensor) * 0.48828125;
  int fanspeed = roomtemp;  //Needs Fix!!!!!!!!!!!!!
  analogWrite(motorpinen,fanspeed);
  if(roomtemp >= 20)
    digitalWrite(motorpin1,HIGH);
  else
    digitalWrite(motorpin1,LOW);

//************************ldr and leds outside***********************
  if(analogRead(ldr) >= 600)
  {
    digitalWrite(ledout1,HIGH);
    digitalWrite(ledout2,HIGH);
    digitalWrite(ledout3,HIGH);
    digitalWrite(ledout4,HIGH);
  }
  else
  {
    digitalWrite(ledout1,LOW);
    digitalWrite(ledout2,LOW);
    digitalWrite(ledout3,LOW);
    digitalWrite(ledout4,LOW);
  }  

//***********************Led, buzzer and door open when gas sensor detects*************
  while(analogRead(gassensor)>800)
  {
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("EVACUATE!!!");
    Blink(ledin);
    opendoor();
    digitalWrite(buzzer,HIGH);
    delay(1000);
    maintheme = true;
  }

//************************Water Sensor and slider*******************
  int dangerlvl=0;
  if(analogRead(watersensor)>dangerlvl){
    //Slidercode
  }
}
