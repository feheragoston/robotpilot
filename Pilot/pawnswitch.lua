
Offset = 0;
Ori = 1;
GripperGrab = 67;
pawnInGripper = false;
pawnInLeftArm = false;
pawnInRightArm = false;

dofile("Pilot/calibration.lua")

function GoToNextPawn(x, y, px, py)
	p.GripperMove(0)
	p.TurnToSafe(x, y)
	p.GoToSafe(x, y)
	p.TurnToSafe(px, py)
	p.GripperMove(90)
	p.GoToSafe(px, py)
	if (c.PawnInGripper()) then
		p.GripperMove(GripperGrab)
		pawnInGripper = true;
		p.GoSafe(-250)
	end
end

function DeployPawn(x1, y1, x2, y2)
	p.GripperMove(GripperGrab)
	p.TurnToSafe(x1, y1)
	p.GoToSafe(x1, y1)
	p.TurnToSafe(x2, y2)
	p.GripperMove(90)
	pawnInGripper = false;
	p.GoSafe(-250)
end

repeat c.process() until (c.GetStartButton());
c.StartMatch(false); -- !!!!!!!!!!!!!!!!!!
--c.print("Meccs elkezdodott");

if (c.GetMyColor()) then
	c.print("Kekek vagyunk");
	Offset = 3000;
	Ori = -1;
end

p.Go(100)

p.GripperMove(90)
p.TurnToSafe(350, Offset + Ori * 800)

pawnInGripper = false
moveFinished = false
c.runparallel(
	function()
		p.GoToSafe(350, Offset + Ori * 800)
		if (not pawnInGripper) then
			p.GripperMove(0)
			p.TurnToSafe(1700, Offset + Ori * 800)
			p.GripperMove(90)
			p.GoToSafe(1700, Offset + Ori * 800, 300, 500)
		end
		moveFinished = true
	end,
	function()
		repeat
			c.process()
		until (c.PawnInGripper() or moveFinished)
		pawnInGripper = c.PawnInGripper()
		if (c.MotionInProgress()) then
			c.print("Parasztot talaltam!")
			p.MotionStop(500)
		end
	end
)

c.print("Varunk...")
p.sleep(100)

if (pawnInGripper) then
	x, y, phi = c.GetRobotPos()
	c.print(x, y, phi)
	p.GoSafe(-200)
	p.GripperMove(0)
	x1, y1, x2, y2 = c.FindPawn(2, x + ROBOT_FRONT_PAWN * math.cos(phi), y + ROBOT_FRONT_PAWN * math.sin(phi))
	c.print(x1, y1, x2, y2)
	p.TurnTo(x2, y2)
	p.GoTo(x2, y2)
	p.Magnet(true, 1)
	p.ArmMove(true, 130)
	p.sleep(10)
	p.ArmMove(true, 0)
end

x, y, phi = c.GetRobotPos()
p.TurnToSafe(x, Offset + Ori * 800)
p.GoToSafe(x, Offset + Ori * 800)

pawnInGripper = false;
p.TurnToSafe(1700, Offset + Ori * 800)
p.GripperMove(90)

pawnInGripper = false;
c.GoToSafe(1700, Offset + Ori * 800, 300, 500)
repeat
	c.process()
until (c.PawnInGripper() or not c.MotionInProgress())
pawnInGripper = c.PawnInGripper()
if (c.MotionInProgress()) then
	p.MotionStop(500)
end

p.sleep(100)

if (pawnInGripper) then
	x, y, phi = c.GetRobotPos()
	p.GoSafe(-ROBOT_FRONT_MAX)
	p.GripperMove(0)
	x1, y1, x2, y2 = c.FindPawn(3, x + ROBOT_FRONT_PAWN * math.cos(phi), y + ROBOT_FRONT_PAWN * math.sin(phi))
	c.print(x1, y1, x2, y2)
	p.TurnTo(x2, y2)
	p.GoTo(x2, y2)
	p.Magnet(false, 1)
	p.ArmMove(false, 130)
	p.sleep(10)
	p.ArmMove(false, 0)
end

GoToNextPawn(1530, 690, 1530, 290)

priorityChange = 1; -- ennyivel kell modositanunk a prioritast
priorityChanged = 0; -- ennyiszer modositottunk mar adott priorityChange-el
while (pawnInGripper) do
	c.print("Paraszt uritese");
	x1, y1, x2, y2, target, priority = c.GetDeployPoint(1);
	if (x1) then
		if (c.simulate(DeployPawn, x1, y1, x2, y2)) then
			deadpos = false;
			DeployPawn(x1, y1, x2, y2);
			c.SetDeployPointPriority(target, 1); -- jeloljuk, hogy a mezo foglalt
			stuck = 0;
		else
			c.SetDeployPointPriority(target, priority + priorityChange);
			if (priorityChange == priorityChanged) then
				-- ha mar priorityChange+1 -szer modositottunk, noveljuk priorityChanget
				priorityChange = priorityChange + 1;
				if (priority + priorityChange >= 0) then
					-- ha a prioritas elerne a 0-t, akkor korbeertunk, nem tudunk lerakni
					c.print("Nem tudunk egyik lerako poziciohoz sem odamenni");
					c.process();
					break;
				end
				priorityChanged = 0;
			else
				priorityChanged = priorityChanged + 1;
			end
		end
	else
		c.print("Nincs tobb lerako pozicio")
		c.process();
		break;
	end
end;

GoToNextPawn(1250, 690, 1250, 290)

p.TurnToSafe(1200, 975)
p.GoToSafe(1200, 975)
p.TurnToSafe(0, 975)

p.ArmMove(true, 130)
p.Magnet(true, -1)
p.ArmMove(true, 0)
p.Magnet(true, 0)

p.TurnSafe(-math.pi)
p.GoSafe(30)

p.ArmMove(false, 90)
p.Magnet(false, -1)
p.ArmMove(false, 0)
p.Magnet(false, 0)

p.GripperMove(60)
p.ConsoleMove(110)
p.TurnSafe(-math.pi / 2)
p.GoSafe(130)
p.GripperMove(90)
p.GoSafe(-200)

--[[
p.TurnTo(1200, 975)
p.GoTo(1200, 975)
p.TurnTo(0, 975)
c.runparallel(
function()
	p.ArmMove(false, 130)
	p.Magnet(false, -1)
	p.ArmMove(false, 0)
	p.Magnet(false, 0)
end,
function()
	p.ArmMove(true, 130)
	p.Magnet(true, -1)
	p.ArmMove(true, 0)
	p.Magnet(true, 0)
end
)
]]