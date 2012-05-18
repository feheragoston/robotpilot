
dofile("Pilot/calibration.lua")

dofile("Pilot/functions.lua")

p.Go(30)
x, y, phi = c.GetRobotPos()
p.TurnTo(x, 1500)
p.Go(140)
p.TurnToSafe(685, 2510 * Ori + Offset)
p.Go(-15)

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
until(c.GetStartButton())

c.StartMatch(true); -- !!!!!!!!!!!!!!!!!!
c.print("Meccs elkezdodott");

if (c.GoTo(510, 2190 * Ori + Offset, 1000, 3000)) then -- finomitani
	while (c.MotionInProgress() or c.MotionStopInProgress()) do
		if (c.MotionInProgress()) then
			for i = 1, OPPONENT_NUM do
				local x, y, validity = c.GetOpponentPos(i-1)
				if (validity > SONAR_TIMEOUT or y * Ori + Offset < 2500 or x > 500) then
					c.MotionStop(MAX_DEC)
				end
			end
		end
		local x, y, phi = c.GetRobotPos()
		if (cd1 and y * Ori + Offset > 600) then
			cd1 = false
		elseif (cd2 and y * Ori + Offset > 1600) then
			cd2 = false
		end
		p.process()
	end
end

while (true) do

	local status, err = pcall(function()
	
		local deadpos = true;

		if (cd1) then
			if (c.simulate(PickUpFrom, 500, 1000 * Ori + Offset)) then
				deadpos = false
				PickUpFrom(500, 1000 * Ori + Offset)
			end
		end
		
		if (cd2) then
			if (c.simulate(PickUpFrom, 500, 2000 * Ori + Offset)) then
				deadpos = false
				PickUpFrom(500, 2000 * Ori + Offset)
			end
		end
	
		if (totem1map and totem2map) then
			if (c.simulate(DoubleTotem, false)) then
				deadpos = false
				DoubleTotem(false)
			end
		end
		
		if (totem1bottle and totem2bottle) then
			if (c.simulate(DoubleTotem, true)) then
				deadpos = false
				DoubleTotem(true)
			end
		end
		
		if (button1) then
			if (c.simulate(PushButton, false)) then
				deadpos = false
				PushButton(false)
			end
		end
		
		if (button2) then
			if (c.simulate(PushButton, true)) then
				deadpos = false
				PushButton(true)
			end
		end

		if (deadpos) then
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