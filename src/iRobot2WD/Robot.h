#pragma once

//Motor A
const int A_pin1 = 9;  // Pin 14 of L293
const int A_pin2 = 10;  // Pin 10 of L293

						//Motor B
const int B_pin3 = 6; // Pin  7 of L293
const int B_pin4 = 5;  // Pin  2 of L293


void ATurnForward() {
	digitalWrite(A_pin1, LOW);
	digitalWrite(A_pin2, HIGH);
}

void ATurnBackward() {
	digitalWrite(A_pin1, HIGH);
	digitalWrite(A_pin2, LOW);
}

void AStop() {
	digitalWrite(A_pin1, LOW);
	digitalWrite(A_pin2, LOW);
}

void BTurnForward() {
	digitalWrite(B_pin3, HIGH);
	digitalWrite(B_pin4, LOW);
}

void BTurnBackward() {
	digitalWrite(B_pin3, LOW);
	digitalWrite(B_pin4, HIGH);
}

void BStop() {
	digitalWrite(B_pin3, LOW);
	digitalWrite(B_pin4, LOW);
}

void robot_Move_Forward() {
	ATurnForward();
	BTurnForward();
	//delay();
}

void robot_Move_Backward() {
	ATurnBackward();
	BTurnBackward();
}

void robot_Turn_Left(int _delay) {
	ATurnBackward();
	BTurnForward();
	//delay determins the angle of turn
	delay(_delay);
}

void robot_Turn_Right(int _delay) {
	ATurnForward();
	BTurnBackward();
	//delay determins the angle of turn
	delay(_delay);
}

void robot_Stop() {
	AStop();
	BStop();
}

