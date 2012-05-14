
dofile("Pilot/calibration.lua")
dofile("Pilot/functions.lua")

--[[
	goSpeed = 400
	goAcc = 500
	turnSpeed = 4
	turnAcc = 8
]]

p.runparallel(
function()
	repeat
		if (not p.RefreshPawnPositions()) then
			p.sleep(500)
		end
		p.process()
	until (c.GetStartButton())
	c.ValidateStartSetup()
end,
function()
	p.sleep(3000)
	c.music("start")
	repeat
		p.process()
	until(c.GetStartButton())
	
	c.StartMatch(true); -- !!!!!!!!!!!!!!!!!!
	c.print("Meccs elkezdodott");
	
	safe1Deployed = false;
	safe2Deployed = false;
	bonus1Deployed = false;

	c.SetDeployPointPriority(30, 1, STORAGE_NONE)
	c.SetDeployPointPriority(31, 1, STORAGE_NONE)
	c.SetDeployPointPriority(34, 1, STORAGE_NONE)
	c.SetDeployPointPriority(35, 1, STORAGE_NONE)
	safe1Deployed = true;
	safe2Deployed = true;
	
	if (c.PawnNearPoint(350, 800)) then
		p.TurnTo(200, Offset + Ori * 400, turnSpeed, 4)
		p.GoTo(200, Offset + Ori * 400, goSpeed, goAcc)
	else
		p.TurnTo(200, Offset + Ori * 800, turnSpeed, 4)
		p.GoTo(200, Offset + Ori * 800, goSpeed, goAcc)
	end
end)

pawnInGripper = false
pawnInLeft = false
pawnInRight = false

towerFinished = false

repeat
	local deadpos = true;
	
	c.print("Felszedo fazis indul")
	
	
	repeat
		ignoreRadius = ROBOT_RADIUS;
		local status, err = pcall(function()
			local leftIR = MAX_DISTANCE
			local rightIR = MAX_DISTANCE
			local gripperGIR = MAX_DISTANCE -- green area
			local gripperBIR = MAX_DISTANCE -- board
			
			if (not pawnInLeft) then
				leftIR, leftPX, leftPY, leftX, leftY = SearchWithArmFromBoard(true, ignoreRadius, MAX_DISTANCE)
				c.print("Left:", leftIR, leftPX, leftPY, leftX, leftY)
			end
			if (not pawnInRight) then
				rightIR, rightPX, rightPY, rightX, rightY = SearchWithArmFromBoard(false, ignoreRadius, leftIR - 10)
				c.print("Right:", rightIR, rightPX, rightPY, rightX, rightY)
			end
			if (not pawnInGripper) then
				ir = math.max(ignoreRadius, ROBOT_FRONT_MAX + PAWN_RADIUS)
				
				c.print("Gripper kereses:", ir, math.min(leftIR, rightIR) - 10)
				
				if (safe1Deployed) then
					gripperPawnType = FIG_KING
					gripperGIR, gripperGPX, gripperGPY, gripperGX, gripperGY = SearchWithGripperFromSides(gripperPawnType, ir, math.min(leftIR, rightIR) - 10)
				else
					gripperPawnType = FIG_PAWN
					gripperBIR, gripperBPX, gripperBPY, gripperBX, gripperBY = SearchWithGripperFromBoard(gripperPawnType, ir, math.min(leftIR, rightIR) - 10)
					c.print("GripperB:", gripperBIR, gripperBPX, gripperBPY, gripperBX, gripperBY)
					gripperGIR, gripperGPX, gripperGPY, gripperGX, gripperGY = SearchWithGripperFromSides(gripperPawnType, ir, math.min(leftIR, rightIR) - 10)
					gripperGIR = gripperGIR + 100 -- a zold teruletrol csak akkor szedjunk ha nagyon muszaj
				end
				c.print("GripperG:", gripperGIR, gripperGPX, gripperGPY, gripperGX, gripperGY)
			end
			
			ignoreRadius = math.min(leftIR, rightIR, gripperGIR, gripperBIR)
			c.print("ignoreRadius:", ignoreRadius)
			if (ignoreRadius < MAX_DISTANCE) then
				deadpos = false
				if (leftIR < ignoreRadius + 10) then
					c.print("Felszedes bal karral")
					c.SetPawnType(leftPX, leftPY, FIG_WENT_OVER)
					if (PickupWithArmFromBoard(true, leftX, leftY)) then
						pawnInLeft = true
						c.SetPawnType(leftPX, leftPY, FIG_PICKED_UP)
					end
				elseif (rightIR < ignoreRadius + 10) then
					c.print("Felszedes jobb karral")
					c.SetPawnType(rightPX, rightPY, FIG_WENT_OVER)
					if (PickupWithArmFromBoard(false, rightX, rightY)) then
						pawnInRight = true
						c.SetPawnType(rightPX, rightPY, FIG_PICKED_UP)
					end
				elseif (gripperBIR < ignoreRadius + 10) then
					c.print("Felszedes gripperrel tablarol")
					c.SetPawnType(gripperBPX, gripperBPY, FIG_WENT_OVER)
					if (PickupWithGripperFromBoard(gripperBX, gripperBY)) then
						pawnInGripper = gripperPawnType;
						c.SetPawnType(gripperBPX, gripperBPY, FIG_PICKED_UP)
					end
				else
					c.print("Felszedes gripperrel zold mezorol")
					c.SetPawnType(gripperGPX, gripperGPY, FIG_WENT_OVER)
					if (PickupWithGripperFromSides(gripperGPX, gripperGPY, gripperGX, gripperGY)) then
						pawnInGripper = gripperPawnType;
						c.SetPawnType(gripperGPX, gripperGPY, FIG_PICKED_UP)
					end
				end
				if (c.PawnsNearby()) then
					if (c.simulate(function() p.GoSafe(-250, goSpeed, goAcc); end)) then
						p.GoSafe(-250, goSpeed, goAcc)
					elseif (c.simulate(function() p.GoSafe(-200, goSpeed, goAcc); end)) then
						p.GoSafe(-200, goSpeed, goAcc)
					elseif (c.simulate(function() p.GoSafe(-100, goSpeed, goAcc); end)) then
						p.GoSafe(-100, goSpeed, goAcc)
					end
				end
			end
		end);
		if (not status) then
			c.print("Hiba", err);
			p.MotionStop(MAX_DEC)
		end
		p.process()
	until (ignoreRadius == MAX_DISTANCE)
	
	c.print("Felszedo fazis kesz: ", pawnInLeft, pawnInRight, pawnInGripper)
	
	local ignorePriority = -1000
	local hasDeployPoint = true
	
	while (pawnInGripper and hasDeployPoint) do
		local status, err = pcall(function()
			c.print("Paraszt uritese");
			x1, y1, x2, y2, target, ignorePriority = c.GetDeployPoint(STORAGE_GRIPPER, ignorePriority);
			if (x1) then
				if (gripperPawnType == FIG_PAWN or (not pawnInLeft and not pawnInRight)) then
					if (c.simulate(DeployFromGripper, x1, y1, x2, y2)) then
						deadpos = false
						DeployFromGripper(x1, y1, x2, y2);
						--c.SetDeployPointPriority(target, 1, STORAGE_GRIPPER); -- jeloljuk, hogy a mezo foglalt
						pawnInGripper = false
						if (target == 30 or target == 35) then
							safe1Deployed = true
						elseif (target == 32 or target == 33) then
							bonus1Deployed = true
						elseif (target == 31 or target == 34) then
							safe2Deployed = true
						end
					else
						--
					end
				else
					if (pawnInLeft) then
						local tx, ty = c.GetStoragePos(STORAGE_LEFT, x2, y2)
						local trx, try = c.GetStoragePos(STORAGE_RIGHT, x2, y2)
						if (tx) then
							if (pawnInRight) then
								if (c.simulate(DeployFullTower, true, tx, ty)) then
									c.music("starwars")
									deadpos = false
									DeployFullTower(true, tx, ty)
									--c.SetDeployPointPriority(target, 1, STORAGE_GRIPPER); -- a tetejere gripperrel rakunk
									towerFinished = true;
									pawnInLeft = false;
									pawnInRight = false;
									pawnInGripper = false;
								elseif (trx and c.simulate(DeployFullTower, false, trx, try)) then
									c.music("starwars")
									deadpos = false
									DeployFullTower(false, trx, try)
									--c.SetDeployPointPriority(target, 1, STORAGE_GRIPPER); -- a tetejere gripperrel rakunk
									towerFinished = true;
									pawnInLeft = false;
									pawnInRight = false;
									pawnInGripper = false;
								end
							else
								if (c.simulate(DeployHalfTower, true, tx, ty)) then
									c.music("axelfoley")
									deadpos = false
									DeployHalfTower(true, tx, ty)
									--c.SetDeployPointPriority(target, 1, STORAGE_GRIPPER); -- a tetejere gripperrel rakunk
									towerFinished = true;
									pawnInLeft = false;
									pawnInGripper = false;
								end
							end
						end
					elseif (pawnInRight) then
						local tx, ty = c.GetStoragePos(STORAGE_RIGHT, x2, y2)
						if (tx) then
							if (c.simulate(DeployHalfTower, false, tx, ty)) then
								c.music("axelfoley")
								deadpos = false
								DeployHalfTower(false, tx, ty)
								--c.SetDeployPointPriority(target, 1, STORAGE_GRIPPER); -- a tetejere gripperrel rakunk
								towerFinished = true;
								pawnInRight = false;
								pawnInGripper = false;
							end
						end
					end
				end
			else
				c.print("Nincs tobb lerako pozicio")
				p.process()
				hasDeployPoint = false
				--TODO
			end
		end);
		if (not status) then
			c.print("Hiba", err)
			p.MotionStop(MAX_DEC)
			ignorePriority = -1000
		end
		p.process()
	end
	
	c.print("Lerako fazis kesz")
	
	local status, err = pcall(function()
		while (deadpos) do
			c.print("Beszorultunk, feloldas indul")
			local turn = 0
			if (math.random() > 0.3) then
				turn = (math.random() - 0.5) * math.pi * 2
			end
			local go = math.random(-1000, 1000)
			if (c.simulate(ResolveDeadpos, turn, go)) then
				c.print("Beszorulas feloldva")
				deadpos = false
				ResolveDeadpos(turn, go)
			end
		end
	end);
	if (not status) then
		c.print("Hiba", err);
		p.MotionStop(MAX_DEC)
	end

until (towerFinished)

p.Go(-50)
p.ConsoleMove(0)
p.Go(80)
p.GripperMove(GripperGrab)

goSpeed = 600
goAcc = 500
turnSpeed = 8
turnAcc = 8

function backOff()
	p.MotionStop(MAX_DEC)
	if (c.simulate(p.GoSafe, -100)) then
		p.Go(-100)
	end 
end

status, err = pcall(function()
	tx = 525
	ty = 1675
	p.TurnToSafe(tx, Offset + Ori * ty, turnSpeed, turnAcc)
	p.GoToSafe(tx, Offset + Ori * ty, goSpeed, goAcc)
end)
if (not status) then
	c.print("Hiba", err);
	backOff()
end

status, err = pcall(function()
	tx = tx + 700
	ty = ty + 700
	p.TurnToSafe(tx, Offset + Ori * ty, turnSpeed, turnAcc)
	p.GoToSafe(tx, Offset + Ori * ty, goSpeed, goAcc)
end)
if (not status) then
	c.print("Hiba", err);
	backOff()
end

status, err = pcall(function()
	tx = tx + 350
	ty = ty - 350
	p.TurnToSafe(tx, Offset + Ori * ty, turnSpeed, turnAcc)
	p.GoToSafe(tx, Offset + Ori * ty, goSpeed, goAcc)
end)
if (not status) then
	c.print("Hiba", err);
	backOff()
end

status, err = pcall(function()
	ty = ty - 700
	p.TurnToSafe(tx, Offset + Ori * ty, turnSpeed, turnAcc)
	p.GoToSafe(tx, Offset + Ori * ty, goSpeed, goAcc)
end)
if (not status) then
	c.print("Hiba", err);
	backOff()
end

status, err = pcall(function()
	tx = tx - 700
	ty = ty - 700
	p.TurnToSafe(tx, Offset + Ori * ty, turnSpeed, turnAcc)
	p.GoToSafe(tx, Offset + Ori * ty, goSpeed, goAcc)
end)
if (not status) then
	c.print("Hiba", err);
	backOff()
end

status, err = pcall(function()
	tx = tx - 350
	ty = ty + 350
	p.TurnToSafe(tx, Offset + Ori * ty, turnSpeed, turnAcc)
	p.GoToSafe(tx, Offset + Ori * ty, goSpeed, goAcc)
end)
if (not status) then
	c.print("Hiba", err);
	backOff()
end

deployed = false

function deploy1()
	p.TurnToSafe(525, Offset + Ori * 1850, turnSpeed, turnAcc)
	p.GoToSafe(525, Offset + Ori * 1850, goSpeed, goAcc)
	p.TurnToSafe(525, Offset + Ori * 3000, turnSpeed, turnAcc)
	p.GripperMove(70)
end
function deploy2()
	p.TurnToSafe(1225, Offset + Ori * 1850, turnSpeed, turnAcc)
	p.GoToSafe(1225, Offset + Ori * 1850, goSpeed, goAcc)
	p.TurnToSafe(1225, Offset + Ori * 3000, turnSpeed, turnAcc)
	p.GripperMove(70)
end
function deploy3()
	p.TurnToSafe(1750, Offset + Ori * 1325, turnSpeed, turnAcc)
	p.GoToSafe(1750, Offset + Ori * 1325, goSpeed, goAcc)
	p.TurnToSafe(2100, Offset + Ori * 1325, turnSpeed, turnAcc)
	p.GripperMove(70)
end
function deploy4()
	p.TurnToSafe(525, Offset + Ori * 800, turnSpeed, turnAcc)
	p.GoToSafe(525, Offset + Ori * 800, goSpeed, goAcc)
	p.TurnToSafe(525, Offset + Ori * 0, turnSpeed, turnAcc)
	p.GripperMove(70)
end

while (not deployed) do
	deadpos = true
	status, err = pcall(function()
		if (c.simulate(deploy1)) then
			deadpos = false
			deploy1()
			deployed = true
		elseif (c.simulate(deploy2)) then
			deadpos = false
			deploy2()
			deployed = true
		elseif (c.simulate(deploy3)) then
			deadpos = false
			deploy3()
			deployed = true
		elseif (c.simulate(deploy4)) then
			deadpos = false
			deploy4()
			deployed = true
		end
		
		while (deadpos) do
			c.print("Beszorultunk, feloldas indul")
			local turn = 0
			if (math.random() > 0.3) then
				turn = (math.random() - 0.5) * math.pi * 2
			end
			local go = math.random(-1000, 1000)
			if (c.simulate(ResolveDeadpos, turn, go)) then
				c.print("Beszorulas feloldva")
				deadpos = false
				ResolveDeadpos(turn, go)
			end
		end

	end)
	if (not status) then
		c.print("Hiba", err);
		p.MotionStop(MAX_DEC)
	end
end
