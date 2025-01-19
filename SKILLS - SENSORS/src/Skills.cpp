/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       emmag                                                     */
/*    Created:      12/29/2024, 1:23:56 PM                                    */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;
 brain Brain; 

inertial Inertial = inertial(PORT18); 
motor leftMotorA = motor(PORT19, ratio18_1, false); 
motor leftMotorB = motor(PORT10, ratio18_1, false);
motor_group LeftDriveSmart = motor_group(leftMotorA, leftMotorB); 
motor rightMotorA = motor (PORT12, ratio18_1, true);
motor rightMotorB = motor (PORT3, ratio18_1, true);
motor_group RightDriveSmart = motor_group(rightMotorA, rightMotorB);
// 1st number is Wheel Travel 2nd number is Track which is the distance between the 1st and 2nd wheels. 3rd measurement is Wheel Base which is a measurement from the front axel to the back axel.  
smartdrive Smartdrive = smartdrive(LeftDriveSmart, RightDriveSmart, Inertial, 319.19, 370, 205, mm, 1); 

controller Controller1 = controller(primary); 

motor FlapMotorA = motor(PORT21, ratio18_1, false);
motor FlapMotorB = motor(PORT2, ratio18_1, true); 
motor_group Flap = motor_group(FlapMotorA, FlapMotorB); 

motor Intake = motor(PORT1, ratio6_1, false); 

digital_out Clamp = digital_out(Brain.ThreeWirePort.A);

// Function- Makes Flap spin. 
void flapclose ()
{
  
  Flap.spinToPosition(0, degrees); 
}

void flapstart ()
{
  Flap.setVelocity(100, percent); 
Flap.spinToPosition(-210, degrees);
}

// Makes Clamp open/close.

void clampclose ()
{
  Clamp.set(true);
  wait(1.0, seconds);
}

void clampopen ()
{
  Clamp.set(false);
  wait(1.0, seconds); 
}

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {

  Flap.resetPosition();
  Inertial.calibrate(); 
  while (Inertial.isCalibrating()){
  Controller1.Screen.clearLine(3); 
  Controller1.Screen.print("Inertial Calibrating"); 
  wait(20, msec);}
Controller1.Screen.clearLine(3); 
Controller1.Screen.print("Ready To Drive"); 
} 
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
Brain.Screen.clearScreen();
  Brain.Screen.print("autonomous code");
  //drives robot forward to the mobile goal then clamps it. 
  Intake.setVelocity(100, percent); 
  Smartdrive.driveFor(forward, 5, inches); 
  clampclose();
  // flips the ring onto the robot
  flapstart();
  flapclose(); 
  Smartdrive.setDriveVelocity(100, percent); 
  Smartdrive.turnToHeading(180, degrees);
  Intake.spin(reverse); 
  Smartdrive.driveFor(reverse, 24, inches);
  wait(1.5, sec); 
  flapstart();
  flapclose();  
  Smartdrive.turnToHeading(270, degrees); 
  Smartdrive.driveFor(reverse, 24, inches); 
  wait(1.5, sec); 
  flapstart();
  flapclose();
  Smartdrive.turnToHeading(355, degrees); 
  Smartdrive.driveFor(reverse, 17, inches);  
  wait(1.5, sec);
  Smartdrive.driveFor(forward, 2, inches); 
  flapstart();
  flapclose();
  wait(4, seconds); 
  Smartdrive.driveFor(reverse, 14, inches);  
  flapstart(); 
  flapclose();
  Smartdrive.driveFor(forward, 16, inches); 
  Smartdrive.turnToHeading(275, degrees); 
  Smartdrive.driveFor(reverse, 10, inches); 
  flapstart();
  flapclose();
  Smartdrive.turnToHeading(150, degrees); 
  Smartdrive.driveFor(forward, 24, inches); 
  clampopen();
  
}
;
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
