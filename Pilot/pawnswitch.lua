
arm1 = STORAGE_LEFT
arm2 = STORAGE_RIGHT
pawnInGripper = false;
pawnInLeftArm = false;
pawnInRightArm = false;

dofile("Pilot/calibration.lua")

function GoToNextPawn(x, y, px, py)
	p.GripperMove(0)
	p.TurnToSafe(x, y, turnSpeed, turnAcc)
	p.GoToSafe(x, y, goSpeed, goAcc)
	p.TurnToSafe(px, py, turnSpeed, turnAcc)
	p.GripperMove(90)
	p.GoToSafe(px, py, goSpeed, goAcc)
	if (c.PawnInGripper()) then
		p.GripperMove(GripperGrab)
		pawnInGripper = true;
		p.GoSafe(-250, goSpeed, goAcc)
	end
end

function DeployPawn(x1, y1, x2, y2)
	p.GripperMove(GripperGrab)
	p.TurnToSafe(x1, y1, turnSpeed, turnAcc)
	p.GoToSafe(x1, y1, goSpeed, goAcc)
	p.TurnToSafe(x2, y2, turnSpeed, turnAcc)
	p.GripperMove(90)
	pawnInGripper = false;
	p.GoSafe(-250, goSpeed, goAcc)
end

repeat p.process() until (c.GetStartButton());
c.StartMatch(false); -- !!!!!!!!!!!!!!!!!!
--c.print("Meccs elkezdodott");

if (c.GetMyColor()) then
	arm1 = STORAGE_RIGHT;
	arm2 = STORAGE_LEFT;
end

p.TurnTo(200, Offset + Ori * 400, turnSpeed, 4)
p.GoTo(200, Offset + Ori * 400, goSpeed, goAcc)

p.GripperMove(90)
p.TurnToSafe(350, Offset + Ori * 800, turnSpeed, 4)

pawnInGripper = false
moveFinished = false
p.runparallel(
	1000,
	function()
		p.GoToSafe(350, Offset + Ori * 800)
		if (not pawnInGripper) then
			p.GripperMove(0)
			p.TurnToSafe(1700, Offset + Ori * 800, turnSpeed, turnAcc)
			p.GripperMove(90)
			p.GoToSafe(1700, Offset + Ori * 800, 300, 500)
		end
		moveFinished = true
	end,
	function()
		repeat
			p.process()
		until (c.PawnInGripper() or moveFinished)
		pawnInGripper = c.PawnInGripper()
		if (pawnInGripper) then
			c.print("Parasztot talaltam!")
		end
		if (c.MotionInProgress()) then
			p.MotionStop(500)
		end
	end
)

c.print("Varunk...")
p.sleep(100)

if (pawnInGripper) then
	x, y, phi = c.GetRobotPos()
	c.print(x, y, phi)
	p.GoSafe(-200, goSpeed, goAcc)
	p.GripperMove(0)
	x1, y1, x2, y2 = c.FindPawn(arm1, 1, x + ROBOT_FRONT_PAWN * math.cos(phi), y + ROBOT_FRONT_PAWN * math.sin(phi))
	c.print(x1, y1, x2, y2)
	p.TurnToSafe(x2, y2, turnSpeed, turnAcc)
	p.GoToSafe(x2, y2, goSpeed, goAcc)
	p.Magnet(not c.GetMyColor(), 1)
	p.ArmMove(not c.GetMyColor(), 130)
	p.sleep(10)
	p.ArmMove(not c.GetMyColor(), 0)
	p.Go(-100, goSpeed, goAcc)
end

x, y, phi = c.GetRobotPos()
p.TurnToSafe(x, Offset + Ori * 800, turnSpeed, turnAcc)
p.GoToSafe(x, Offset + Ori * 800, goSpeed, goAcc)

pawnInGripper = false;
p.TurnToSafe(1700, Offset + Ori * 800, turnSpeed, turnAcc)
p.GripperMove(90)

pawnInGripper = false;
c.GoToSafe(1700, Offset + Ori * 800, 300, 500)
repeat
	p.process(1000)
until (c.PawnInGripper() or not c.MotionInProgress())
pawnInGripper = c.PawnInGripper()
if (c.MotionInProgress()) then
	p.MotionStop(500)
end

p.sleep(100)

if (pawnInGripper) then
	x, y, phi = c.GetRobotPos()
	p.GoSafe(-ROBOT_FRONT_MAX, goSpeed, goAcc)
	p.GripperMove(0)
	x1, y1, x2, y2 = c.FindPawn(arm2, 1, x + ROBOT_FRONT_PAWN * math.cos(phi), y + ROBOT_FRONT_PAWN * math.sin(phi))
	c.print(x1, y1, x2, y2)
	p.TurnTo(x2, y2, turnSpeed, turnAcc)
	p.GoTo(x2, y2, goSpeed, goAcc)
	p.Magnet(c.GetMyColor(), 1)
	p.ArmMove(c.GetMyColor(), 130)
	p.sleep(10)
	p.ArmMove(c.GetMyColor(), 0)
	p.Go(-100, goSpeed, goAcc)
end

GoToNextPawn(1530, Offset + Ori * 690, 1530, Offset + Ori * 290)

priorityChange = 1; -- ennyivel kell modositanunk a prioritast
priorityChanged = 0; -- ennyiszer modositottunk mar adott priorityChange-el
while (pawnInGripper) do
	c.print("Paraszt uritese");
	x1, y1, x2, y2, target, priority = c.GetDeployPoint(STORAGE_GRIPPER);
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
					p.process();
					break;
				end
				priorityChanged = 0;
			else
				priorityChanged = priorityChanged + 1;
			end
		end
	else
		c.print("Nincs tobb lerako pozicio")
		p.process();
		break;
	end
end;

GoToNextPawn(1250, Offset + Ori * 690, 1250, Offset + Ori * 290)

x1, y1, x2, y2, dist = c.FindPawn(STORAGE_LEFT, 1, 1225, Offset + Ori * 650)
if (not x1) then
	x1, y1, x2, y2, dist = c.FindPawn(STORAGE_LEFT, 1, 525, Offset + Ori * 650)
end
c.print(x1, y1, x2, y2, dist)
p.TurnTo(x2, y2, turnSpeed, turnAcc)
p.GoTo(x2, y2, goSpeed, goAcc)

p.runparallel(
function()
	p.ArmMove(true, 130)
	p.Magnet(true, -1)
	p.ArmMove(true, 0)
	p.Magnet(true, 0)
	
	p.TurnSafe(-math.pi, 4, 8)
	p.GoSafe(30, goSpeed, goAcc)

	p.ArmMove(false, 90)
	p.Magnet(false, -1)
	p.ArmMove(false, 0)
	p.Magnet(false, 0)
end,
function()
	p.GripperMove(GripperHold)
	p.ConsoleMove(120)
end)

p.TurnSafe(-math.pi / 2, 4, 8)
p.GoSafe(130, goSpeed, goAcc)
p.GripperMove(90)
p.GoSafe(-200, goSpeed, goAcc)

--[[
p.TurnTo(1200, 975)
p.GoTo(1200, 975)
p.TurnTo(0, 975)
p.runparallel(
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