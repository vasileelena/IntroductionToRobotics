# ProjectName 
SecurityAlarm

# Description
Do you ever come home after a long day, and just want to relax in your cozy bed but wait...you see the closetdoor is wide open...oh no! Your sister came to your room AGAIN to borrow some clothes and left a total mess! This is so frustrating, isn't it? Now you have to clean instead of relaxing!

Or maybe, your mother cleaned your room and throwed away that paper you've been writing some great ideas on! Oh man, wouldn't it be so good if people couldn't come to your room when you're not there? 

Well, let's see, how do people protect their properties, their homes, their stores, and so on? 

That's right: with a security alarm! And since you've been taking the Introduction to Robotics course at University of Bucharest, Faculty of Mathematics and Informatics, why buy one when we can build it? 

# How does it work
The principle is simple: you have a device that features a:

-> LCD display on which you can see the state of the alarm (activated or not) and other options like changing the password

-> keypad for you to type your numeric password

-> PIR movement sensor to detect the movement near your door and

-> of course passive buzzer to ring the alarm when your mother comes to room to find your diary...I mean...to clean the room.

# Features
* The password has 4 digits, and is initially "0000";
* When changing the password, at first you must enter the old password; this way, the sistem "knows" that is you who's trying to change it;
* Changing the password has some features like deleting a digit('#') because your finger slipped and you pressed the wrong one or you just changed your mind;
* When changing the password, you can also cancel the procedure anytime, without saving the password you introduced so far (just press '*');
* After changing the password, the old password doesn't work anymore(obviously);
* When the sensor detects some movement and the alarm starts ringing, you have to enter the password to stop it and enter the main menu;


# Hardware components
* LCD display 16x2
* Keypad 4x3
* Passive buzzer
* Medium size breadboard
* Wires
* Potentiometer (for the LCD's contrast)
* PIR movement sensor
* Arduino Uno


# Video & Bill Of Materials(BOM) links:
>> Video link: https://drive.google.com/file/d/1brFCpf5uu0flclAtQS8q7z8u087UG8N0/view

>> BOM link: https://docs.google.com/spreadsheets/d/1Htry010sDG5Vxl1XxuDkIDsEU6a6pIBbHVVmY9l-o_E/edit#gid=1811352457

*This project was inspired from the tutorial below; I used it only for learning how the keypad works, and to see the project asambled and the principle behind.

>> Tutorial link: https://howtomechatronics.com/projects/arduino-security-alarm-system-project/


