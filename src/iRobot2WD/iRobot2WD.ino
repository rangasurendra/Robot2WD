/*
 Name:		iRobot2WD.ino
 Created:	4/10/2017 4:03:47 PM
 Author:	rwi
*/

#include <Servo.h>.
#include "Robot.h".

//Ultrasound
const int US_trigPin = 11;
const int US_echoPin = 3;

//Servo
Servo SRV_Motor;
const int angle_Offset = 15; // 180 / 12
const int angle_Origin = 90; // 0 is left max postion and 180 is right max position, 90 is the middle
const int safeDistance = 20;
const float velocity_of_sound = 340; // Meters per second
//variables
bool isBlocked = false;
bool critical = false;

enum Direction { LeftMost, HighLeft, Left, Middle, Right, HighRight, RightMost, None };

Direction blockedDirection = None;

bool isRoadBlocked(int _safeDistance, enum Direction _direction);
bool isRoadBlocked(int _safeDistance, enum Direction _direction) {
	delay(100);
	switch (_direction)
	{
	case LeftMost:
		SRV_Motor.write(angle_Origin + (angle_Offset * 3));
		Serial.print("LeftMost:");
		Serial.print(angle_Origin + (angle_Offset * 3));
		break;
	case HighLeft:
		SRV_Motor.write(angle_Origin + angle_Offset * 2);
		Serial.print("HighLeft:");
		Serial.print(angle_Origin + angle_Offset * 2);
		break;
	case Left:
		SRV_Motor.write(angle_Origin + angle_Offset * 1);
		Serial.print("Left:");
		Serial.print(angle_Origin + angle_Offset * 1);
		break;
	case Middle:
		SRV_Motor.write(angle_Origin + angle_Offset * 0);
		Serial.print("Middle:");
		Serial.print(angle_Origin + angle_Offset * 0);
		break;
	case Right:
		SRV_Motor.write(angle_Origin + angle_Offset * -1);
		Serial.print("Right:");
		Serial.print(angle_Origin + angle_Offset * -1);
		break;
	case HighRight:
		SRV_Motor.write(angle_Origin + angle_Offset * -2);
		Serial.print("HighRight:");
		Serial.print(angle_Origin + angle_Offset * -2);
		break;
	case RightMost:
		SRV_Motor.write(angle_Origin + angle_Offset * -3);
		Serial.print("RightMost:");
		Serial.print(angle_Origin + angle_Offset * -3);
		break;
	case None:
		Serial.print("None:");
		break;
	default:
		Serial.print("default:");
		break;
	}



	delay(60);//prevent trigger signal to be the echo signal
	digitalWrite(US_trigPin, LOW);
	delayMicroseconds(2);
	// Sets the trigPin on HIGH state for 10 micro seconds
	digitalWrite(US_trigPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(US_trigPin, LOW);

	float duration = pulseIn(US_echoPin, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds
	float distance = duration* (velocity_of_sound * 100 / 1000000) / 2; //unit converet to centimeters...
	critical = (distance < 20);

	Serial.print(" , distance:");
	Serial.print(distance);
	Serial.print(" , blocked:");


	if (_safeDistance > distance)
	{
		blockedDirection = _direction;
		Serial.println((_safeDistance > distance));
	}
	else
	{
		blockedDirection = Direction::None;
		Serial.println((_safeDistance > distance));
	}

	return (_safeDistance > distance);
}

void walkNaturally();
void walkNaturally() {
	int rand_int = rand() % 100 + 1;

	int elector = rand_int % 9;

	switch (elector)
	{
	case 1:
		robot_Turn_Left(400);
		break;
	case 2:
		robot_Turn_Right(400);
		break;
	case 3:
		robot_Move_Forward();
		break;
	case 4:
		robot_Move_Forward();
		break;
	case 5:
		robot_Move_Forward();
		break;
	case 6:
		robot_Move_Forward();
		break;
	case 7:
		robot_Move_Forward();
		break;
	case 8:
		robot_Move_Forward();
		break;
	case 9:
		robot_Move_Forward();
		break;
	default:
		robot_Move_Forward();
		break;
	}

}



// the setup function runs once when you press reset or power the board
void setup() {
	//Pin setup
	//Set pins as outputs
	pinMode(A_pin1, OUTPUT);
	pinMode(A_pin2, OUTPUT);
	pinMode(B_pin3, OUTPUT);
	pinMode(B_pin4, OUTPUT);


	pinMode(US_trigPin, OUTPUT); // Sets the trigPin as an Output
	pinMode(US_echoPin, INPUT); // Sets the echoPin as an Input
	Serial.begin(9600);

	pinMode(LED_BUILTIN, OUTPUT);

	//Setting Servo at look ahead position.
	SRV_Motor.attach(12);
	for (int servo_angle = 0; servo_angle <= 90; servo_angle++)
	{
		SRV_Motor.write(servo_angle);
		delay(10);
	}

	delay(5000);
}

// the loop function runs over and over again until power down or reset
void loop() {

	
	int number = rand() % 100 + 1;
	int fail_count = 0;
	while (
		isRoadBlocked(safeDistance, LeftMost) ||
		isRoadBlocked(safeDistance, HighLeft) ||
		isRoadBlocked(safeDistance, Left) ||
		isRoadBlocked(safeDistance, Middle) ||
		isRoadBlocked(safeDistance, Right) ||
		isRoadBlocked(safeDistance, HighRight) ||
		isRoadBlocked(safeDistance, RightMost)
		)
	{
		//fail_count++;

		if (fail_count >= 3 || critical)
		{
			digitalWrite(LED_BUILTIN, HIGH);
			Serial.println("Status critical or failed three times...");
			robot_Stop();
			robot_Move_Backward();
			delay(500);
			robot_Turn_Left(1000);
			digitalWrite(LED_BUILTIN, LOW);

		}
		else
		{

			//approximate turning angle - 500ms = 180 degree turn

			switch (blockedDirection)
			{
			case LeftMost:
				robot_Turn_Right(200);
				break;
			case HighLeft:
				robot_Turn_Right(250);
				break;
			case Left:
				robot_Turn_Right(350);
				break;
			case Middle:
				if (number % 2 == 0)
					robot_Turn_Left(500);
				else
					robot_Turn_Right(500);
				break;
			case Right:
				robot_Turn_Left(350);
				break;
			case HighRight:
				robot_Turn_Left(250);
				break;
			case RightMost:
				robot_Turn_Left(200);
				break;
			case None:
				break;
			default:
				break;
			}
		}

		robot_Stop();
	}

	fail_count = 0;

	robot_Move_Forward();//Robot will continue to roll...
}
