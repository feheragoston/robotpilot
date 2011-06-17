
Offset = 0;
Ori = 1;

dofile("Pilot/calibration.lua")

repeat c.process() until (c.GetStartButton());
--c.StartMatch();
--c.print("Meccs elkezdodott");

if (c.GetMyColor()) then
	c.print("Kekek vagyunk");
	Offset = 3000;
	Ori = -1;
end

p.GoTo(250, Offset + Ori * 800)

moveFinished = false;
pawnInGripper = false;
c.runparallel(
function()
	p.TurnToSafe(1500, Offset + Ori * 800, 10, 20)
	p.GripperMove(90)
	p.GoToSafe(1500, Offset + Ori * 800, 400, 500)
	moveFinished = true;
end,
function()
	repeat c.process() until (c.PawnInGripper() or moveFinished);
	pawnInGripper = c.PawnInGripper()
	if (not moveFinished) then
		p.MotionStop(500);
	end
end)

if (pawnInGripper) then
	p.GoSafe(-200)
	p.GripperMove(0)
	p.TurnSafe(math.pi / 2, 10, 20)
	p.ArmMove(false, 125)
	p.Magnet(false, 1)
	p.sleep(10)
	p.ArmMove(false, 0)
end
