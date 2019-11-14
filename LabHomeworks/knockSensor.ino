const int knockSensorPin = A0;
const int buzzPin = 11;
const int buttonPin = 2;

int knockValue;
bool ok = 0;

const int threshold = 2;
const int frequency = 5000;
const long interval = 5000;

unsigned long currentTime;
unsigned long knockTime;

int buttonState;

void setup() {
  pinMode(knockSensorPin, INPUT);
  pinMode(buzzPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  knockValue = analogRead(knockSensorPin);
  currentTime = millis();
  
  if(knockValue > threshold){
    Serial.println("Knock!");
    knockTime = millis();
    ok = 1; // (we knocked)
  }
  buttonState = digitalRead(buttonPin);
  if(buttonState == LOW){
    noTone(buzzPin);
  }
  if(currentTime - knockTime >= interval && ok){
    tone(buzzPin, frequency);
    ok = 0;
  }
  delay(10);
}
