#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <chrono>

#include <Joystick.h>
#include <SampleRobot.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include <RobotDrive.h>
#include <Timer.h>
#include <Victor.h>
#include <Solenoid.h>
#include "WPILib.h"
#include "AHRS.h"
#include "Encoder.h"


class Robot: public frc::SampleRobot
{
	// Motor ports
	int rearRight = 0;
	int rearLeft = 2;
	int frontRight = 1;
	int frontLeft = 3;
	int _shooter = 6;
	int lift = 4;
	public : int _gear = false;

	// Scalers
	int shooterPower = .76;
	int drivePower = .70;

	bool shooterStatus = false;

	// Navx Data
	const double kToleranceDegrees = 2.0f;
	AHRS *ahrs;

	// Encoder
	//Encoder leftEncoder = new Encoder(0, 1, false, Encoder::EncodingType::k4X);
	//Encoder rightEncoder = new Encoder(2, 3, false, Encoder::EncodingType::k4X);

	// Button & Axis mapping
	int rt = 3;
	int lt = 2;
	int rsy = 1;
	int lsy = 5;
	int bx = 2;
	int ab = 0;
	int xb = 2;

	// Motors
	frc::RobotDrive driveTrain  {rearRight, rearLeft, frontRight, frontLeft};
	//frc::RobotDrive hopper { hopperLeft, hopperRight };

	frc::Victor shooter  { _shooter };
	//frc::Victor testMotor { testPort };
	//frc::Victor intake { _intake };

	// Controllers
	frc::Joystick joystick0 { 0 };
	public : frc::Joystick joystick1 { 1 };

	// Pnumatics
	//public : frc::Solenoid gear { 0 };

	// Misc
	frc::SendableChooser<std::string> chooser;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "My Auto";
	const std::string driveStraight = "Drive Straight";
	const std::string goToGear = "Drive to gear";

public:
	Robot()
	{
		// Set motor expirations
		driveTrain.SetExpiration(0.1);
		shooter.SetExpiration(0.1);

		// Attempt to initalize the controller
		try
		{
			ahrs = new AHRS(SPI::Port::kMXP);
		}
		catch (std::exception& ex)
		{
			std::string err_string = "Error instantiating navX MXP:    ";
			err_string += ex.what();
			DriverStation::ReportError(err_string.c_str());
		}

		// Set the encoder 0
		//leftEncoder.Reset();
		//rightEncoder.Reset();
	}

	void DriveStraightForDistance(bool useDriveStraight, int distance)
	{
		if (useDriveStraight)
		{

		}
		else
		{

		}
	}

	void RobotInit()
	{
		chooser.AddDefault(autoNameDefault, autoNameDefault);
		chooser.AddObject(autoNameCustom, autoNameCustom);
		chooser.AddObject(driveStraight, driveStraight);
		chooser.AddObject(goToGear, goToGear);
		frc::SmartDashboard::PutData("Auto Modes", &chooser);

		driveTrain.SetInvertedMotor(frc::RobotDrive::kFrontLeftMotor, false);
		driveTrain.SetInvertedMotor(frc::RobotDrive::kFrontRightMotor, false);
		driveTrain.SetInvertedMotor(frc::RobotDrive::kRearLeftMotor, false);
		driveTrain.SetInvertedMotor(frc::RobotDrive::kRearRightMotor, false);
		shooter.SetInverted(true);
	}

	void Autonomous()
	{
		auto autoSelected = chooser.GetSelected();
		// std::string autoSelected = frc::SmartDashboard::GetString("Auto Selector", autoNameDefault);
		std::cout << "Auto selected: " << autoSelected << std::endl;

		if (autoSelected == autoNameCustom)
		{
			// Custom Auto goes here
			std::cout << "Running custom Autonomous" << std::endl;
			driveTrain.SetSafetyEnabled(false);
			driveTrain.Drive(-0.5, 1.0); // spin at half speed
			frc::Wait(2.0);                // for 2 seconds
			driveTrain.Drive(0.0, 0.0);  // stop robot
		}

		if (autoSelected == driveStraight)
		{
			std::cout << "Running drive straight" << std::endl;
			ahrs->Reset();
			driveTrain.SetSafetyEnabled(false);
			std::cout << "Angle: " << ahrs->GetAngle() << std::endl;

			namespace cr = std::chrono;
			typedef cr::high_resolution_clock my_clock;

			double seconds = 5;

			for ( double i = 0; i < seconds * 20; i++ )
			{
				driveTrain.TankDrive(-GetMotorOutput(ahrs->GetAngle(), 1) *.5 ,-GetMotorOutput(ahrs->GetAngle(), 0) *.5);
				frc::Wait(.05);
			}

			driveTrain.TankDrive(0.0,0.0);
		}

		if (autoSelected == goToGear)
		{
			std::cout << "Running go to gear" << std::endl;
			ahrs->Reset();
			driveTrain.SetSafetyEnabled(false);
			std::cout << "Angle: " << ahrs->GetAngle() << std::endl;

			namespace cr = std::chrono;
			typedef cr::high_resolution_clock my_clock;

			double seconds = 2;

			for ( double i = 0; i < seconds * 20; i++ )
			{
				driveTrain.TankDrive(-GetMotorOutput(ahrs->GetAngle(), 1) *.5 ,-GetMotorOutput(ahrs->GetAngle(), 0) *.5);
				frc::Wait(.05);
			}

			driveTrain.TankDrive(0.0,0.0);

			// Now, rotate to the degree.
			RotateToDegrees(-61.22, 5.0);

			// Now, Drive stright for 1 second.
			//DriveStraightFor(1.0);


			// Now, drive straight.
			ahrs->Reset();

			frc::Wait(0.5);

			seconds = 5.0;
			for ( double i = 0; i < seconds * 20; i++ )
			{
				driveTrain.TankDrive(-GetMotorOutput(ahrs->GetAngle(), 1) *.5 ,-GetMotorOutput(ahrs->GetAngle(), 0) *.5);
				frc::Wait(.05);
			}

			driveTrain.TankDrive(0.0,0.0);


		}

		else
		{
			// Default Auto goes here
			std::cout << "Running default Autonomous" << std::endl;
			driveTrain.SetSafetyEnabled(false);
			driveTrain.Drive(-0.5, 0.0); // drive forwards half speed
			frc::Wait(2.0);                // for 2 seconds
			driveTrain.Drive(0.0, 0.0);  // stop robot
		}
	}

	void DriveStraightFor(double time)
	{
		ahrs -> Reset();
		for ( double i = 0; i < time * 20; i++ )
		{
			driveTrain.TankDrive(-GetMotorOutput(ahrs->GetAngle(), 1) *.5 ,-GetMotorOutput(ahrs->GetAngle(), 0) *.5);
			frc::Wait(.05);
		}
	}

	void RotateToDegrees ( float degrees, double tolerance )
	{
		std::cout << "Set tol" << std::endl;
		float high = degrees + tolerance;
		float low = degrees - tolerance;

		std::cout << high << std::endl;
		std::cout << low << std::endl;
		std::cout << ahrs->GetAngle() << std::endl;
		std::cout << "Start loop" << std::endl;

		while ( low  > ahrs->GetAngle() || ahrs->GetAngle() > high)
		{
			std::cout << "Angle: " << ahrs->GetAngle() << std::endl;
			// If less than low go right.
			if (ahrs->GetAngle() < low)
			{

				driveTrain.TankDrive(0.5, -0.5);
			}
			if (ahrs ->GetAngle() > high)
			{
				driveTrain.TankDrive(-0.5, 0.5);
			}
		}

		driveTrain.TankDrive(0.0,0.0);

		std::cout << "Loop ended." << std::endl;
	}

	double GetMotorOutput(float degrees, int sel)
	{
		// max ouputs
		int rX = 1;
		int lX = 1;
		float lP = 0;
		float rP = 0;
		float mid = 15.0;
		// Plug in the function for the new right output.

		if (degrees > 360)
		{
			degrees -= 360;
		}
		if (degrees < -360)
		{
			degrees += 360;
		}

        rP = rX - ( ( 1.0 / mid ) * -degrees * rX );
        lP = lX - ( ( 1.0 / mid ) * degrees * lX );

        if (rP > 1)
        {
            rP = 1;
        }
        else if (rP < -1)
        {
            rP = -1;
        }

        if ( lP > 1 )
        {
            lP = 1;
        }
        else if ( lP < -1 )
        {
            lP = -1;
        }

        double ret = (sel == 1) ? rP : lP;
        std::cout << "New motor power:  " << ret << std::endl;
        return ret;

	}


	void OperatorControl() override
	{

		// Set Safety
		//driveTrain.SetSafetyEnabled(true);

		while (IsOperatorControl() && IsEnabled())
		{
			// Print out the rotation
			std::cout << "Yaw:    " << ahrs->GetAngle() << std::endl;


			// Drive train controls
			// First, check if the right ( turbo ) trigger has been presesd. If so, use full values.
			// If not, then use the scaler.
			if (joystick0.GetRawAxis(rt))
			{
				driveTrain.TankDrive(joystick0.GetRawAxis(lsy), joystick0.GetRawAxis(rsy));
			}
			else
			{
				driveTrain.TankDrive(joystick0.GetRawAxis(lsy) * 0.7, joystick0.GetRawAxis(rsy) * 0.7);
			}

			// Shooter toggle on x
			if (joystick0.GetRawButton(1))
			{
				shooterStatus = !shooterStatus;

				if (shooterStatus)
				{
					shooter.Set(1);
				}
				else
				{
					shooter.Set(0);
				}

				frc::Wait(.2);
			}

			// If the 4th button is being pressed, then use the drive straight function.

			if (joystick0.GetRawButton(4))
			{

				driveTrain.TankDrive(-GetMotorOutput(ahrs->GetAngle(), 1) *.5 ,-GetMotorOutput(ahrs->GetAngle(), 0) *.5);

			}

			if (joystick0.GetRawButton(2))
			{
				std::cout<< "Resetting..." << std::endl;
				ahrs->Reset();
			}


			// Hopper controls
			if (joystick1.GetRawAxis(rt))
			{
				// Start the right hopper

			}
			else if (joystick1.GetRawAxis(lt))
			{

			}
			else
			{

			}

			frc::Wait(0.005);
		}
	}

	void Test() override
	{

	}
};



START_ROBOT_CLASS(Robot)
