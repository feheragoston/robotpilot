-- CALIBRATE
if (CALIBRATED == nil) then
	CALIBRATED = true;
	
	Offset = 0
	Ori = 1
	Left = true
	Right = false

	goSpeed = 600
	goAcc = 800
	turnSpeed = 6
	turnAcc = 10
	
	p.sleep(1000)
	
	c.music("start")

	c.print("Startra varunk");
	repeat p.process() until (c.GetStartButton());
	c.print("Startgomb lenyomva, elkezdjuk a kalibraciot");

	c.SetMotorSupply(true)
	c.print("Motortap bekapcsolva");
	
	p.sleep(2000)
	
	p.ArmMove(0)
	p.GripperMove(false, 1)
	p.GripperMove(true, 6)
	p.ClawMove(false, 0)
	p.ClawMove(true, 2)
	
	p.CalibrateConsole();
	
	while (c.GetMyColor() ~= 0) do
		p.process()
		c.music("caliberror")
	end

	c.music("calibrate")
	
	c.CalibrateDeadreckoning()
	c.print("Deadreckoning beallitva");
	p.sleep(1000)
	
	p.sleep(1000)

	if (c.GetMyColor() == RED) then
		c.print("Pirosak vagyunk");
		Offset = 3000;
		Ori = -1;
		Left = false
		Right = true

		p.runparallel(
			function()
				p.GripperMove(true, 55)
				p.GripperMove(false, 55)
				p.GripperMove(true, 1)
				p.GripperMove(false, 6)
			end,
			function()
				p.ClawMove(true, 55)
				p.ClawMove(false, 55)
				p.ClawMove(true, 0)
				p.ClawMove(false, 2)
			end
		)

	elseif (c.GetMyColor() == PURPLE) then
		c.print("Hajra Lilak!");
	end

	c.print("Calibrate finished");
end
-- CALIBRATE END