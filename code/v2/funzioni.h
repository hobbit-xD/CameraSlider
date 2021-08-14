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
  else
    confirmed = true;

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


void joystickReading() {
  // Read new position
  int analog_in = analogRead(Xaxis);
  //Serial.println(analog_in);

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
      display.setTextSize(1);
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
    display.setTextSize(1);
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
}


void updateMainMenu()                               //Updates the display data for the main menu
{
  /*
    //display.clearDisplay();                           //Clear display
    //display.setTextSize(1);                           //Set the text size
    display.setCursor(28, 4);
    display.print("Camera Slider");

    display.display();
  */
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(28, 4);
  display.setTextColor(WHITE);
  display.print("Camera Slider");
  display.setCursor(25, 20);                        //Set the display cursor position
  display.print("Track Object");                          //Set the display text
  display.setCursor(25, 30);
  display.print("Loop");

  //Stores cursor vertical position to show selected item
  //Serial.println(menuPosition);

  if (menuPosition == 1)
    selected = 20;
  else if (menuPosition == 2)
    selected = 30;

  display.setCursor(14, selected);                   //Set the display cursor position
  display.print(">");
  display.display();                           //Output the display text
}
