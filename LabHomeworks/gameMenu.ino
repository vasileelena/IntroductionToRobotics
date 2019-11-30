#include <LiquidCrystal.h>
const int RS = 12;
const int enable = 11;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;
LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);

int selected = 1;
int startingLevelValue = 0;
const int nextLvlInterval = 5000;
int currentMillis = 0;
int prevMillis = 0;
int highscore = 0;
bool ok = false;
bool endGame = 0;

const int pinSW = 13;
const int pinX = A0;
const int pinY = A2;

const int minThreshold = 400;
const int maxThreshold = 700;
int prevButtonValue = 1;
int buttonState = 0;
int switchValue;
int xValue = 0;
int yValue = 0;
bool xMoved = 0;
bool yMoved = 0;

void selectedFunction(int selected){
  lcd.setCursor(0, 0);
  if(selected == 1){
   lcd.print(">Start");
  }
  else {
   lcd.print(" Start");
  }
  lcd.setCursor(7, 0);
  if (selected == 2) {
    lcd.print(">Highscore");
  } else {
    lcd.print(" Highscore");
  }
  lcd.setCursor(4, 1);
  if (selected == 3) {
    lcd.print(">Settings");
  } else {
    lcd.print(" Settings");
  }
}

void lcdClearLine(int line) {
  lcd.setCursor(0, line);
  for (int i = 0; i < 16; ++i) {
    lcd.write(' ');
  }
}

void Start(){
      lcd.setCursor(0, 0);
      lcd.print("Lives:3");
      lcd.setCursor(8, 0);
      lcd.print("Level:");
      lcd.setCursor(14, 0);
      lcd.print(startingLevelValue);
      lcd.setCursor(0, 1);
      lcd.print("Score:");
      lcd.setCursor(6, 1);
      lcd.print(startingLevelValue * 3);
}

void Settings() {
  lcd.setCursor(0, 0);
  lcd.print("Starting level: ");
  lcd.setCursor(1, 1);
  lcd.print(startingLevelValue);
  
}

void Highscore() {
  lcd.setCursor(0, 0);
  lcd.print("Highscore:");
  lcd.setCursor(0, 1);
  lcd.print(highscore);
}

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(pinSW, INPUT_PULLUP);
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);
}

void loop() {
  switchValue = digitalRead(pinSW);
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  if(buttonState == 0){
  selectedFunction(selected);

  if(buttonState == 0){
  if(xValue > maxThreshold && !xMoved){
    selected++;
    if(selected > 3)
      selected = 1;
    xMoved = 1;
  }
  
  if(xValue < minThreshold && !xMoved){
    selected--;
    if(selected < 1)
      selected = 3;
    xMoved = 1;  
  }

   if(xValue > minThreshold && xValue < maxThreshold){
    xMoved = 0;
   }
}
}
  if(prevButtonValue != switchValue){
     if(switchValue == 0){
       buttonState = !buttonState;
       if(endGame == 1)
       endGame = 0;
     }
  }
  prevButtonValue = switchValue;
  if(buttonState == 1){
    if(selected == 1){
      //lcd.clear();
      if(!endGame){  
       lcdClearLine(0);
       lcdClearLine(1);  
      Start();
      currentMillis = millis();
      if(currentMillis - prevMillis > nextLvlInterval && !ok){
        startingLevelValue++;
        ok = true;
        prevMillis = currentMillis;
      }
      else if(currentMillis - prevMillis > nextLvlInterval && ok){
        startingLevelValue++;
        ok = false;
        if(startingLevelValue * 3 > highscore)  
          highscore = startingLevelValue * 3;
          endGame = 1;
          lcdClearLine(0);
          lcd.setCursor(0, 0);
          lcd.print("Game over");
          lcdClearLine(1);
          lcd.setCursor(0,1);
          lcd.print("Congratulations! ");
          delay(2000);
        }
      }
        else{ 
          lcdClearLine(0);
          lcd.setCursor(0, 0);
          lcd.print("Press the button");
          lcdClearLine(1);
          lcd.setCursor(0,1);
          lcd.print("to exit");
      }
  }
  else if(selected == 2){
    Highscore();
    prevMillis = millis();
  }
  else{
    Settings();
    if(yValue > maxThreshold && !yMoved){
    startingLevelValue++;
    yMoved = 1;
  }
  
  if(yValue < minThreshold && !yMoved){
    if(startingLevelValue > 0){
      startingLevelValue--;
      yMoved = 1;  
    }
  }

   if(yValue > minThreshold && yValue < maxThreshold){
    yMoved = 0;
   }
   prevMillis = millis(); 
  }
  lcdClearLine(0);
  lcdClearLine(1);  
 }    
}
