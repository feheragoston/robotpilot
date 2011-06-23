Offset = 0;
Ori = 1;

dofile("Pilot/calibration.lua")

goSpeed = 400
goAcc = 800
turnSpeed = 2
turnAcc = 8

repeat p.process() until (c.GetStartButton());
--c.StartMatch();
--c.print("Meccs elkezdodott");

if (c.GetMyColor()) then
	c.print("Kekek vagyunk");
	Offset = 3000;
	Ori = -1;
end

repeat
	p.TurnTo(350, 1150, turnSpeed, turnAcc)
	p.GoTo(350, 1150, goSpeed, goAcc)
	p.TurnTo(1600, 1150, turnSpeed, turnAcc)
	p.GoTo(1600, 1150, goSpeed, goAcc)
	p.Go(-1250, goSpeed, goAcc)
	
	p.TurnTo(350, 1850, turnSpeed, turnAcc)
	p.GoTo(350, 1850, goSpeed, goAcc)
	p.TurnTo(1600, 1850, turnSpeed, turnAcc)
	p.GoTo(1600, 1850, goSpeed, goAcc)
	p.Go(-1250, goSpeed, goAcc)
until (c.GetStopButton())
