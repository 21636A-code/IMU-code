// this method turns the bot to an absolute position
// for example, if the bot is at 36.7 degrees from it’s calibration point and it is told to turn to 90 degrees it will drive right till for 53.3 degrees which makes the bot face 90 degrees from the starting point.
// this is useful because if cubes or any other force acts on upon the robot in autonomous the robot will still turn to the correct angle regardless of if it got pushed off of course
// to turn in a left motion use negative numbers in the raw parameter when calling the method (e.g. turnTo(-90))

// you may notice that using all three parameters available are not necessary which is because some of the parameters are assigned a default value which can get overwritten when calling the method (e.g. if you want to run your intake while turning at 50% speed you can call the method like this: turnTo(90, 50))

// once you start tuning the PID constants (kP, kI, kD) you may notice that it’s very time consuming so inorder to make tuning easier there is a 3rd parameter in the turn function which checks if the drive speed is at 0, in which case the bot has come to a complete stop, and then exits the loop
// the timeout parameter controls how long the robot sits at 0 and the minimum you can put in the timeout parameter is [1, 15] which is 15ms because that is how long the wait is in the while loop
// if you think that the minimum wait time after the turn is too long you can slightly modify the code to allow for a sooner break in the turning loop by increasing the minimum velocity in the velocity if statement

// putting a 0 in for timeout makes the robot attempt to get within +- 1 degree of the target before ending the turn which has its own benefits and drawbacks

void turnTo(double degrees, int intakeSpeed = 0, int timeout = 15)
{
  if (Inertial.installed()) 
  {
    double kP = 0.45;    // this basically controls the turn power
    double kI = 0.00006; // this controls correction power
    double kD = 0.50;    // this controls deceleration

//important note, kP, kI, kD will need to be tuned in order for it to fit your bot perfectly. This can be running a simple turn 90 degrees and changing the values from what you observed happen in that turn.

    double target = degrees;
    double error = target - Inertial.rotation(rotationUnits::deg);
    double integral = error;
    double prevError = error;
    double derivative = error - prevError;
    int motionless = 0;
    while ((timeout == 0 && std::abs(error) > 1) || (std::abs(error) > 0 && (motionless <= timeout)))
    {
      intake.spin(directionType::rev, intakeSpeed, percentUnits::pct);
      error = target - Inertial.rotation(rotationUnits::deg);
      integral += error;
      if (error == 0) 
      {
        integral = 0;
      }
      derivative = error - prevError;
      prevError = error;
      double volts = error * kP + integral * kI + derivative * kD;
      l.spin(fwd, volts, voltageUnits::volt);
      r.spin(reverse, volts, voltageUnits::volt);
      if(dt.velocity(percentUnits::pct) == 0)
        motionless+=15;
      if(dt.velocity(percentUnits::pct) != 0)
        motionless+=0;
      vex::task::sleep(15);
    }
    goto end; 
  }
  else
  {
    Brain.Screen.clearScreen();
    Brain.Screen.setFont(fontType::mono40);
    Brain.Screen.setFillColor(red);
    Brain.Screen.print("No Inertial Sensor Installed"); 
  }
end:
  vex::task::sleep(20);
}


// Haven’t tested yet but should work
// Basically if you try to turn to 450 degrees it will make sure you turn to 90 and not in a circle to get there
// This is more useful for Odometry purposes but you can use it in manual too
void turnToHeading(double target, int intakeSpeed = 0, int timeout = 15) 
{
  double rotation = Inertial.rotation(rotationUnits::deg);
  while(rotation >= 360)
  {
    target += 360;
    rotation -= 360;
  }
  while(rotation <= 0)
  {
    target -= 360;
    rotation += 360;
  }
  turnTo(target, intakeSpeed, timeout);
}

