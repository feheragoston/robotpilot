Offset = 0;
Ori = 1;

dofile("Pilot/calibration.lua")

repeat p.process() until (c.GetStartButton());
--c.StartMatch();
--c.print("Meccs elkezdodott");

if (c.GetMyColor()) then
	c.print("Kekek vagyunk");
	Offset = 3000;
	Ori = -1;
end

p.GoTo(225, Offset + Ori * (410 + ROBOT_WIDTH))
p.TurnTo(1700, Offset + Ori * (410 + ROBOT_WIDTH))

p.runparallel(
function()
	p.GoTo(1700, Offset + Ori * (410 + ROBOT_WIDTH), 200, 400)
end,
function()
	repeat
		a, b = c.GetDistance("left")
		x, y, phi = c.GetRobotPos()
		c.print(x, y, a, b)
		p.process()
	until (not c.MotionInProgress())
end)
