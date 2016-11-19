#pragma config(Sensor, in1,    Gyro,           sensorGyro)
#pragma config(Sensor, dgtl1,  TL_EN,          sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  TR_EN,          sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  R_EN,           sensorQuadEncoder)
#pragma config(Sensor, dgtl7,  L_EN,           sensorQuadEncoder)
#pragma config(Motor,  port1,           Lift_Left,     tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           Top_Right,     tmotorVex393_MC29, openLoop, reversed, encoderPort, dgtl1)
#pragma config(Motor,  port3,           Top_Left,      tmotorVex393_MC29, openLoop, encoderPort, dgtl3)
#pragma config(Motor,  port4,           Bottom_Right,  tmotorVex269_MC29, openLoop, reversed, encoderPort, dgtl5)
#pragma config(Motor,  port5,           Bottom_Left,   tmotorVex393_MC29, openLoop, encoderPort, dgtl7)
#pragma config(Motor,  port6,           Front_Right,   tmotorVex393_MC29, openLoop, reversed, driveRight)
#pragma config(Motor,  port7,           Front_Left,    tmotorVex393_MC29, openLoop, driveLeft)
#pragma config(Motor,  port8,           Back_Right,    tmotorVex393_MC29, openLoop, reversed, driveRight)
#pragma config(Motor,  port9,           Back_Left,     tmotorVex393_MC29, openLoop, driveLeft)
#pragma config(Motor,  port10,          Lift_Right,    tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/* NOTE: All directional names are referring to side of object when viewing robot from the back */

#pragma platform(VEX)

// Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(60)
#pragma userControlDuration(0)

#include "Vex_Competition_Includes.c"

/////////////////////////////////////////////////////////////////////////////////////////
//
//                         						 NOTES
//
// 3 SQUARES: 1550 clicks
//
/////////////////////////////////////////////////////////////////////////////////////////


//************************************************ Autonomous Tasks ***********************************************//
///////////////////// SENSORS
void resetControllers() // Reset all (in use) sensors' values
{
	SensorValue[R_EN] = 0; // Right wheel base encoder
	SensorValue[L_EN] = 0; // Left wheel base encoder
	SensorValue[TR_EN] = 0; // Right lift encoder
	SensorValue[in1] =0; // Gyroscope
}

//////////////////// POPPER
void setPopper(short power) // Power the popper
{
	motor[Lift_Left]= power;
	motor[Lift_Right]= power;
}

void raisePopper(int duration) // Raise the popper
{
	setPopper(127);
	delay(duration);
	setPopper(0);
}

void lowerPopper(int duration) // Lower the popper
{
	setPopper(-127);
	delay(duration);
	setPopper(0);
}

//////////////////////// LIFT
void setLift(short power) // Power the lift
{
	motor[Bottom_Left] = power;
	motor[Bottom_Right] = power;
	motor[Top_Left] = power;
	motor[Top_Right] = power;
}

void raiseLift(int threshold) // Raise the lift
{
	resetControllers();
	while(abs(SensorValue[TR_EN]) < threshold) // While lift is within hight limit, raise lift
	{
		setLift(90);
	}
	setLift(0); //stop
}

void lowerLift(int threshold) // Lower the lift
{
	resetControllers();
	while(abs(SensorValue[TR_EN]) < threshold) // While lift is above ground limit, lower lift
	{
		setLift(-90);
	}
	setLift(0);
}

///////////////////////// WHEEL BASE
void setWheels(short leftpower, short rightpower) // Power the wheel base
{
	motor[Back_Left] = leftpower;
	motor[Back_Right]= rightpower;
	motor[Front_Left]= leftpower;
	motor[Front_Right]= rightpower;
}

void drive(float units, bool brake) // Distance measurement for wheel base
{
	short direction = 1; // Set direction (Positive = forward; Negative = backward)
	if(units < 0.0) direction = -1; // Set opposite direction if # of squares ("units") set is negative

	resetControllers();

	int threshold = (int)(abs(units)*(1550.0/3.0)); // Take absolute value for distance ("threshold")

	while(abs(SensorValue[L_EN]) < threshold) // While distance not achieved, move according to direction
	{
		setWheels(direction*127, direction*127);
	}

	if(brake) // If brake is set to true, quickly run motors in opposite direction to decrease momentum
	{
		setWheels(-direction*10, -direction*10);
		delay(200);
		setWheels(0, 0);
	}
}

//************************************************** PRE-AUTONOMOUS *******************************************//
void pre_auton()
{
	bStopTasksBetweenModes = true;

	// Reset encoders
	SensorValue[R_EN] = 0;
	SensorValue[L_EN] = 0;
	SensorValue[TR_EN] = 0;
	SensorValue[TL_EN] = 0;
}

//************************************************** AUTONOMOUS ***********************************************//
task autonomous() {
	/* Sets up the gyroscope. Gyro-controlled turns don't work right now, so this is commented out to save time.
	SensorType[in1] = sensorNone;
	delay(1000); // Mandatory wait to allow initialization
	SensorType[in1] = sensorGyro;
	*/

	/////////////////////// FIRST RUN (initial setup)
	drive(-0.6, true); //drive back
	delay(3000); //wait for driver load
	//raiseLift(100); //lift a bit: this is here in case we want to do 2 stars at once
	raiseLift(380); //raise lift all the way up
	delay(500); //delay for safety
	drive(-1.55, true); //drive back
	//raiseLift(300); //for 2 stars
	raisePopper(800); //pop over
	delay(500); //wait for star to fall out
	lowerPopper(300);
	lowerLift(350); //lower lift

	//////////////////////// SECOND RUN
	drive(1.55, true); //go back
	delay(3000); //wait for driver load
	raiseLift(380); //raise up
	delay(500); //wait for safety
	drive(-1.55, true); //drive back
	raisePopper(800); //pop over
	delay(500); //wait for star to fall
	lowerPopper(300);
	lowerLift(350); //lower lift
	delay(500); //wait for safety

	/////////////////////// THIRD RUN (identical to 2nd run)
	drive(1.55, true);
	delay(3000);
	raiseLift(380);
	delay(500);
	drive(-1.55, true);
	raisePopper(800);
	delay(500);
	lowerPopper(300);
	lowerLift(350);
	delay(500);

	////////////////////// FOURTH RUN (identical to 2nd run)
	drive(1.55, true);
	delay(3000);
	raiseLift(380);
	delay(500);
	drive(-1.55, true);
	raisePopper(800);
	delay(500);
	lowerPopper(300);
	lowerLift(350);
	delay(500);

}

//************************************************ USER CONTROL *******************************************//
task usercontrol()
{
	UserControlCodePlaceholderForTesting(); //Remove this function call once you have "real" code.
}
