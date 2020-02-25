#include <Keypad.h>
#include <LiquidCrystal.h>

const int RS = 9;
const int enable = A0;
const int d4 = 10;
const int d5 = 11;
const int d6 = 12;
const int d7 = 13;
const int pirPin = A5;
const int buzzerPin = A3;

LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);

bool debounce = false;
bool enterPass = false;
bool setPass = false;
bool changePass = false;
bool activated = false;
bool alarm = false;
bool firstTone = true;
bool start = true;
int lastDebounceTime = 0;
int currentTime;
int passIterator = 0;
int pirState = 0;
int toneStart = 0;
const byte rows = 4;
const byte cols = 3;
const int toneInterval = 300;
const int passLength = 4;
const int debounceDelay = 100;
char keyPressed;
char prevKeyPressed = NO_KEY;
char password[passLength] = {'0', '0', '0', '0'};
char lastPass[passLength] = {'0', '0', '0', '0'};
char keyMap[rows][cols] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[rows] = {8, 7, 6, 5};
byte colPins[cols] = {4, 3, 2};
Keypad myKeypad = Keypad( makeKeymap(keyMap), rowPins, colPins, rows, cols);

void readKeypad() {
  keyPressed = myKeypad.getKey();
  currentTime = millis();
  if (keyPressed != NO_KEY) {
    lastDebounceTime = millis();
  }
  if (keyPressed != NO_KEY) {
    if (keyPressed != prevKeyPressed) {
      debounce = true;
    }
    else if (keyPressed == prevKeyPressed && keyPressed != NO_KEY) {
      if (currentTime - lastDebounceTime > debounceDelay) {
        debounce = true;
      }
    }
  }
  prevKeyPressed = keyPressed;
}

void activateAlarm() {
  lcd.setCursor(0, 0);
  lcd.print("Activating...");
  for (int i = 9; i >= 0; i--) {
    lcd.setCursor(7, 1);
    lcd.print(i);
    delay(500);
  }
  lcdClearLine(0);
  lcdClearLine(1);
  lcd.setCursor(0, 0);
  lcd.print("Alarm Activated!");
  delay(2000);
  lcdClearLine(0);
  lcdClearLine(1);
}

void alarmSound() {
  if (firstTone) {
    tone(buzzerPin, 800);
    currentTime = millis();
    if (currentTime - toneStart >= toneInterval) {
      noTone(buzzerPin);
      toneStart = currentTime;
      firstTone = false;
    }
  }
  else {
    tone(buzzerPin, 400);
    currentTime = millis();
    if (currentTime - toneStart >= toneInterval) {
      noTone(buzzerPin);
      toneStart = currentTime;
      firstTone = true;
    }
  }
}

bool motionDetected() {
  bool ok = false;
  pirState = digitalRead(pirPin);
  if (pirState == HIGH) {// if motion is detected
    ok = true;
  }
  return ok;
}

bool checkPass() {
  bool ok = true;
  for (int i = 0; i < passLength; i++) {
    if (lastPass[i] != password[i]) {
      ok = false;
      lcd.setCursor(0, 0);
      lcd.print("Wrong Password!"); //we didn't introduced the correct password so we are not allowed to change it to a new one
      delay(500);
      lcdClearLine(0);
      lcdClearLine(1);
    }
  }
  return ok;
}

void welcomeFunction(){
  lcd.setCursor(0, 0);
  lcd.print("Welcome!");
  delay(1000);
  lcdClearLine(0);
  lcd.setCursor(0, 0);
  lcd.print("Vasile Elena");
  lcd.setCursor(0, 1);
  lcd.print("@UniBucRobotics");
  delay(2000);
}

void introducePassword() {
  readKeypad();
  if (keyPressed != NO_KEY && debounce) {
    if (keyPressed == '#') { //deleting a number we introduced
      if (passIterator > 0) {
        passIterator--;
        lcd.setCursor(passIterator, 1);
        lcd.print(" ");
        debounce = false;
      }
    }
    else if (keyPressed == '*') { //option to stop introducing the password
      for (int i = 0; i < passLength; i++) {
        password[i] = lastPass[i]; //setting the password back to the last password so we don't have the partially introduced password as the new one
      }
      passIterator = passLength;
      setPass = false; //exit the setting/changing/entering password mode if it's the case
      changePass = false;
      enterPass = false;
      debounce = false;
    }
    else {
      if (setPass || changePass) {
        lcd.print(keyPressed); //if in the setting/changing password mode, show the numbers
      }
      else {
        lcd.print('*'); //if in the introducing password to activate/deactivate the alarm, don't show the numbers
      }
      password[passIterator] = keyPressed;
      passIterator++;
      debounce = false;
    }
  }
  if (passIterator == passLength) {
    delay(100); //so we get a view of the full password
    lcdClearLine(0); //clear the lcd for the next step
    lcdClearLine(1);
    if (setPass) {
      setPass = false;
      changePass = checkPass(); //we introduced the old pass, now we check if it's the right one so we can change it
    }
    else if (changePass) {
      for (int i = 0; i < passLength; i++) {
        lastPass[i] = password[i]; //update the password
      }
      changePass = false;
    }
    else if (enterPass && activated) { //trying to deactivate the alarm
      if (checkPass()) {
        enterPass = false;
        activated = false;
        alarm = false;
        noTone(buzzerPin);
        lcdClearLine(0);
        lcdClearLine(1);
        welcomeFunction();
      }
    }
    else if (enterPass) {
      activated = checkPass(); //for activating the alarm, we entered the password and now we have to check if it's correct
      if (activated) {
        activateAlarm();
        enterPass = false;
      }
    }
    passIterator = 0;
  }
}

void lcdClearLine(int line) {
  lcd.setCursor(0, line);
  for (int i = 0; i < 16; ++i) {
    lcd.write(' ');
  }
}

void homeScreen() {
  if (!setPass && !changePass && !activated && !enterPass) {
    lcdClearLine(0);
    lcdClearLine(1);
    lcd.setCursor(0, 0);
    lcd.print("1-Activate alarm");
    lcd.setCursor(0, 1);
    lcd.print("2-Change passwrd");
    readKeypad();
    if (keyPressed == '1') {
      enterPass = true;
      lcdClearLine(0);
      lcdClearLine(1);
    }
    else if (keyPressed == '2') {
      setPass = true;
      lcdClearLine(0);
      lcdClearLine(1);
    }
  }
  if (setPass || changePass) {
    lcd.setCursor(0, 0);
    if (setPass) {
      lcd.print(" Last Password: ");
    }
    else {
      lcd.print("  New Password: ");
    }
    lcd.setCursor(5, 1);
    lcd.print("#-del ");
    lcd.setCursor(11, 1);
    lcd.print("*-ext");
    lcd.setCursor(passIterator, 1);
    introducePassword();
  }
  else if (enterPass) {
    lcd.setCursor(0, 0);
    lcd.print("PASSWORD:");
    lcd.setCursor(passIterator, 1);
    if (alarm) {
      alarmSound();
    }
    introducePassword();
    
  }
  else if (activated) { //the alarm is activated
    lcd.setCursor(0, 0);
    if (motionDetected()) {
      alarm = true;
    }
    if (!alarm) {
      lcd.print("ALARM ACTIVATED");
    }
    else {
      lcd.print("  ACCES DENIED  ");
      alarmSound();
    }
    lcd.setCursor(0, 1);
    lcd.print("PRESS * TO LOGIN");
    readKeypad();
    if (keyPressed == '*') {
      enterPass = true;
      lcdClearLine(0);
      lcdClearLine(1);
    }
  }
}

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(pirPin, INPUT);
}

void loop() {
  if(start){
    welcomeFunction();
    start = false;
  }
  homeScreen();
}
