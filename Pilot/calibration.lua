-- CALIBRATE
if (CALIBRATED == nil) then
	CALIBRATED = true;
	
	p.sleep(1000)
	c.SetMotorSupply(true)
	c.print("Motortap bekapcsolva");
	
	p.sleep(2000)
	repeat c.process() until (c.GetStartButton());
	c.print("Startgomb lenyomva felvesszuk a kezdopoziciot");
	
	p.Magnet(true, 0)
	p.Magnet(false, 0)
	p.ArmMove(true, 0)
	c.print("Balkar behajtva");
	p.ArmMove(false, 0)
	c.print("Jobbkar behajtva");
	
	p.GripperMove(0)
	c.print("Gripper behajtva");
	
	c.CalibrateConsole();
	
	c.music("calibrate")
	
	c.CalibrateDeadreckoning()
	c.print("Deadreckoning beallitva");
	p.sleep(1000)
	
	p.Go(30)
	x, y, phi = c.GetRobotPos();
	p.TurnTo(x, 1500)
	
	p.Go(60)
	
	p.sleep(1000)
	c.print("Calibrate finished");
end
-- CALIBRATE END