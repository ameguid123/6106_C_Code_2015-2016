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

#pragma platform(VEX)

// Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(15)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                         						 NOTES
//
// All directional names in motor/sensor setup are referring to side of object when viewing robot from the back
// 3 SQUARES: 1550 clicks
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Variables
int height = 360; // Top lift encoder limit
int ground = 5;	// Bottom lift encoder limit

//******************************************** Driver Control Tasks ************************************************//
task d_drive()
{
	while(true)
	{
		motor[Back_Left]  = (vexRT[Ch4] + vexRT[Ch3])/2;
		motor[Front_Left]  = (vexRT[Ch4] + vexRT[Ch3])/2;
		motor[Back_Right] = (vexRT[Ch2] + vexRT[Ch1])/2;
		motor[Front_Right]  = (vexRT[Ch2] + vexRT[Ch1])/2;
	}
}

task d_lift() // Driver lift control
{
	while(true)
	{
		while((vexRT[Btn6U] == 1) && (abs(SensorValue[TR_EN]) < abs(height))) // While Button 6U is pressed and lift is within hight limit, raise lift
		{
			motor[Top_Right] = 110;
			motor[Bottom_Right] = 110;
			motor[Top_Left] = 110;
			motor[Bottom_Left] = 110;
		}
		if(abs(SensorValue[TR_EN]) >= abs(height)) // If Button 6U is pressed but lift is not within hight limit, stop
		{
			motor[Top_Left] = 0;
			motor[Top_Right] = 0;
			motor[Bottom_Left] = 0;
			motor[Bottom_Right] = 0;
		}

		while((vexRT[Btn6D] == 1) && (abs(SensorValue[TR_EN]) > abs(ground))) // While Button 6D is pressed and lift is above ground limit, lower lift
		{
			motor[Top_Right] = -90;
			motor[Bottom_Right] = -90;
			motor[Top_Left] = -90;
			motor[Bottom_Left] = -90;
		}
		if(abs(SensorValue[TR_EN]) <= abs(ground)) // If Button 6D is pressed but lift is not above ground limit, stop
		{
			motor[Top_Left] = 0;
			motor[Top_Right] = 0;
			motor[Bottom_Left] = 0;
			motor[Bottom_Right] = 0;
		}

		else // If neither button pressed, stop
		{
			motor[Top_Left] = 0;
			motor[Top_Right] = 0;
			motor[Bottom_Left] = 0;
			motor[Bottom_Right] = 0;
		}
	}
}

task d_popper() // Driver popper control
{
	while(true)
	{
		motor[Lift_Left] = 0;
		motor[Lift_Right] = 0;

		while(vexRT[Btn5U] == 1) // Popper activated while Button 5U held down
		{
			motor[Lift_Left] = 127;
			motor[Lift_Right] = 127;
		}

		while(vexRT[Btn5D] == 1) // Popper deactivated while Button 5D held down
		{
			motor[Lift_Left] = -100;
			motor[Lift_Right] = -100;
		}
	}
}

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

	SensorValue[R_EN] = 0;
	SensorValue[L_EN] = 0;
	SensorValue[TR_EN] = 0;
	SensorValue[TL_EN] = 0;
}

//************************************************** AUTONOMOUS ***********************************************//
task autonomous()
{
	drive(-0.7, true); // Drive backwards towards fence
	delay(200); // Delay for safety
	raiseLift(390); // Raise lift all the way up
	delay(500); // Delay for safety
	drive(-1.45, true); // Drive backwards more (pushes pre-load star under fence)
	raisePopper(800); // Pop over fence (dislodges fence stars)
	delay(500); // Wait for stars to fall off
	lowerPopper(300);// Lower popper
	lowerLift(370); // Lower lift
	setLift(0);
}

//************************************************ USER CONTROL *******************************************//
task usercontrol()
{
	// Reset Encoders
	SensorValue[R_EN] = 0;
	SensorValue[L_EN] = 0;
	SensorValue[TR_EN] = 0;
	SensorValue[TL_EN] = 0;

	while (true)
	{
		// Start drive, popper, and lift tasks and run them simulatneously
		startTask(d_drive);
		startTask(d_lift);
		startTask(d_popper);

		while(true)
		{
			wait1Msec(1); // Keep the program alive; DO NOT DELETE
		}
	}
}
