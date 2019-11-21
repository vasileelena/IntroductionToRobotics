const int pinA = 12;
const int pinB = 8;
const int pinC = 5;
const int pinD = 3;
const int pinE = 2;
const int pinF = 11;
const int pinG = 6;
const int pinDP = 4;
const int pinD1 = 7;
const int pinD2 = 9;
const int pinD3 = 10;
const int pinD4 = 13;

const int segSize = 8;
const int noOfDisplays = 4;
const int noOfDigits = 10;

int prevButtonValue = 1;
int lastDebounceTime = 0;
int debounceDelay = 50;


int stateDP = LOW;
int displayNo = 1;

const int segments[segSize] = {pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP};
const int digits[noOfDisplays] = {pinD1, pinD2, pinD3, pinD4};
const byte digitMatrix[noOfDigits][segSize - 1] = {
// a  b  c  d  e  f  g
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};
int values[4] = {0, 0, 0, 0};
int dots[4] = {0, 0, 0, 0};

const int interval = 500;
int lastBlink = 0;

const int pinSW = A2;
const int pinX = A0;
const int pinY = A1;

int switchValue;
int xValue = 0;
int yValue = 0;
int xMinThreshold = 500;
int xMaxThreshold = 550;
int yMinThreshold = 400;
int yMaxThreshold = 700;
bool xMoved = 0;
bool yMoved = 0;

void displayFunction(byte digit, byte decimalPoint){
  for(int i = 0; i < segSize - 1; i++){
    digitalWrite(segments[i], digitMatrix[digit][i]);
  }
 digitalWrite(segments[segSize - 1], decimalPoint);
}

void showDigit(int num){
  for(int i = 0; i < noOfDisplays; i++){
    digitalWrite(digits[i], HIGH);
  }
  digitalWrite(digits[num-1], LOW);
}

void setup() {
  for(int i = 0; i < segSize ; i++){
    pinMode(segments[i], OUTPUT);
  }
  for(int i = 0; i< noOfDisplays; i++){
    pinMode(digits[i], OUTPUT);
  }
  pinMode(pinSW, INPUT_PULLUP);
  Serial.begin(9600);
}

int buttonState = 0;

void loop() {
  
  switchValue = digitalRead(pinSW);
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);


if(buttonState == 0){
  if(yValue > yMaxThreshold && !yMoved){
    dots[displayNo - 1] = LOW;
    displayNo--;
    if(displayNo < 1)
      displayNo = 4;
    yMoved = 1;
  }
  
  if(yValue < yMinThreshold && !yMoved){
    dots[displayNo - 1] = LOW;
    displayNo++;
    if(displayNo > 4)
      displayNo = 1;
    yMoved = 1;  
  }

   if(yValue > yMinThreshold && yValue < yMaxThreshold){
    yMoved = 0;
    if(millis() - lastBlink >= interval){
      dots[displayNo - 1] = !dots[displayNo - 1];
      lastBlink = millis();
    }
   }
}

  if(prevButtonValue != switchValue){
     if(switchValue == 0){
       buttonState = !buttonState;
     }
  }
  prevButtonValue = switchValue;
  
   if(buttonState == 1){
    dots[displayNo - 1] = HIGH;
  if(xValue < xMinThreshold && !xMoved){
    values[displayNo - 1]--;
    if(values[displayNo - 1] < 0)
      values[displayNo - 1] = 9;
    xMoved = 1;
  }
  if(xValue > xMaxThreshold && !xMoved){
    values[displayNo - 1]++;
    if(values[displayNo - 1] > 9)
      values[displayNo - 1] = 0;
    xMoved = 1;
  }
   if(xValue > xMinThreshold && xValue < xMaxThreshold){
    xMoved = 0;
  }
  
 }
  showDigit(1);
  displayFunction(values[0], dots[0]);
  delay(5);
  showDigit(2);
  displayFunction(values[1], dots[1]);
  delay(5);
  showDigit(3);
  displayFunction(values[2], dots[2]);
  delay(5);
  showDigit(4);
  displayFunction(values[3], dots[3]);
  delay(5);
  

}
