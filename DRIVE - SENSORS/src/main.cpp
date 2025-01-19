/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       emmag                                                     */
/*    Created:      12/30/2024, 6:07:37 PM                                    */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// Allows use of VEX terms (Header files) 
#include "vex.h"

using namespace vex;

// A global instance of brain used for printing to the V5 brain screen
brain Brain;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here
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
  Flap.setVelocity(100, percent); 
  Flap.spinToPosition(-210, degrees); 
}

void flapstart ()
{
Flap.spinToPosition(0, degrees); 
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

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
  Brain.Screen.clearScreen();
  Brain.Screen.print("pre auton code");
  wait(1,seconds);
Inertial.calibrate(); 
  while (Inertial.isCalibrating()){
  Controller1.Screen.clearLine(3); 
  Controller1.Screen.print("Inertial Calibrating"); 
  wait(20, msec);}
Controller1.Screen.clearLine(3); 
Controller1.Screen.print("Ready To Drive");
  // resets Flap position to 0 degrees. 
  Flap.resetPosition();  
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
  Smartdrive.driveFor(forward, 32, inches);
  clampclose(); 
  flapclose();
  flapstart();

  
  
}

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
Controller1.ButtonL1.pressed(flapclose); 
Controller1.ButtonL2.pressed(flapstart); 
Controller1.ButtonUp.pressed(clampopen); 
Controller1.ButtonDown.pressed(clampclose); 
Intake.setVelocity(100, percent); 
    
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.
  
    LeftDriveSmart.spin(forward, Controller1.Axis3.position(), percent); 
    RightDriveSmart.spin(forward, Controller1.Axis2.position(), percent);      

      // Set Drivercontrol controls for the intake. 
      if (Controller1.ButtonR2.pressing())
      {
         
        Intake.spin(reverse);
      
      }
      else if (Controller1.ButtonR1.pressing())
      {
        
        Intake.spin(forward);
        
      }
      else
      {
        Intake.stop(coast); 
        
      }

    
    
    }

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
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