#pragma region VEXcode Generated Robot Configuration
// Make sure all required headers are included. 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#include "v5.h"
#include "v5_vcs.h"


// Brain should be defined by default. #test change
brain Brain;
competition Competition;

// START V5 MACROS
#define waitUntil(condition) \
  do                         \
  {                          \
    wait(5, msec);           \
  } while (!(condition))

#define repeat(iterations) \
  for (int iterator = 0; iterator < iterations; iterator++)
// END V5 MACROS

// Robot configuration code.
motor leftMotorA = motor(PORT19, ratio18_1, false);
motor leftMotorB = motor(PORT10, ratio18_1, false);
motor_group LeftDriveSmart = motor_group(leftMotorA, leftMotorB);
motor rightMotorA = motor(PORT12, ratio18_1, true);
motor rightMotorB = motor(PORT3, ratio18_1, true);
motor_group RightDriveSmart = motor_group(rightMotorA, rightMotorB);
drivetrain Drivetrain = drivetrain(LeftDriveSmart, RightDriveSmart, 319.19, 295, 40, mm, 1);

controller Controller1 = controller(primary);
motor FlapMotorA = motor(PORT21, ratio18_1, false);
motor FlapMotorB = motor(PORT2, ratio18_1, true);
motor_group Flap = motor_group(FlapMotorA, FlapMotorB);

motor Intake = motor(PORT1, ratio6_1, false);

digital_out Clamp = digital_out(Brain.ThreeWirePort.A);

// generating and setting random seed
void initializeRandomSeed()
{
  int systemTime = Brain.Timer.systemHighResolution();
  double batteryCurrent = Brain.Battery.current();
  double batteryVoltage = Brain.Battery.voltage(voltageUnits::mV);

  // Combine these values into a single integer
  int seed = int(batteryVoltage + batteryCurrent * 100) + systemTime;

  // Set the seed
  srand(seed);
}

void vexcodeInit()
{

  // Initializing random seed.
  initializeRandomSeed();
}

// Helper to make playing sounds from the V5 in VEXcode easier and
// keeps the code cleaner by making it clear what is happening.
void playVexcodeSound(const char *soundName)
{
  printf("VEXPlaySound:%s\n", soundName);
  wait(5, msec);
}

// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;
// define variables used for controlling motors based on controller inputs
bool Controller1RightShoulderControlMotorsStopped = true;
bool DrivetrainLNeedsToBeStopped_Controller1 = true;
bool DrivetrainRNeedsToBeStopped_Controller1 = true;

// define a task that will handle monitoring inputs from Controller1
int rc_auto_loop_function_Controller1()
{
  // process the controller input every 20 milliseconds
  // update the motors based on the input values
  while (true)
  {
    if (RemoteControlCodeEnabled)
    {

      // calculate the drivetrain motor velocities from the controller joystick axies
      // left = Axis3
      // right = Axis2
      int drivetrainLeftSideSpeed = Controller1.Axis3.position();
      int drivetrainRightSideSpeed = Controller1.Axis2.position();

      // check if the value is inside of the deadband range
      if (drivetrainLeftSideSpeed < 5 && drivetrainLeftSideSpeed > -5)
      {
        // check if the left motor has already been stopped
        if (DrivetrainLNeedsToBeStopped_Controller1)
        {
          // stop the left drive motor
          LeftDriveSmart.stop();
          // tell the code that the left motor has been stopped
          DrivetrainLNeedsToBeStopped_Controller1 = false;
        }
      }
      else
      {
        // reset the toggle so that the deadband code knows to stop the left motor nexttime the input is in the deadband range
        DrivetrainLNeedsToBeStopped_Controller1 = true;
      }
      // check if the value is inside of the deadband range
      if (drivetrainRightSideSpeed < 5 && drivetrainRightSideSpeed > -5)
      {
        // check if the right motor has already been stopped
        if (DrivetrainRNeedsToBeStopped_Controller1)
        {
          // stop the right drive motor
          RightDriveSmart.stop();
          // tell the code that the right motor has been stopped
          DrivetrainRNeedsToBeStopped_Controller1 = false;
        }
      }
      else
      {
        // reset the toggle so that the deadband code knows to stop the right motor next time the input is in the deadband range
        DrivetrainRNeedsToBeStopped_Controller1 = true;
      }

      // only tell the left drive motor to spin if the values are not in the deadband range
      if (DrivetrainLNeedsToBeStopped_Controller1)
      {
        LeftDriveSmart.setVelocity(drivetrainLeftSideSpeed, percent);
        LeftDriveSmart.spin(forward);
      }
      // only tell the right drive motor to spin if the values are not in the deadband range
      if (DrivetrainRNeedsToBeStopped_Controller1)
      {
        RightDriveSmart.setVelocity(drivetrainRightSideSpeed, percent);
        RightDriveSmart.spin(forward);
      }
      // check the ButtonR1/ButtonR2 status to control Intake
      if (Controller1.ButtonR1.pressing())
      {
        Intake.spin(forward);
        Controller1RightShoulderControlMotorsStopped = false;
      }
      else if (Controller1.ButtonR2.pressing())
      {
        Intake.spin(reverse);
        Controller1RightShoulderControlMotorsStopped = false;
      }
      else if (!Controller1RightShoulderControlMotorsStopped)
      {
        Intake.stop();
        // set the toggle so that we don't constantly tell the motor to stop when the buttons are released
        Controller1RightShoulderControlMotorsStopped = true;
      }
    }
    // wait before repeating the process
    wait(20, msec);
  }
  return 0;
}

task rc_auto_loop_task_Controller1(rc_auto_loop_function_Controller1);

#pragma endregion VEXcode Generated Robot Configuration

// ----------------------------------------------------------------------------
//
//    Project:
//    Author:
//    Created:
//    Configuration:
//
// ----------------------------------------------------------------------------

// Include the V5 Library
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#include "v5.h"
#include "v5_vcs.h"

// Allows for easier use of the VEX Library
using namespace vex;

// Begin project code

void pre_auton(void)
{
  // actions to do when the program starts
  Brain.Screen.clearScreen();
  Brain.Screen.print("pre auton code");
  wait(1, seconds);
}

void autonomous(void)
{
  Brain.Screen.clearScreen();
  Brain.Screen.print("autonomous code");
  Drivetrain.driveFor(forward, 40, inches);
  Drivetrain.turnFor(left, 180, degrees);
  Drivetrain.driveFor(forward, 40, inches);
  Drivetrain.turnFor(left, 40, degrees);
  Drivetrain.driveFor(reverse, 60, inches);
}

void flapclose()
{
  Flap.spinFor(reverse, 180, degrees);
}

void flapstart()
{
  Flap.spinFor(forward, 180, degrees);
}
void clampclose()
{

  Clamp.set(true);
  wait(1.0, seconds);
}
void clampOpen()
{

  Clamp.set(false);
  wait(1, seconds);
}

void userControl(void)
{
  Brain.Screen.clearScreen();
  Controller1.ButtonL1.pressed(flapclose);
  Controller1.ButtonL2.pressed(flapstart);
  Controller1.ButtonUp.pressed(clampOpen);
  Controller1.ButtonDown.pressed(clampclose);

  // Controls the flipper on the robot
  while (true)
  {
    if (Controller1.ButtonUp.pressing())
    {
      Clamp.set(true);
    }
    else if (Controller1.ButtonDown.pressing())
    {
      Clamp.set(false);
    }
    wait(20, msec);
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main()
{
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(userControl);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true)
  {
    wait(100, msec);
  }
}
