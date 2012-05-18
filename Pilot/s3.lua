
dofile("Pilot/calibration.lua")

dofile("Pilot/functions.lua")

p.Go(150)
x, y, phi = c.GetRobotPos()
p.sleep(1000)
p.TurnTo(500, 1000 * Ori + Offset)

cd1 = true
cd2 = true
totem1map = true
totem2map = true
totem1bottle = true
totem2bottle = true
button1 = true
button2 = true

c.music("start")
repeat
	p.process()
	x, y, phi = c.GetRobotPos()
until(c.GetStartButton())

c.StartMatch(true); -- !!!!!!!!!!!!!!!!!!
c.print("Meccs elkezdodott");

local status, err = pcall(function()
	p.runparallel(
		function()
			p.GripperMove(Left, 90)
		end,
		function()
			p.ClawMove(Left, 90)
			p.ClawMove(Right, 90)
		end
	)
	p.ArmMove(87)
	
	p.runparallel(
		function()
			p.Go(600) -- ???
			p.TurnSafe(-1.5165 * Ori)
		end,
		function()
			p.Compressor(true)
			repeat
				x, y, phi = c.GetRobotPos()
				p.process()
			until (y * Ori + Offset > 750) -- ???
			p.ArmMove(100, 1000, 3000)
			p.sleep(200)
			p.ArmMove(10, 1000, 200)
			p.Valve(true)
			p.sleep(100)
			p.Valve(false)
			p.ArmMove(0)
			c.Compressor(false)
			p.ClawMove(Right, 0)
			p.ClawMove(Left, 2)
			p.GripperMove(Left, 130)
		end
	)
	p.GoSafe(500) -- ???
	p.runparallel(
		function()
			x, y, phi = c.GetRobotPos()
			p.MoveToSafe(x, 400 * Ori + Offset)
		end,
		function()
			p.GripperMove(Left, 90)
		end
	)
	Eject(false)
	p.GoSafe(-300)
	
	-- gomb benyomas
	p.runparallel(
		function()
			ResetActuators()
		end,
		function()
			p.MoveToSafe(1795, 700 * Ori + Offset)
			p.Go(-50)
			p.GoTo(1795, 700 * Ori + Offset, 100, 75)
		end
	)
	p.sleep(1000)
	x, y, phi = c.GetRobotPos()
	p.Go(1700 - x);

end);
if (not status) then
	c.print("Hiba", err);
	p.MotionStop(MAX_DEC)
end

c.print(c.matchtime())

while (true) do

	local status, err = pcall(function()

		local deadpos = true;

		if (button2 and ActuatorsClosed()) then
			if (c.simulate(PushButton, true)) then
				deadpos = false
				PushButton(true)
				c.print(c.matchtime())
			end
		end

		-- BERAGADAS FELOLDASA
		if (deadpos) then
			if (not ActuatorsClosed()) then
				if (c.simulate(p.GoSafe, -200)) then
					p.GoSafe(-200)
				end
			end

			x, y, phi = c.GetRobotPos()
			y = y * Ori + Offset
			if ((x > 800 or y > 900) and c.simulate(p.MoveToSafe, 355, 665 * Ori + Offset)) then
				p.MoveToSafe(355, 665 * Ori + Offset)
			elseif ((x < 1200 or y > 900) and c.simulate(p.MoveToSafe, 1645, 715 * Ori + Offset)) then
				p.MoveToSafe(1645, 715 * Ori + Offset)
			elseif ((x > 800 or y < 2100) and c.simulate(p.MoveToSafe, 580, 2250 * Ori + Offset)) then
				p.MoveToSafe(580, 2250 * Ori + Offset)
			elseif ((x < 1200 or y < 2100) and c.simulate(p.MoveToSafe, 1400, 2250 * Ori + Offset)) then
				p.MoveToSafe(1400, 2250 * Ori + Offset)
			else

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

			end
		end

	end);
	if (not status) then
		c.print("Hiba", err);
		p.MotionStop(MAX_DEC)
	end

end