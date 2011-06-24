function PickupWithGripperFromBoard(x, y)
	p.TurnToSafe(x, y, turnSpeed, turnAcc)
	p.GripperMove(90)
	c.GoToSafe(x, y, goSpeed, goAcc)
	repeat
		p.process()
	until (c.PawnInGripper() or not c.MotionInProgress())
	local pawnInGripper = c.PawnInGripper()
	if (c.MotionInProgress()) then
		p.MotionStop(500)
	end
	if (pawnInGripper) then
		p.GripperMove(GripperGrab)
		return true
	end
	return false
end

function SearchWithGripperFromBoard(ignoreRadius, type)
	local px, py, x, y
	while (true) do
		px, py, ignoreRadius = c.FindPawn(type, ignoreRadius);
		x, y = c.GetStoragePos(STORAGE_GRIPPER, px, py)
		if (x) then
			-- a zold mezokrol nem keresunk
			if (py > 400 and py < 2600 and c.simulate(PickupWithGripperFromBoard, x, y)) then
				return ignoreRadius, px, py, x, y
			else
				ignoreRadius = ignoreRadius + 1
			end
		else
			return MAX_DISTANCE
		end
	end
end

function PickupWithGripperFromSides(px, py, x, y)
	p.GripperMove(0)
	p.TurnToSafe(x, y, turnSpeed, turnAcc)
	p.GoToSafe(x, y, goSpeed, goAcc)
	p.TurnToSafe(px, py, turnSpeed, turnAcc)
	p.GripperMove(90)
	c.GoToSafe(px, py, goSpeed, 500)
	repeat
		p.process()
	until (c.PawnInGripper() or not c.MotionInProgress())
	local pawnInGripper = c.PawnInGripper()
	if (c.MotionInProgress()) then
		p.MotionStop(500)
	end
	if (pawnInGripper) then
		p.GripperMove(GripperGrab)
		return true
	end
	return false
end

function SearchWithGripperFromSides(ignoreRadius, type)
	local px, py, x, y
	while (true) do
		px, py, ignoreRadius = c.FindPawn(type, ignoreRadius);
		x, y, px, py = c.GetStoragePos(4, px, py)
		if (x) then
			-- csak a zold mezokrol keresunk
			if ((py < 400 or py > 2600) and c.simulate(PickupWithGripperFromSides, px, py, x, y)) then
				return ignoreRadius, px, py, x, y
			else
				ignoreRadius = ignoreRadius + 1
			end
		else
			return MAX_DISTANCE
		end
	end
end

function PickupWithArmFromBoard(left, x, y)
	p.TurnToSafe(x, y, turnSpeed, turnAcc)
	p.GoToSafe(x, y, goSpeed, goAcc)
	local side = "right"
	if (left) then
		side = "left"
	end
	local low, high = c.GetDistance(side)
	c.print("PickupWithArmFromBoard 1", low, high)
	if (low < 100) then
		p.Magnet(left, 1)
		p.ArmMove(left, 130)
		p.sleep(10)
		p.ArmMove(left, 0)
		
		low, high = c.GetDistance(side)
		c.print("PickupWithArmFromBoard 2", low, high)
		if (low < 90) then
			p.Magnet(left, 1)
			p.ArmMove(left, 130)
			p.Turn(0.2)
			p.Turn(-0.4)
			p.Turn(0.2)
			p.ArmMove(left, 0)
			low, high = c.GetDistance(side)
			c.print("PickupWithArmFromBoard 3", low, high)
			if (low < 75) then
				return false
			end
			return true
		end
		return true
	end
	return false
end

function SearchWithArmFromBoard(left, ignoreRadius)
	local arm = STORAGE_RIGHT
	local px, py, x, y
	if (left) then
		arm = STORAGE_LEFT
	end
	while (true) do
		px, py, ignoreRadius = c.FindPawn(FIG_PAWN, ignoreRadius)
		x, y = c.GetStoragePos(arm, px, py)
		if (x) then
			-- a zold mezokrol nem keresunk
			if (py > 400 and py < 2600 and c.simulate(PickupWithArmFromBoard, left, x, y)) then
				return ignoreRadius, px, py, x, y
			else
				ignoreRadius = ignoreRadius + 1
			end
		else
			return MAX_DISTANCE
		end
	end
end

function DeployFromArm(left)
	p.ArmMove(left, 130)
	p.Magnet(left, -1)
	p.ArmMove(left, 0)
	p.Magnet(left, 0)
end

function DeployFromGripper(x1, y1, x2, y2)
	p.GripperMove(GripperGrab)
	p.TurnToSafe(x1, y1, turnSpeed, turnAcc)
	p.GoToSafe(x1, y1, goSpeed, goAcc)
	p.TurnToSafe(x2, y2, turnSpeed, turnAcc)
	p.GripperMove(90)
end

function DeployFullTower(x, y)
	p.TurnToSafe(x, y, turnSpeed, turnAcc)
	
	p.runparallel(
	function()
		p.GoToSafe(x, y, goSpeed, goAcc)
		
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
end

function DeployHalfTower(left, x, y)
	p.TurnToSafe(x, y, turnSpeed, turnAcc)
	
	p.runparallel(
	function()
		p.GoToSafe(x, y, goSpeed, goAcc)
		
		p.ArmMove(left, 130)
		p.Magnet(left, -1)
		p.ArmMove(left, 0)
		p.Magnet(left, 0)
	end,
	function()
		p.GripperMove(GripperHold)
		p.ConsoleMove(70)
	end)
	
	if (left) then
		p.TurnSafe(math.pi / 2, 4, 8)
	else
		p.TurnSafe(-math.pi / 2, 4, 8)
	end
	p.GoSafe(130, goSpeed, goAcc)
	p.GripperMove(90)
end
