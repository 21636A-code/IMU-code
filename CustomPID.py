# VEX V5 Python Project with Competition Template
import sys
import vex
from vex import *
import math

# Creates a competition object that allows access to Competition methods.
competition = vex.Competition()

def pre_auton():
    # All activities that occur before competition start
    # Example: setting initial positions
    brain        = vex.Brain()
    rightMotor1  = vex.Motor(vex.Ports.PORT1, vex.GearSetting.RATIO18_1, True)
    #rightMotor2 = vex.Motor(vex.Ports.PORT2, vex.GearSetting.RATIO18_1, True)
    leftMotor1   = vex.Motor(vex.Ports.PORT3, vex.GearSetting.RATIO18_1, False)
    #leftMotor2  = vex.Motor(vex.Ports.PORT3, vex.GearSetting.RATIO18_1, False)
    controller   = vex.Controller(vex.ControllerType.PRIMARY)
    drivetrain   = vex.Drivetrain(leftMotor1, rightMotor1, 319.1764, 292.1, vex.DistanceUnits.IN)
    leftEncoder  = vex.Encoder(vex.Triport.b)
    rightEncode  = vex.Encoder(vex.Triport.a)
    IMU          = vex.Gyro(vex.Triport.d)
    

def autonomous():
    # Place autonomous code here
    
    #below: variables to change
    wheelR = 2 #inches
    Kp = 1 #proportional pid value
    Kf = 0 #feed forward value
    maxTurnVelocity = 10
    maxDriveVelocity = 20

    #below: variables not to change
    wheelD = wheelR * 2
    wheelC = math.pi*wheelD
    
    #below: self explanitory functions
    def getvelocity():
        #TODO: get encoder command
        return 0 # return the encoder RPM
    def getTicks():
        return 0
    def resetEncoder():
        return
    def resetGyro():
        return
    def getGyroDegrees():
        return 0
    def setPWR(leftPower, rightPower):
        global driveVelocity
        drivetrain.set_velocity(driveVelocity, vex.VelocityUnits.PCT)
        leftMotor1.spin(vex.DirectionType.FWD, leftPower, vex.VelocityUnits.PCT)
        rightMotor1.spin(vex.DirectionType.FWD, rightPower, vex.VelocityUnits.PCT)
    def inchesToFeet(inches):
        return inches/12
    def feetToInches(feet):
        return feet*12
    def rpmToIPS(rpm):
        rps = rpm * 60 #one RPM unit is equivalent to 60 rps units
        IPS = rps*wheelC
        return IPS
    def rpmToFPS(rpm):
        FPS = inchesToFeet(rpmToIPS(rpm))
        return FPS
    def pidRPM(targetRPM):
        global Kp
        global Kf
        while True:
            error = getvelocity()-targetRPM
            setPWR(Kp*error)
    def ticksTorotation(ticks):
        rotations = ticks/360
        return rotations
    def rotationsToTicks(rotations):
        ticks = rotations*360
        return ticks
    def inchesToRotations(inches):
        global wheelC
        rotations = inches/wheelC
    def moveInches(inches, timeOut):
        resetEncoder()
        global driveVelocity
        global Kp
        global Kf
        inches = rotationsToTicks(inchesToRotations(inches))
        while timer < timeOut:
            Ke = inches - getTicks()
            pwr = Kp*Ke+Kf
            setPWR(pwr, pwr)
    def turnRight(degrees, timeOut):
        resetEncoder()
        global turnVelocity
        global Kp
        global Kf
        while timer < timeOut:
            Ke = inches - getGyroDegrees()
            pwr = Kp*Ke+Kf
            setPWR(pwr, -pwr)
    def turnLeft(degrees):
        resetEncoder()
        global turnVelocity
        global Kp
        global Kf
        while timer < timeOut:
            Ke = inches - getGyroDegrees()
            pwr = Kp*Ke+Kf
            setPWR(-pwr, pwr)
def drivercontrol():
    # Place drive control code here, inside the loop
    while True:
        # This is the main loop for the driver control.
        # Each time through the loop you should update motor
        # movements based on input from the controller.
        pass

# Do not adjust the lines below

# Set up (but don't start) callbacks for autonomous and driver control periods.
competition.autonomous(autonomous)
competition.drivercontrol(drivercontrol)

# Run the pre-autonomous function.
pre_auton()

# Robot Mesh Studio runtime continues to run until all threads and
# competition callbacks are finished.
