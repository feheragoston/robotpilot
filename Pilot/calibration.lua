-- CALIBRATE
if (CALIBRATED == nil) then
	CALIBRATED = true;
	
	Offset = 0
	Ori = 1

	goSpeed = 800
	goAcc = 800
	turnSpeed = 6
	turnAcc = 10
	
	GripperGrab = 67
	GripperHold = 55

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

	--p.Go(60)
	if (c.GetMyColor()) then
		p.Turn(0.4)
	else
		p.Turn(-0.4)
	end

	p.sleep(1000)

	if (c.GetMyColor()) then
		c.print("Kekek vagyunk");
		Offset = 3000;
		Ori = -1;
	end

	c.print("Calibrate finished");
end
-- CALIBRATE END