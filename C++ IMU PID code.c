void turnTo(double raw, int intakeSpeed, int timeout = 15) {
if (Inertial.installed()) {
double kP = 0.45; // 0.45
double kI = 0.00006; // 0.00006
double kD = 0.50; // 0.45
double target = raw;
double error = target - Inertial.rotation(rotationUnits::deg);
double integral = error;
double prevError = error;
double derivative = error - prevError;
int motionless = 0;
while ((timeout == 0 && std::abs(error) > 1) || (std::abs(error) > 0 && (motionless <= timeout))) {
intake.spin(directionType::rev, intakeSpeed, percentUnits::pct); //controls();
error = target - Inertial.rotation(rotationUnits::deg);
integral += error;
if (error == 0) {
integral = 0; }
derivative = error - prevError;
prevError = error;
double volts = error * kP + integral * kI + derivative * kD; l.spin(fwd, volts, voltageUnits::volt);
r.spin(reverse, volts, voltageUnits::volt); if(dt.velocity(percentUnits::pct) == 0)
motionless+=15; if(dt.velocity(percentUnits::pct) != 0)
motionless+=0; vex::task::sleep(15);
}
goto end; }
else {
Brain.Screen.clearScreen(); Brain.Screen.setFont(fontType::mono40); Brain.Screen.setFillColor(red); Brain.Screen.print("No Inertial Sensor Installed");
}

 end: vex::task::sleep(20);
}
//Haven’t tested yet but should work
//Basically if you try to turn to 450 degrees it will make sure you turn to 90 and not in a circle to get there
void turnToHeading(double target, int intakeSpeed = 0, int timeout = 15) 
{
double rotation = Inertial.rotation(rotationUnits::deg); while(rotation >= 360)
{
target += 360;
rotation -= 360; }
while(rotation <= 0) {
target -= 360;
rotation += 360; }
turnTo(target, intakeSpeed, timeout); }