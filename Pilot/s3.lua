
dofile("Pilot/calibration.lua")

dofile("Pilot/functions.lua")

p.Go(150)
x, y, phi = c.GetRobotPos()
p.TurnTo(x, 1500)
x, y, phi = c.GetRobotPos()
p.GoTo(x, 340)

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
	p.GoToSafe(x, 590)
end);
if (not status) then
	c.print("Hiba", err);
	p.MotionStop(MAX_DEC)
end

while (true) do

	local status, err = pcall(function()

		local deadpos = true;

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