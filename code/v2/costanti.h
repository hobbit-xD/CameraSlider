
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
int menuPosition = 1;
int modeSelected = 0;
int prevmenuPosition = 0;
volatile bool turnDetected = false;
unsigned long lastClick = 0;
int flag = 0;
boolean confirmed = false;
long XstartPoint;
long YstartPoint;
long XendPoint;
long YendPoint;

int selected = 0;
long goToPosition[2];
long totaldistance = 0;

float setspeed;
float motorspeed;
float timeinsec;
float timeinmins;
