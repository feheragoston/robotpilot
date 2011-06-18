
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

p.TurnToSafe(1700, Offset + Ori * 800, 10, 20)
p.GripperMove(90)

pawnInGripper = false;
c.GoToSafe(1700, Offset + Ori * 800, 400, 500)
repeat
	c.process()
until (c.PawnInGripper() or not c.MotionInProgress())
pawnInGripper = c.PawnInGripper()
if (c.MotionInProgress()) then
	p.MotionStop(500)
end

if (pawnInGripper) then
	x, y, phi = c.GetRobotPos()
	p.GoSafe(-ROBOT_FRONT_MAX)
	p.GripperMove(0)
	x1, y1, x2, y2 = c.FindPawn(3, x + ROBOT_FRONT_PAWN, y)
	c.print(x1, y1, x2, y2)
	p.TurnTo(x2, y2)
	p.GoTo(x2, y2)
	p.ArmMove(false, 130)
	p.Magnet(false, 1)
	p.sleep(10)
	p.ArmMove(false, 0)
end

x, y, phi = c.GetRobotPos()
p.TurnToSafe(x, Offset + Ori * 800, 10, 20)
p.GoToSafe(x, Offset + Ori * 800, 800, 800)

pawnInGripper = false;
p.TurnToSafe(1700, Offset + Ori * 800, 10, 20)
p.GripperMove(90)

pawnInGripper = false;
c.GoToSafe(1700, Offset + Ori * 800, 400, 500)
repeat
	c.process()
until (c.PawnInGripper() or not c.MotionInProgress())
pawnInGripper = c.PawnInGripper()
if (c.MotionInProgress()) then
	p.MotionStop(500)
end

if (pawnInGripper) then
	x, y, phi = c.GetRobotPos()
	p.GoSafe(-ROBOT_FRONT_MAX)
	p.GripperMove(0)
	x1, y1, x2, y2 = c.FindPawn(2, x + ROBOT_FRONT_PAWN, y)
	c.print(x1, y1, x2, y2)
	p.TurnTo(x2, y2)
	p.GoTo(x2, y2)
	p.ArmMove(true, 130)
	p.Magnet(true, 1)
	p.sleep(10)
	p.ArmMove(true, 0)
end

