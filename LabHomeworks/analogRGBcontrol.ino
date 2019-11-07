const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;

const int potRedPin = A0;
const int potGreenPin = A1;
const int potBluePin = A2;

int potRedValue = 0;
int redValue = 0;
int potGreenValue = 0;
int greenValue = 0;
int potBlueValue = 0;
int blueValue = 0;

void setup() {
  pinMode(potRedPin, INPUT);
  pinMode(redPin, OUTPUT);
  pinMode(potGreenPin, INPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(potBluePin, INPUT);
  pinMode(bluePin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  potRedValue = analogRead(potRedPin); 
  potGreenValue = analogRead(potGreenPin);
  potBlueValue = analogRead(potBluePin); 
  redValue = map(potRedValue, 0, 1023, 0, 255);
  greenValue = map(potGreenValue, 0, 1023, 0, 255);
  blueValue = map(potBlueValue, 0, 1023, 0, 255);
  setColor(redValue, greenValue, blueValue);
}

void setColor(int red, int green, int blue){
  analogWrite(redPin, red);
  Serial.println(red);
  Serial.println(green);
  Serial.println(blue);
  delay(100);
  analogWrite(greenPin, green);
  delay(100);
  analogWrite(bluePin, blue);
  delay(100);
}
