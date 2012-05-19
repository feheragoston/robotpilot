
-- felszedes karral
function PickUp(waitforit)
	p.runparallel(
		function()
			if (c.GetClawPos(Left) <  80) then
				c.ClawMove(Left, 90)
			end
			if (c.GetClawPos(Right) < 80) then
				c.ClawMove(Right, 90)
			end
		end,
		function()
			p.Compressor(true)
			p.sleep(200)
		end
	)
	p.ArmMove(107)
	p.sleep(400)
	p.ArmMove(10, 1000, 200)
	p.Valve(true)
	p.sleep(150)
	c.Valve(false)
	if (waitforit) then
		while (control.ValveInProgress()) do
			process()
		end
	end
end

-- urites
function Eject(waitforit)
	-- Urites
	p.ConsoleMove(140, 1000, 15)
	
	-- Vissza
	p.ConsoleMove(20, 400, 15)
	c.CalibrateConsole()
	if (waitforit) then
		while (control.CalibrateConsoleInProgress()) do
			process()
		end
	end
end

-- cel megkozelitese dist tavolsagba
function GoToTarget(x, y, dist)
	rx, ry, rphi = c.GetRobotPos()
	tphi = math.atan2(y - ry, x - rx)
	dx = math.cos(tphi) * dist
	dy = math.sin(tphi) * dist
	c.print("(functions) GoToTarget celpont", x - dx, y - dy)
	p.TurnToSafe(x - dx, y - dy)
	p.GoToSafe(x - dx, y - dy)
end

-- coin megkozelitese
-- minimum tavolsag: karhossz + cd sugar: 343 + 60 ~= 400
function GoToCoin(x, y)
	GoToTarget(x, y, 260)
end

-- coin felszedese
function PickUpFrom(x, y, waitforit)
	GoToCoin(x, y)
	PickUp(waitforit)
end

--
function DoubleTotem(bottleside)
	local xposition = 685
	local ystart = 2510
	local rightArmPos = 90
	local leftArmPos = 130
	local totemSide = Left

	if (bottleside) then
		xposition = 1315
		ystart = 2410 -- palyahoz igazitani
		rightArmPos = 130
		leftArmPos = 90
		totemSide = Right
	end

	p.MoveToSafe(xposition, ystart * Ori + Offset)

	p.TurnToSafe(xposition, 1500)
	
	p.runparallel(
		function()
			p.GripperMove(Left, rightArmPos)
			p.GripperMove(Right, leftArmPos)
		end,
		function()
			p.ClawMove(Left, rightArmPos)
			p.ClawMove(Right, leftArmPos)
		end
	)
	
	
	-- felszedes
	if (c.in_simulate()) then
		p.GoTo(xposition, 2290 * Ori + Offset) -- Palyahoz igazitani
	else
		p.GoToSafe(xposition, 2290 * Ori + Offset) -- Palyahoz igazitani
	end
	p.GripperMove(totemSide, 80, 500, 400)
	if (totemSide == true) then
		p.GripperMove(totemSide, 60, 500, 400) -- Beallitani
	end
	p.GripperMove(totemSide, 100)
	PickUp()
	
	p.MoveToSafe(xposition, 2150 * Ori + Offset)
	PickUp()
	p.ArmMove(0)
	p.Valve(false)
	p.Compressor(false)
	
	p.MoveToSafe(xposition, 1700 * Ori + Offset)
	
	if (bottleside) then
		totem2bottle = false -- GLOBAL
	else
		totem2map = false -- GLOBAL
	end
	
	p.GripperMove(totemSide, 130)
	if (c.in_simulate()) then
		p.GoTo(xposition, 1485 * Ori + Offset) -- Palyahoz igazitani
	else
		p.GoToSafe(xposition, 1485 * Ori + Offset) -- Palyahoz igazitani
	end
	p.GripperMove(totemSide, 100)
	p.GoToSafe(xposition, 900 * Ori + Offset)
	
	if (bottleside) then
		totem1bottle = false -- GLOBAL
	else
		totem1map = false -- GLOBAL
	end

	p.runparallel(
		function()
			p.MoveToSafe(1000, 400 * Ori + Offset) -- Palyahoz igazitani
		end,
		function()
			p.GripperMove(Left, 90)
		end,
		function()
			p.GripperMove(Right, 90)
		end,
		function()
			p.ClawMove(Left, 55)
		end,
		function()
			p.ClawMove(Right, 55)
		end
	)

	-- TODO forgas uriteshez, ha lila es alul vagy ha piros es felul

	p.runparallel(
		function()
			p.ConsoleMove(140, 1000, 15)
		end,
		function()
			p.GripperMove(Left, 105)
		end,
		function()
			p.GripperMove(Right, 105)
		end
	)
	
	p.runparallel(
		function()
			p.GoSafe(-300)
		end,
		function()
			ResetActuators()
		end
	)
end

-- palackposta benyomasa
function PushButton(farther)
	local yposition = 670
	if (farther) then
		yposition = 1913
	end
	x, y, phi = c.GetRobotPos()
	if (math.abs(y - (yposition * Ori + Offset)) > 50) then
		p.MoveToSafe(1700, yposition * Ori + Offset)
	end
	x, y, phi = c.GetRobotPos()
	p.TurnTo(1795, y)
	x, y, phi = c.GetRobotPos()
	p.GoTo(1795, y)
	p.Go(-50)
	p.runparallel(
		function()
			x, y, phi = c.GetRobotPos()
			p.GoTo(1820, y, 100, 75)
		end,
		function()
			p.sleep(1500)
			p.MotionStop(MAX_DEC)
		end
	)
	--p.GripperMove(Right, 30)

	if (farther) then
		button2 = false -- GLOBAL
	else
		button1 = false -- GLOBAL
	end

	--ResetActuators()
	p.Go(-220)
end

-- kar, csapok, megfogok csukva vannak-e
function ActuatorsClosed()
	if (c.GetArmPos() > 5) then
		return false
	end
	if (c.GetGripperPos(Right) > 3) then
		return false
	end
	if (c.GetGripperPos(Left) > 8) then
		return false
	end
	if (c.GetClawPos(Right) > 2) then
		return false
	end
	if (c.GetClawPos(Left) > 4) then
		return false
	end
	return true
end

-- karok, csapok, megfogok, konzol alaphelyzetbe allitasa
function ResetActuators()
	if (c.GetArmPos() > 5) then
		p.ArmMove(0)
	end

	p.runparallel(
		function()
			local lgp = c.GetGripperPos(Right)
			if (lgp < 55 and lgp > c.GetGripperPos(Left)) then
				p.GripperMove(Right, 55)
				p.GripperMove(Left, 55)
			end
			if (c.GetGripperPos(Right) > 3) then
				p.GripperMove(Right, 1)
			end
			if (c.GetGripperPos(Left) > 8) then
				p.GripperMove(Left, 6)
			end
		end,
		function()
			local lcp = c.GetClawPos(Right)
			if (lcp < 55 and lcp > c.GetClawPos(Left)) then
				p.ClawMove(Right, 55)
				p.ClawMove(Left, 55)
			end
			if (c.GetClawPos(Right) > 2) then
				p.ClawMove(Right, 0)
			end
			if (c.GetClawPos(Left) > 4) then
				p.ClawMove(Left, 2)
			end
		end,
		function()
			if (c.GetConsolePos() > 20) then
				p.ConsoleMove(20, 400, 15)
				p.CalibrateConsole()
			end
		end,
		function()
			c.Valve(false)
			c.Compressor(false)
		end
	)
end

-- Beszorulas feloldasa
function ResolveDeadpos(turn, go)
	if (turn ~= 0) then
		p.TurnSafe(turn)
	end;
	p.GoSafe(go)
end
-- Beszorulas feloldasa
