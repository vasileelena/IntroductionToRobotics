## Updated files:
### 1st Homework: 
**filename**: analogRGBcontrol.ino

**description**: controlling the RGB led with three potentiometers, each of them controlling one of the RGB colors' intensity.
### 2nd Homework:
**filename**: knockSensor.ino

**description** : using a buzzer as a knock sensor, it detects a knock (sensivity detection by choice); 5 seconds after the knock, a tone starts on another buzzer; the tone stops when a push button is pressed;
### 3rd Homework:
**filename**: 4digits7segmentJoystick.ino

**description** : controlling a 4 digit 7 segment display using a joystick: cycling through the 4 displays using the Y-axis with left and right moves, pressing the button to select the display we want to change and lock it, modifying the value of te digit using the X-axis with up and down moves; 

There are 3 states: locked in / selected (not locked in though) / not selected. When locked in (the button is pressed), the point of the locked display is always on; When selected, the point of the selected display is blinking; The others points are always off;
### 4th Homework:
**filename**: gameMenu.ino

**description** : create a menu in the LCD which you scroll through with the joystick. The menu
structure should be the following: 

**a.Start game** - showing the number of lives, the level and the score; The level increases by one every 5 seconds; After 10 seconds the game ends; The score = 3xLevel;

**b.HighScore** - initially 0; it updates everytime it is passed by the player;

**c.Settings** - shows the starting level value which can be modified with the Y axis of the joystick; 

video link: https://drive.google.com/file/d/1vFf9aAiRLY5u11_YVqrj6-NTd8xeLV52/view
