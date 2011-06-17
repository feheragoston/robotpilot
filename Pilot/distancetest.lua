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

p.GoTo(225, Offset + Ori * (410 + ROBOT_WIDTH))
p.TurnTo(1500, Offset + Ori * (410 + ROBOT_WIDTH))

c.runparallel(
function()
	p.GoTo(1500, Offset + Ori * (410 + ROBOT_WIDTH))
end,
function()
	repeat
		a, b = c.GetDistance("left")
		x, y, phi = c.GetRobotPos()
		c.print(x, y, a, b)
		c.process()
	until (not c.MotionInProgress())
end)
