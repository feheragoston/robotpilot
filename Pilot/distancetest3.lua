Offset = 0;
Ori = 1;

dofile("Pilot/calibration.lua")

goSpeed = 100
goAcc = 400
turnSpeed = 0.25
turnAcc = 4

repeat c.process() until (c.GetStartButton());
--c.StartMatch();
--c.print("Meccs elkezdodott");

if (c.GetMyColor()) then
	c.print("Kekek vagyunk");
	Offset = 3000;
	Ori = -1;
end

p.TurnTo(350, 1000, turnSpeed, turnAcc)
p.GoTo(350, 1000, goSpeed, goAcc)

repeat
	p.TurnTo(1050, 1500, turnSpeed, turnAcc)
	p.GoTo(1050, 1500, goSpeed, goAcc)
	p.Turn(math.pi * 2, turnSpeed, turnAcc)
	p.Turn(-math.pi * 2, turnSpeed, turnAcc)
	
	p.TurnTo(1400, 1150, turnSpeed, turnAcc)
	p.GoTo(1400, 1150, goSpeed, goAcc)
	p.Turn(math.pi * 2, turnSpeed, turnAcc)
	p.Turn(-math.pi * 2, turnSpeed, turnAcc)
	
	p.TurnTo(1400, 1850, turnSpeed, turnAcc)
	p.GoTo(1400, 1850, goSpeed, goAcc)
	p.Turn(math.pi * 2, turnSpeed, turnAcc)
	p.Turn(-math.pi * 2, turnSpeed, turnAcc)
	
	p.TurnTo(700, 1850, turnSpeed, turnAcc)
	p.GoTo(700, 1850, goSpeed, goAcc)
	p.Turn(math.pi * 2, turnSpeed, turnAcc)
	p.Turn(-math.pi * 2, turnSpeed, turnAcc)
	
	p.TurnTo(700, 1150, turnSpeed, turnAcc)
	p.GoTo(700, 1150, goSpeed, goAcc)
	p.Turn(math.pi * 2, turnSpeed, turnAcc)
	p.Turn(-math.pi * 2, turnSpeed, turnAcc)
until (c.GetStopButton())
