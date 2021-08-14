// Include Library
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include "bitmap.h"

#define Xaxis A1
#define pinButton 3
#define limitSwitchLeft  9

#define OLED_RESET 4

#define X_dirPin 2
#define X_stepPin 5

#define Z_dirPin 4
#define Z_stepPin 7

#define SSD1306_LCDHEIGHT 64

#define motorInterfaceType 1

AccelStepper myStepper1(motorInterfaceType, X_stepPin, X_dirPin);
AccelStepper myStepper2(motorInterfaceType, Z_stepPin, Z_dirPin);

MultiStepper steppers;

Adafruit_SSD1306 display(OLED_RESET);

int rotationdirection = 0;
volatile bool turnDetected = false;
unsigned long lastClick = 0;
int flag = 0;

int XstartPoint;
int YstartPoint;
long XendPoint;
long YendPoint;

long goToPosition[2];
volatile long totaldistance = 0;

float setspeed;
float motorspeed;
float timeinsec;
float timeinmins;

void stepperposition(int n)
{
  myStepper1.setMaxSpeed(3000);
  myStepper1.setSpeed(200);
  myStepper1.setMaxSpeed(3000);
  myStepper1.setSpeed(200);

  //  Serial.print("Posizione: ");
  //  Serial.println(myStepper1.currentPosition());
  //  Serial.print("Valore SwitchL: ");
  //  Serial.println(digitalRead(limitSwitchLeft));

  if (turnDetected)
  {
    turnDetected = false;  // do NOT repeat IF loop until new rotation detected
    if (n == 1)
    {
      /*
          Se la direzione di rotazione è -1, devo andare indietro con il mio carrellino
          All' inizio la mia posizione sarà 100, poi 500 e cos+ via fino arrivare a 0,
          ad ogni interazione con il joystick vado indietro con il mio carrellino di 500 step.
          Per fare gli ultimi passi, nel caso avessi una distanza non multiplo di 500
          faccio un ciclo while per arrivare fino in cima.
      */
      if (rotationdirection == -1)
      {
        long numero1 = myStepper1.currentPosition() + 500;
        //        Serial.println(numero1);
        if (numero1 <= 0 )
        {
          myStepper1.move(500);
          while (myStepper1.distanceToGo() != 0)
          {
            myStepper1.setSpeed(3000);
            myStepper1.runSpeed();
          }
        }
        else
        {
          while (myStepper1.currentPosition() != 0)
          {
            myStepper1.setSpeed(3000);
            myStepper1.runSpeed();
          }
        }
      }

      if (rotationdirection == 1)
      {
        /*
           Se la direzione di rotazione è 1, devo andare avanti con il mio carrellino
           All' inizio la mia posizione sarà 0, poi 500 e cos+ via, ad ogni interazione con il joystick
           vado avanti con il mio carrellino di 500 step.
           Per fare gli ultimi passi, nel caso avessi una distanza non multiplo di 500
           faccio un ciclo while per arrivare fino in fondo alla distanza.
        */

        long numero = abs(myStepper1.currentPosition() - 500);
        //        Serial.println(numero);
        if (numero < 85000 )
        {
          myStepper1.move(-500);
          while (myStepper1.distanceToGo() != 0)
          {
            myStepper1.setSpeed(-3000);
            myStepper1.runSpeed();
          }
        }
        else
        {
          while (abs(myStepper1.currentPosition()) != 85000)
          {
            myStepper1.setSpeed(-3000);
            myStepper1.runSpeed();

          }
        }
      }
    }

    if (n == 2) {
      if (rotationdirection == -1) {

        myStepper2.move(50);
        while (myStepper2.distanceToGo() != 0) {
          myStepper2.setSpeed(1000);
          myStepper2.runSpeed();
        }
      }
      if (rotationdirection == 1) {

        myStepper2.move(-50);
        while (myStepper2.distanceToGo() != 0) {
          myStepper2.setSpeed(-1000);
          myStepper2.runSpeed();
        }
      }
    }
  }
}

void changeSection()
{
  if (millis() - lastClick > 500)
    flag = flag + 1;

  lastClick = millis();
}

void Home()
{
  myStepper1.setMaxSpeed(3000);
  myStepper1.setSpeed(200);

  //  Serial.println("Sto tornando alla home");

  if (digitalRead(limitSwitchLeft) == 1)
  {
    display.drawBitmap(0, 0, Homing, 128, 64, 1);
    display.display();
  }
  while (digitalRead(limitSwitchLeft) == 1)
  {
    myStepper1.setSpeed(3000);
    myStepper1.runSpeed();
  }

  delay(20);
  myStepper1.setCurrentPosition(0);
  myStepper1.moveTo(-500);
  while (myStepper1.distanceToGo() != 0)
  {
    myStepper1.setSpeed(-1000);
    myStepper1.runSpeed();
  }
  myStepper1.setCurrentPosition(0);
  display.clearDisplay();
}

void setup() {

  myStepper1.setMaxSpeed(3000);
  myStepper1.setSpeed(200);
  myStepper2.setMaxSpeed(3000);
  myStepper2.setSpeed(200);

  steppers.addStepper(myStepper1);
  steppers.addStepper(myStepper2);

  pinMode(limitSwitchLeft, INPUT);

  pinMode(pinButton, INPUT_PULLUP);
  attachInterrupt (digitalPinToInterrupt(pinButton), changeSection, RISING); // SW connected to D3
  //
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  //display Boot logo
  display.drawBitmap(0, 0, CamSlider, 128, 64, 1);
  display.display();
  delay(2000);
  display.clearDisplay();

  Home();
}

void joystickReading() {
  // Read new position
  int analog_in = analogRead(Xaxis);
  Serial.println(analog_in);

  if (analog_in < 400 || analog_in > 600)
  {
    turnDetected = true;
    if (analog_in < 400)
    {
      rotationdirection = 1;
    } else if (analog_in > 600)
    {
      rotationdirection = -1;
    }
  }
  else
  {
    turnDetected = false;
    rotationdirection = 0;
  }
}

void SetSpeed()
{
  display.clearDisplay();
  while ( flag == 6)
  {
    joystickReading();
    if (turnDetected)
    {
      turnDetected = false;  // do NOT repeat IF loop until new rotation detected
      if (rotationdirection == -1)
      {
        setspeed = setspeed + 30;
      }
      if (rotationdirection == 1)
      {
        setspeed = setspeed - 30;
        if (setspeed < 0)
        {
          setspeed = 0;
        }
      }

      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(30, 0);
      display.print("Speed");

      motorspeed = setspeed / 80;

      display.setCursor(5, 16);
      display.print(motorspeed);
      display.print(" mm/s");

      totaldistance = XendPoint - XstartPoint;
      if (totaldistance < 0)
      {
        totaldistance = totaldistance * (-1);
      }

      timeinsec = (totaldistance / setspeed);
      timeinmins = timeinsec / 60;

      display.setCursor(35, 32);
      display.print("Time");
      display.setCursor(8, 48);
      if (timeinmins > 1)
      {
        display.print(timeinmins);
        display.print(" min");
      }
      else
      {
        display.print(timeinsec);
        display.print(" sec");
      }
      display.display();
    }

    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(30, 0);
    display.print("Speed");

    motorspeed = setspeed / 80;

    display.setCursor(5, 16);
    display.print(motorspeed);
    display.print(" mm/s");

    totaldistance = XendPoint - XstartPoint;
    if (totaldistance < 0)
    {
      totaldistance = totaldistance * (-1);
    }
    else
    {

    }
    timeinsec = (totaldistance / setspeed);
    timeinmins = timeinsec / 60;
    display.setCursor(35, 32);
    display.print("Time");
    display.setCursor(8, 48);
    if (timeinmins > 1)
    {
      display.print(timeinmins);
      display.print(" min");
    }
    else
    {
      display.print(timeinsec);
      display.print(" sec");
    }
    display.display();
  }

}

void loop() {

  if (flag == 0) {

    display.clearDisplay();
    display.drawBitmap(0, 0, BeginSetup, 128, 64, 1);
    display.display();
    setspeed = 2000;
  }
  if (flag == 1) {
    //Move Xaxis start position
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(10, 28);
    display.println("Set X In");
    display.display();

    while (flag == 1) {
      joystickReading();
      stepperposition(1);
    }
    XstartPoint = myStepper1.currentPosition();
  }
  if (flag == 2) {
    //Move Yaxis start position
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(10, 28);
    display.println("Set Y In");
    display.display();

    while (flag == 2) {
      joystickReading();
      stepperposition(2);
    }

    myStepper2.setCurrentPosition(0);
    YstartPoint = myStepper2.currentPosition();
  }
  if (flag == 3) {
    //Move Xaxis end position

    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(10, 28);
    display.println("Set X Out");
    display.display();

    while (flag == 3) {
      joystickReading();
      stepperposition(1);
    }
    XendPoint = myStepper1.currentPosition();
  }
  if (flag == 4) {
    //Move Yaxis end position

    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(10, 28);
    display.println("Set Y Out");
    display.display();

    while (flag == 4) {
      joystickReading();
      stepperposition(2);
    }
    YendPoint = myStepper2.currentPosition();
    display.clearDisplay();

    //RItorna alla home facendo una specie di preview

    goToPosition[0] = XstartPoint;
    goToPosition[1] = YstartPoint;

    display.clearDisplay();
    display.setCursor(8, 28);
    display.println(" Preview  ");
    display.display();

    myStepper1.setMaxSpeed(3000);

    steppers.moveTo(goToPosition);
    steppers.runSpeedToPosition();

  }
  if (flag == 5) {
    display.clearDisplay();
    display.setCursor(8, 28);
    display.println("Speed");
    display.display();

  }
  if (flag == 6)
  {
    display.clearDisplay();


    SetSpeed();

  }

  if (flag == 7)
  {
    display.clearDisplay();
    display.setCursor(30, 27);
    display.println("Start");
    display.display();
  }

  if (flag == 8) {
    // Ora faccio procedo con lo slider vero e proprio

    display.clearDisplay();
    display.setCursor(20, 27);
    display.println("Running");
    display.display();

    goToPosition[0] = XendPoint;
    goToPosition[1] = YendPoint;

    myStepper1.setMaxSpeed(setspeed);
    steppers.moveTo(goToPosition);
    steppers.runSpeedToPosition();

    flag = flag + 1;
  }
  if (flag == 9) {
    // Stampero sull' oled che ho finito.
    display.clearDisplay();
    display.setCursor(24, 26);
    display.println("Finish");
    display.display();
  }
  if (flag == 10) {
    //Torno alla home
    Serial.println("Torno alla home");
    display.clearDisplay();

    Home();
    flag = 0;
  }

}
