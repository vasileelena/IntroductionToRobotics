#include "LedControl.h"
#include <EEPROM.h>
#include <LiquidCrystal.h>

LedControl lc = LedControl(12, 11, 10, 1);

const int RS = 13;
const int enable = 3;
const int d4 = 4;
const int d5 = 5;
const int d6 = 6;
const int d7 = 7;
const int V0 = 9;

LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);

bool matrix[8][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

const uint64_t levelAnimation[] = { //level animation created using a website
  0x0000000000000000,
  0x7e1818181c181800,
  0x7e060c3060663c00,
  0x3c66603860663c00,
  0x30307e3234383000,
  0x3c6660603e067e00,
};
const int levelAnimation_LEN = sizeof(levelAnimation) / 8;

const uint64_t brokenHeart[] = {
  0x182c528991996600
};

const uint64_t restoreLives[] = {
  0x0000000000000000,
  0x1824428181996600,
  0x183c428181996600,
  0x183c7e8181996600,
  0x183c7eff81996600,
  0x183c7effff996600,
  0x183c7effffff6600
};

const int restoreLives_LEN = sizeof(restoreLives) / 8;


const int maxNum = 8;
const int xPin = A0;
const int yPin = A1;
const int swPin = 2;
const int minThreshold = 400;
const int maxThreshold = 700;

int xValue;
int yValue;
int row = 6;
int col = 4;

int level = 1;
int score = 0;
const int endLevelTime = 15000;
int levelInterval = 10000;
int startLevelTime = 0;
int lives = 3;
int deathTime;
const int deathAnimationInterval = 2000;
bool alive = 1;
bool endGame = 0;

int difficulty = 3; //1 = easy | 2 = medium | 3 = hard;
int currentTime;
const int fallIntervalEasy = 600;
const int fallIntervalMedium = 400;
const int fallIntervalHard = 200;
int lastFall = 0;

bool yMoved = false;
bool xMoved = false;

int selected = 1;
int settingsSelected = 1;
int prevSettingsButtonValue = 0;
int settingsButtonState = 1;
bool firstSettings = 1; //having a problem when entering the settings menu, the value changes imediatly to 1 and shows the selected menu instead of the settings menu
bool start = 0;
int highscore = 0;
int prevButtonValue = 1;
int buttonState = 0;
int switchValue;
bool welcome = 0;
int contrast = 50;
int moveInfo = 0;

void moveJoystick() {
  xValue = analogRead(xPin);
  yValue = analogRead(yPin);
  if (xValue > maxThreshold && !xMoved) {
    turnOffCar(col, row);
    if (col < 6) {
      col++;
    }
    xMoved = true;
  }

  if (xValue < minThreshold && !xMoved) {
    turnOffCar(col, row);
    if (col > 0) {
      col--;
    }
    xMoved = true;
  }

  if (xValue >= minThreshold && xValue <= maxThreshold) {
    xMoved = false;
  }

  if (yValue < minThreshold && !yMoved) {
    turnOffCar(col, row);
    fall();
    loseLife();
    scoreFunction();
    fall();
    loseLife();
    scoreFunction();
    yMoved = true;
  } //fast forward

  if (yValue >= minThreshold && yValue <= maxThreshold) {
    yMoved = false;
  }
}

void turnOffCar(int col, int row) {
  lc.setLed(0, row, col, false);
  lc.setLed(0, row + 1, col, false);
  lc.setLed(0, row + 1, col + 1, false);
  lc.setLed(0, row + 1, col - 1, false);
}

void moveCar(int col, int row) {

  lc.setLed(0, row, col, true);
  lc.setLed(0, row + 1, col, true);
  lc.setLed(0, row + 1, col + 1, true);
  lc.setLed(0, row, col + 1, true);
}

void clearMatrix() {
  for (int i = 0; i < maxNum; i++)
    for (int j = 0; j < maxNum; j++) {
      matrix[i][j] = 0;
    }
}

void died(const uint64_t image) {
  for (int i = 0; i < maxNum; i++) {
    byte r = (image >> i * 8) & 0xFF;
    for (int j = 0; j < maxNum; j++)
      lc.setLed(0, i, j, bitRead(r, j));
  }
  clearMatrix();
}

void loseLife() {

  if ((matrix[row][col] == 1) || (matrix[row + 1][col + 1] == 1) || (matrix[row + 1][col] == 1) || (matrix[row][col + 1] == 1)) {
    lives--;
    alive = 0;
    deathTime = millis();
  }
  if (lives == 0) {
    level = 1;
    lives = 3;
  }
}

void obstacleLvl(int level) {
  bool ok = true; //checking if the first 3 lines are empty, so there will be enough space for the car to avoid the obstacles
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < maxNum; j++) {
      if (matrix[i][j] != 0)
        ok = false;
    }
  if (ok) {
    int randCol;
    switch (level) {
      case 1: randCol = random(0, 7);
        if (randCol % 2) {
          lc.setLed(0, 0, randCol, true);
          matrix[0][randCol] = 1;
          lc.setLed(0, 1, randCol + 1, true);
          matrix[1][randCol + 1] = 1;
        }
        else {
          lc.setLed(0, 0, randCol, true);
          matrix[0][randCol] = 1;
          lc.setLed(0, 1, randCol + 1, true);
          matrix[0][randCol + 1] = 1;
        }
        break;
      case 2: randCol = random(0, 6);
        if (randCol % 2) {
          lc.setLed(0, 0, randCol, true);
          matrix[0][randCol] = 1;
          lc.setLed(0, 0, randCol + 1, true);
          matrix[0][randCol + 1] = 1;
          lc.setLed(0, 1, randCol - 1, true);
          matrix[1][randCol - 1] = 1;
        }
        else {
          lc.setLed(0, 0, randCol, true);
          matrix[0][randCol] = 1;
          lc.setLed(0, 0, randCol + 1, true);
          matrix[0][randCol + 1] = 1;
        }
        break;
      case 3: randCol = random(0, 5);
        if (randCol % 2) {
          lc.setLed(0, 0, randCol, true);
          matrix[0][randCol] = 1;
          lc.setLed(0, 0, randCol + 1, true);
          matrix[0][randCol + 1] = 1;
          lc.setLed(0, 1, randCol + 1, true);
          matrix[1][randCol + 1] = 1;
        }
        else {
          if (randCol == 3) {
            lc.setLed(0, 0, randCol, true);
            matrix[0][randCol] = 1;
            lc.setLed(0, 0, randCol + 1, true);
            matrix[0][randCol + 1] = 1;
            lc.setLed(0, 0, randCol + 2, true);
            matrix[0][randCol + 2] = 1;
          }
          else {
            lc.setLed(0, 0, randCol, true);
            matrix[0][randCol] = 1;
            lc.setLed(0, 0, randCol + 1, true);
            matrix[0][randCol + 1] = 1;
            lc.setLed(0, 1, randCol + 2, true);
            matrix[1][randCol] = 1;
          }
        }
        break;
      case 4: randCol = random(0, 4);
        if (randCol == 2) {
          lc.setLed(0, 0, randCol, true);
          matrix[0][randCol] = 1;
          lc.setLed(0, 0, randCol + 1, true);
          matrix[0][randCol + 1] = 1;
          lc.setLed(0, 0, randCol + 2, true);
          matrix[0][randCol + 2] = 1;
          lc.setLed(0, 0, randCol + 3, true);
          matrix[0][randCol + 3] = 1;
        }
        else if (randCol == 3) {
          lc.setLed(0, 0, randCol, true);
          matrix[0][randCol] = 1;
          lc.setLed(0, 0, randCol + 1, true);
          matrix[0][randCol + 1] = 1;
          lc.setLed(0, 0, randCol + 2, true);
          matrix[0][randCol + 2] = 1;
          lc.setLed(0, 1, randCol + 2, true);
          matrix[1][randCol + 2] = 1;
        }
        else {
          lc.setLed(0, 0, randCol, true);
          matrix[0][randCol] = 1;
          lc.setLed(0, 0, randCol + 1, true);
          matrix[0][randCol + 1] = 1;
          lc.setLed(0, 0, randCol + 2, true);
          matrix[0][randCol + 2] = 1;
          lc.setLed(0, 1, randCol, true);
          matrix[1][randCol] = 1;
        }
        break;
      case 5: randCol = random(0, 4);
        if (randCol == 2) {
          lc.setLed(0, 0, randCol, true);
          matrix[0][randCol] = 1;
          lc.setLed(0, 0, randCol + 1, true);
          matrix[0][randCol + 1] = 1;
          lc.setLed(0, 0, randCol + 2, true);
          matrix[0][randCol + 2] = 1;
          lc.setLed(0, 0, randCol + 3, true);
          matrix[0][randCol + 3] = 1;
          lc.setLed(0, 1, randCol + 3, true);
          matrix[1][randCol + 3] = 1;
        }
        else if (randCol == 3) {
          lc.setLed(0, 0, randCol, true);
          matrix[0][randCol] = 1;
          lc.setLed(0, 0, randCol + 1, true);
          matrix[0][randCol + 1] = 1;
          lc.setLed(0, 0, randCol + 2, true);
          matrix[0][randCol + 2] = 1;
          lc.setLed(0, 0, randCol + 3, true);
          matrix[0][randCol + 3] = 1;
          lc.setLed(0, 1, randCol, true);
          matrix[1][randCol] = 1;
        }
        else if (randCol == 4) {
          lc.setLed(0, 0, randCol, true);
          matrix[0][randCol] = 1;
          lc.setLed(0, 0, randCol + 1, true);
          matrix[0][randCol + 1] = 1;
          lc.setLed(0, 0, randCol + 2, true);
          matrix[0][randCol + 2] = 1;
          lc.setLed(0, 0, randCol + 3, true);
          matrix[0][randCol + 3] = 1;
          lc.setLed(0, 1, randCol + 2, true);
          matrix[1][randCol + 2] = 1;
        }
        else if (randCol == 1) {
          lc.setLed(0, 0, randCol, true);
          matrix[0][randCol] = 1;
          lc.setLed(0, 0, randCol + 1, true);
          matrix[0][randCol + 1] = 1;
          lc.setLed(0, 0, randCol + 2, true);
          matrix[0][randCol + 2] = 1;
          lc.setLed(0, 1, randCol + 1, true);
          matrix[1][randCol + 1] = 1;
          lc.setLed(0, 1, randCol + 2, true);
          matrix[1][randCol + 2] = 1;
        }
        else {
          lc.setLed(0, 0, randCol, true);
          matrix[0][randCol] = 1;
          lc.setLed(0, 0, randCol + 1, true);
          matrix[0][randCol + 1] = 1;
          lc.setLed(0, 0, randCol + 2, true);
          matrix[0][randCol + 2] = 1;
          lc.setLed(0, 1, randCol + 1, true);
          matrix[1][randCol + 1] = 1;
          lc.setLed(0, 1, randCol, true);
          matrix[1][randCol] = 1;
        }
        break;
      default: break;
    }
  }
}

void scoreFunction() {
  int scoreLevel = level + 1;
  bool passedObstacle = 0;
  for (int j = 0; j < maxNum && !passedObstacle; j++) {
    if (matrix[maxNum - 1][j] == 1 && matrix[maxNum - 2][j] != 1 && matrix[maxNum - 2][j - 1] != 1) {
      if (j != col && j != col + 1) {
        score += scoreLevel;
        passedObstacle = 1;
      }
    }
  }
  if (score > highscore) {
    highscore = score;
    EEPROM.put(0, highscore);
  }
}

void animation(uint64_t image) {
  for (int i = 0; i < maxNum; i++) {
    byte r = (image >> i * 8) & 0xFF;
    for (int j = 0; j < maxNum; j++) {
      lc.setLed(0, i, j, bitRead(r, j));
    }
  }
}

void fall() {
  for (int i = maxNum - 1; i > 0; i--) {
    for (int j = 0; j < maxNum; j++) {
      matrix[i][j] = matrix[i - 1][j];
      lc.setLed(0, i - 1, j, false);
      lc.setLed(0, i, j, matrix[i][j]);
    }
  }
  for (int j = 0; j < maxNum; j++) {
    matrix[0][j] = 0;
    lc.setLed(0, 0, j, false);
  }

}

void fallFrequency() {
  currentTime = millis();
  switch (difficulty) {
    case 1:  if (currentTime - lastFall >= fallIntervalEasy) {
        fall();
        loseLife();
        scoreFunction();
        lastFall = currentTime;
      }
      break;
    case 2:  if (currentTime - lastFall >= fallIntervalMedium) {
        fall();
        loseLife();
        scoreFunction();
        lastFall = currentTime;
      }
      break;
    case 3:  if (currentTime - lastFall >= fallIntervalHard) {
        fall();
        loseLife();
        scoreFunction();
        lastFall = currentTime;
      }
      break;
    default: break;
  }
}

void lcdClearLine(int line) {
  lcd.setCursor(0, line);
  for (int i = 0; i < 16; ++i) {
    lcd.write(' ');
  }
}

void Highscore() {
  lcd.setCursor(0, 0);
  lcd.print("Highscore:");
  lcd.setCursor(0, 1);
  int value = EEPROM.get(0, highscore);
  lcd.print(value);
  if (prevButtonValue != switchValue) {
    if (switchValue == 0) {
      buttonState = !buttonState;
    }
    prevButtonValue = switchValue;
  }
}

void Start() {
  lcd.setCursor(0, 0);
  lcd.print("Lives:");
  lcd.print(lives);
  lcd.setCursor(8, 0);
  lcd.print("Level:");
  lcd.setCursor(14, 0);
  lcd.print(level);
  lcd.setCursor(0, 1);
  lcd.print("Score:");
  lcd.setCursor(6, 1);
  lcd.print(score);
}

void settingsMenu() {
  lcd.setCursor(0, 0);
  if (settingsSelected == 1) {
    lcd.print(">Game Mode");
  }
  else {
    lcd.print(" Game Mode");
  }
  lcd.setCursor(11, 0);
  if (settingsSelected == 2) {
    lcd.print(">Back");
  } else {
    lcd.print(" Back");
  }
  lcd.setCursor(0, 1);
  if (settingsSelected == 3) {
    lcd.print(">Change contrast");
  } else {
    lcd.print(" Change contrast");
  }
}

void Settings() {
  xValue = analogRead(xPin);
  yValue = analogRead(yPin);
  Serial.println(settingsButtonState);
  if (settingsButtonState == 0) {
    settingsMenu();
    if (settingsButtonState == 0) {
      if (xValue > maxThreshold && !xMoved) {
        settingsSelected++;
        if (settingsSelected > 3)
          settingsSelected = 1;
        xMoved = 1;
      }
      if (xValue < minThreshold && !xMoved) {
        settingsSelected--;
        if (settingsSelected < 1)
          settingsSelected = 3;
        xMoved = 1;
      }
      if (xValue > minThreshold && xValue < maxThreshold) {
        xMoved = 0;
      }
    }
  }
  switchValue = digitalRead(swPin);
  if (prevSettingsButtonValue != switchValue) {
    if (switchValue == 0) {
      settingsButtonState = !settingsButtonState;
    }
    prevSettingsButtonValue = switchValue;
  }
}

void difficultySelect() {
  xValue = analogRead(xPin);
  yValue = analogRead(yPin);
  if (settingsButtonState == 1) {
    difficultyMenu();
    if (settingsButtonState == 1) {
      if (xValue > maxThreshold && !xMoved) {
        difficulty++;
        if (difficulty > 3)
          difficulty = 1;
        xMoved = 1;
      }
      if (xValue < minThreshold && !xMoved) {
        difficulty--;
        if (difficulty < 1)
          difficulty = 3;
        xMoved = 1;
      }
      if (xValue > minThreshold && xValue < maxThreshold) {
        xMoved = 0;
      }
    }
  }
  if (prevSettingsButtonValue != switchValue) {
    if (switchValue == 0) {
      settingsButtonState = !settingsButtonState;
    }
    prevSettingsButtonValue = switchValue;
  }
}

void difficultyMenu() {
  lcd.setCursor(0, 0);
  if (difficulty == 1) {
    lcd.print(">easy");
  }
  else {
    lcd.print(" easy");
  }
  lcd.setCursor(0, 8);
  if (difficulty == 2) {
    lcd.print(">medium");
  } else {
    lcd.print(" medium");
  }
  lcd.setCursor(9, 1);
  if (difficulty == 3) {
    lcd.print(">hard");
  } else {
    lcd.print(" hard");
  }

}

void Back() {
  lcdClearLine(0);
  lcdClearLine(1);
  buttonState = 0;
  settingsButtonState = 0;
}

void Contrast() {
  lcdClearLine(1);
  lcd.setCursor(0, 1);
  lcd.print("Contrast: < ");
  lcd.print(contrast);
  lcd.print(" >");
  xValue = analogRead(xPin);
  if (xValue > maxThreshold && !xMoved) {
    if (contrast < 150) {
      contrast += 10;
    }
    xMoved = 1;
  }
  if (xValue < minThreshold && !xMoved) {
    if (contrast > 0) {
      contrast -= 10;
    }
    xMoved = 1;
  }
  if (xValue > minThreshold && xValue < maxThreshold) {
    xMoved = 0;
  }
}

void Info() {
  xValue = analogRead(xPin);
  if (xValue < minThreshold) {
    if (xMoved == false) {
      if (moveInfo > 1) {
        moveInfo = 0;
      }
      else {
        moveInfo ++;
      }
      xMoved = true;
    }
  }
  else {
    xMoved = false;
  }
  if (moveInfo == 0) {
    lcd.setCursor(0, 0);
    lcd.print("<- Vasile Elena");
    lcd.setCursor(0, 1);
    lcd.print("Game:Racing Car");
  }
  else if (moveInfo == 1) {
    lcd.setCursor(0, 0);
    lcd.print("<-  GitHub:     ");
    lcd.setCursor(0, 1);
    lcd.print("bit.ly/2S3nV4m");
  }
  else {
    lcd.setCursor(0, 0);
    lcd.print(" @UniBucRobotics");
    lcd.setCursor(0, 1);
    lcd.print("   Thank you!   ");
  }
  if (prevButtonValue != switchValue) {
    if (switchValue == 0) {
      buttonState = !buttonState;
    }
    prevButtonValue = switchValue;
  }
}


void settingsSelectedFunction() {
  switch (settingsSelected) {
    case 1: difficultySelect();
      break;
    case 2: Back();
      break;
    case 3: Contrast();
      break;
    default: break;
  }
}

void selectedFunction() {
  switch (selected) {
    case 1: start = true;
      endGame = 0;
      startLevelTime = millis();
      break;
    case 2: Highscore();
      break;
    case 3:
      if (firstSettings) {
        settingsButtonState = 0;
        firstSettings = 0;
      }
      Settings();
      if (settingsButtonState == 1) {
        lcdClearLine(0);
        lcdClearLine(1);
        settingsSelectedFunction();
      }
      break;
    case 4: Info();
    default: break;
  }
}

void mainMenu(int selected) {
  lcd.setCursor(0, 0);
  if (selected == 1) {
    lcd.print(">Start");
  }
  else {
    lcd.print(" Start");
  }
  lcd.setCursor(7, 0);
  if (selected == 2) {
    lcd.print(">HighScore");
  } else {
    lcd.print(" HighScore");
  }
  lcd.setCursor(0, 1);
  if (selected == 3) {
    lcd.print(">Settings");
  } else {
    lcd.print(" Settings");
  }
  lcd.setCursor(11, 1);
  if (selected == 4) {
    lcd.print(">Info");
  } else {
    lcd.print(" Info");
  }
}

void menuSelect() {
  xValue = analogRead(xPin);
  yValue = analogRead(yPin);
  if (buttonState == 0) {
    mainMenu(selected);
    if (buttonState == 0) {
      if (xValue > maxThreshold && !xMoved) {
        selected++;
        if (selected > 4)
          selected = 1;
        xMoved = 1;
      }
      if (xValue < minThreshold && !xMoved) {
        selected--;
        if (selected < 1)
          selected = 4;
        xMoved = 1;
      }
      if (xValue > minThreshold && xValue < maxThreshold) {
        xMoved = 0;
      }
    }
  }
  if (prevButtonValue != switchValue) {
    if (switchValue == 0) {
      buttonState = !buttonState;
    }
    prevButtonValue = switchValue;
  }
}

void welcomeFunction() { //used only once through the boolean "welcome" variable
  lcd.print("    Welcome to ");
  lcd.setCursor(2, 1);
  lcd.print("Racing Car! ");
  delay(2000);
  lcdClearLine(0);
  lcdClearLine(1);
  welcome = true;
}

void congratsFunction() {
  lcdClearLine(0);
  lcdClearLine(1);
  lcd.setCursor(4, 0);
  lcd.print("Congrats!");
  for (int i = 0; i < 5; i++) {
    lcd.display();
    delay(200);
    lcd.noDisplay();
    delay(200);
  }
  lcd.display();
  lcd.setCursor(1, 0);
  lcd.print("Your score is:");
  lcd.setCursor(6, 1);
  lcd.print(score);
  delay(1500);
  int value = EEPROM.get(0, highscore);
  if (score == value) {
    lcd.setCursor(1, 0);
    lcd.print("New Highscore!!");
    for (int i = 0; i < 5; i++) {
      lcd.display();
      delay(200);
      lcd.noDisplay();
      delay(200);
    }
    lcd.display();
  }
  score = 0;
}

void gameOverFunction() {
  lcdClearLine(0);
  lcdClearLine(1);
  lcd.setCursor(4, 0);
  lcd.print("Game over!");
  for (int i = 0; i < 5; i++) {
    lcd.display();
    delay(200);
    lcd.noDisplay();
    delay(200);
  }
  lcd.display();
  for (int i = 0; i < restoreLives_LEN; i++) {
    animation(restoreLives[i]);
    delay(500);
  }
  congratsFunction();
}

void setup() {
  pinMode(swPin, INPUT_PULLUP);
  lc.shutdown(0, false);
  lc.setIntensity(0, 2);
  lc.clearDisplay(0);
  randomSeed(analogRead(2));
  Serial.begin(9600);
  lcd.begin(16, 2);
}

void loop() {
  analogWrite(V0, contrast);
  switchValue = digitalRead(swPin);
  if (!start) {
    if (!welcome) {
      welcomeFunction();
    }
    else {
      if (buttonState == 0) {
        menuSelect();
        lcdClearLine(0);
        lcdClearLine(1);
      }
      else {
        selectedFunction();
      }
    }
  }
  else {
    if (!endGame) {
      Start();
      if (level == 6) {
        endGame = 1;
        level = 0;
        start = 0;
        buttonState = 0;
        congratsFunction();
        for (int row = 0; row < 8; row++) {
          for (int col = 0; col < 8; col++) {
            lc.setLed(0, row, col, true);
            delay(25);
          }
        }
        for (int row = 0; row < 8; row++) {
          for (int col = 0; col < 8; col++) {
            lc.setLed(0, row, col, false);
            delay(25);
          }
        }
      }
      if (alive) {
        moveJoystick();
        moveCar(col, row);
        obstacleLvl(level);
        fallFrequency();
        currentTime = millis();
        if (currentTime - startLevelTime >= levelInterval) {
          level++;
          clearMatrix();
          if (level <= 5) {
            for (int i = 0; i < 2; i++) {
              animation(levelAnimation[level]);
              delay(1000);
              animation(levelAnimation[0]);
              delay(1000);
            }
            startLevelTime = millis();
          }
        }
      }
      else {
        died(brokenHeart[0]);
        currentTime = millis();
        if (currentTime - deathTime >= deathAnimationInterval) {
          alive = 1;
          if (lives == 3) {
            start = 0;
            buttonState = 0;
            gameOverFunction();
          }
          if (start) {
            for (int i = 0; i < 2; i++) {
              animation(levelAnimation[level]);
              delay(1000);
              animation(levelAnimation[0]);
              delay(1000);
            }
          }
          startLevelTime = millis();
        }
      }
    }
  }
}
