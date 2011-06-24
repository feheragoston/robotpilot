PI = 3.141592;
Offset = 0;
Ori = 1;
GripperGrab = 67;
pawnInGripper = false;
pawnInLeftArm = false;
pawnInRightArm = false;

-- Beszorulas feloldasa
function resolveDeadpos(turn, go)
	if (turn ~= 0) then
		p.TurnSafe(turn)
	end;
	p.GoSafe(go)
end
-- Beszorulas feloldasa

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

function PickNextPawnWithArm(x, y, left)
	p.TurnToSafe(x, y)
	p.GoToSafe(x, y)
	p.Magnet(left, 1)
	p.ArmMove(left, 130)
	p.ArmMove(left, 0)
	if (left) then
		pawnInLeftArm = true;
	else
		pawnInRightArm = true;
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

function DeployPawnFromArm(x1, y1, x2, y2, left)
	p.TurnToSafe(x1, y1)
	p.GoToSafe(x1, y1)
	p.TurnToSafe(x2, y2)
	p.ArmMove(left, 130)
	p.Magnet(left, -1)
	p.ArmMove(left, 0)
	p.Magnet(left, 0)
	if (left) then
		pawnInLeftArm = false;
	else
		pawnInRightArm = false;
	end
end

dofile("Pilot/calibration.lua")

repeat p.process() until (c.GetStartButton());
c.StartMatch(false); -- !!!!!!!!!!!!!!!!!!
--c.print("Meccs elkezdodott");

if (c.GetMyColor()) then
	c.print("Kekek vagyunk");
	Offset = 3000;
	Ori = -1;
end

p.RefreshPawnPositions()

p.GoTo(250, Offset + Ori * 800)

local stuck = 0;
repeat
	local status, err = pcall(function()
		c.print("KEZDUNK");
		local deadpos = true;
		stuck = stuck + 1;
		p.RefreshPawnPositions()
		
		local ignoreRadius = ROBOT_FRONT_MAX + PAWN_RADIUS;
		
		--[[
		while (not pawnInLeftArm) do
			c.print("Paraszt keresese bal karba");
			p.RefreshPawnPositions()
			px, py, ignoreRadius = c.FindPawn(FIG_PAWN, ignoreRadius);
			tx, ty = c.GetStoragePos(STORAGE_LEFT, px, py)
			if (tx) then
				if (c.Simulate(PickNextPawnWithArm, tx, ty, true)) then
					deadpos = false;
					PickNextPawnWithArm(tx, ty, true);
					stuck = 0;
				else
					ignoreRadius = ignoreRadius + 1;
				end
			else
				c.print("Nincs tobb paraszt bal karba");
				break;
			end
		end;
		
		while (pawnInLeftArm) do
			c.print("Paraszt uritese bal karbol");
			x1, y1, x2, y2, target, priority = c.GetDeployPoint(STORAGE_LEFT);
			if (x1) then
				if (c.simulate(DeployPawnFromArm, x1, y1, x2, y2, true)) then
					deadpos = false;
					DeployPawnFromArm(x1, y1, x2, y2, true);
					c.SetDeployPointPriority(target, 1); -- jeloljuk, hogy a mezo foglalt
					stuck = 0;
				else
					c.SetDeployPointPriority(target, -1);
					-- TODO
				end
			else
				c.print("Nincs tobb lerako pozicio bal karhoz")
				p.process()
				break;
			end
		end;
		]]
		
		while (not pawnInGripper) do
			c.print("Paraszt keresese");
			p.RefreshPawnPositions()
			px, py, ignoreRadius = c.FindPawn(FIG_NOTHING, ignoreRadius);
			x, y, px, py = c.GetStoragePos(4, px, py)
			if (x) then
				if (c.simulate(GoToNextPawn, x, y, px, py)) then
					deadpos = false;
					GoToNextPawn(x, y, px, py);
					stuck = 0;
				else
					ignoreRadius = ignoreRadius + 1;
				end
			else
				c.print("Nincs tobb paraszt!");
				break;
			end
		end;
		
		local priorityChange = 1; -- ennyivel kell modositanunk a prioritast
		local priorityChanged = 0; -- ennyiszer modositottunk mar adott priorityChange-el
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
		
		if (stuck > 5) then
			deadpos = true;
			stuck = 0;
		end;
		
		if (deadpos) then
			local deadPosResolved = false;
			repeat
				c.print("Beragadas feloldasa");
				p.RefreshPawnPositions()
				local turn = 0;
				if (math.random() > 0.3) then
					turn = (math.random() - 0.5) * math.pi * 2;
				end
				local go = math.random(-1000, 1000);
				if (c.simulate(resolveDeadpos, turn, go)) then
					resolveDeadpos(turn, go);
					deadPosResolved = true;
				end
			until (deadPosResolved);
		end
		
	end);
	if (not status) then
		c.print("Hiba", err);
		p.MotionStop(MAX_DEC)
	end
	p.process()
until (c.GetStopButton());