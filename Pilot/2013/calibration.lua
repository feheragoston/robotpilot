-- CALIBRATE
if (CALIBRATED == nil) then
	CALIBRATED = true;
	
	dofile("Pilot/2013/defines.lua")	

	p.sleep(1000)
	
	c.music("start")

	c.print("Startra varunk");
	repeat p.process() until (c.GetStartButton());
	c.print("Startgomb lenyomva, elkezdjuk a kalibraciot");

	c.SetMotorSupply(true)
	c.print("Motortap bekapcsolva");
	
	p.sleep(2000)

	p.CalibrateConsole();

	--rear: 90 fuggoleges, 0 vizszint	
	p.ArmMove(FRONT,90)
	p.ArmMove(REAR,90)

	-- benn -40,100000,100000 (innen kozepre 22,100,100) labda rajta 22, kinn 
	p.SelectorMove(-40)
	p.SelectorMove(60)
	p.SelectorMove(22)

	p.FlipperMove(0)
	p.FlipperMove(90)

	--164 / 204 belseje milyen tav
	p.ContractorMove(164)
	p.ContractorMove(204)

	-- 0 megakaszt, 90 atenged
	p.FireStopperMove(90)
	p.FireStopperMove(0)

	p.SetFireWheelOn()
	p.SetCaracoleOn()

	p.sleep(2000)

	p.SetFireWheelOff()
	p.SetCaracoleOff()


	while (c.GetMyColor() ~= 0) do
		p.process()
		c.music("caliberror")
	end

	c.music("calibrate")
	
	c.CalibrateDeadreckoning()		--ez mit csinal?
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
				p.GripperMove(true, 70)
				p.GripperMove(false, 70)
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

	elseif (c.GetMyColor() == BLUE) then
		c.print("Hajra Kekek!");
	end

	c.print("Calibrate finished");
end
-- CALIBRATE END
