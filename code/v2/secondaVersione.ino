// Include Library
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <AccelStepper.h>
#include <MultiStepper.h>

#include "costanti.h"
#include "funzioni.h"


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

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.setTextSize(1.5);
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.print("Camera Slider");
  display.display();
  delay(2000);


  Home();
}

void loop() {

  updateMainMenu();
  confirmed = false;                                //Both used to confirm button push to select mode
  menuPosition = 1;  //Encoder starts from 0, first menu option


  while (!confirmed)                                        //While the user has not confirmed the selection
  {
    joystickReading();
    if (rotationdirection == 1 and (menuPosition - 1) > 0) {
      menuPosition = menuPosition - 1;
    } else if (rotationdirection == -1 and (menuPosition + 1) < 3) {
      menuPosition = menuPosition + 1;
    }

    if (menuPosition != prevmenuPosition)                       //Update the display if the encoder position has changed
    {
      updateMainMenu();
      prevmenuPosition = menuPosition;
    }

  }

  if (menuPosition == 1) {
    runTrack();
  } else if (menuPosition == 2)
    loopTracking();

}

void runTrack() {
  flag = 0;

  if (flag == 0) {

    display.clearDisplay();

    display.setCursor(10, 28);
    display.print("Inizio");
    display.display();
    speedValue = 2000;
    delay(500);
    flag = flag + 1;
  }

  if (flag == 1) {
    //Move Xaxis start position
    display.clearDisplay();
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
    display.println("Preview");
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
    delay(250);
    flag = flag + 1;

  }
  if (flag == 6)
  {
    display.clearDisplay();
    chooseSpeed();
  }

  if (flag == 7)
  {
    display.clearDisplay();
    display.setCursor(30, 27);
    display.println("Start");
    display.display();
    flag = flag + 1;
  }

  if (flag == 8) {
    // Ora faccio procedo con lo slider vero e proprio

    display.clearDisplay();
    display.setCursor(20, 27);
    display.print("Running");
    display.display();

    goToPosition[0] = XendPoint;
    goToPosition[1] = YendPoint;

    myStepper1.setMaxSpeed(speedValue);

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
    flag = flag + 1;
  }
  if (flag == 10) {
    //Torno alla home
    display.clearDisplay();

    Home();
    flag = 0;
  }

}
void loopTracking() {
  flag = 0;



  if (flag == 0) {

    display.clearDisplay();
    display.print("Inizio");
    display.display();
    speedValue = 2000;
    flag = flag + 1;
  }
  if (flag == 1) {
    //Move Xaxis start position
    display.clearDisplay();
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
    display.println("Preview");
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
    flag = flag + 1;

  }
  if (flag == 6)
  {
    display.clearDisplay();
    chooseSpeed();
  }

  if (flag == 7)
  {
    display.clearDisplay();
    display.setCursor(30, 27);
    display.println("Start");
    display.display();
    flag = flag + 1;
  }

  if (flag == 8) {
    // Ora faccio procedo con lo slider vero e proprio
    confirmed = false;
    display.clearDisplay();
    display.setCursor(20, 27);
    display.print("Running Loop");
    display.setCursor(20, 37);
    display.print(confirmed);
    display.display();

    while (!confirmed) {
      goToPosition[0] = XendPoint;
      goToPosition[1] = YendPoint;

      myStepper1.setMaxSpeed(speedValue);
      steppers.moveTo(goToPosition);
      steppers.runSpeedToPosition();

      goToPosition[0] = XstartPoint;
      goToPosition[1] = YstartPoint;

      steppers.moveTo(goToPosition);
      steppers.runSpeedToPosition();
    }
  }
  if (flag == 9) {
    // Stampero sull' oled che ho finito.
    display.clearDisplay();
    display.setCursor(24, 26);
    display.println("Finish");
    display.display();
    flag = flag + 1;
  }
  if (flag == 10) {
    //Torno alla home
    display.clearDisplay();

    Home();
    flag = 0;
  }
}
