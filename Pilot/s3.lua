
dofile("Pilot/calibration.lua")

dofile("Pilot/functions.lua")

p.Go(50)
p.TurnTo(250, 274 * Ori + Offset)
p.sleep(500)
p.GoTo(250, 274 * Ori + Offset)
p.sleep(500)
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
rose = true

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
			p.Turn(-1.5165 * Ori)
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
			p.GripperMove(Left, 130)
		end
	)
	p.runparallel(
		function()
			p.Go(500) -- ???
		end,
		function()
			p.ClawMove(Right, 0)
			p.ClawMove(Left, 2)
		end
	)
	p.runparallel(
		function()
			x, y, phi = c.GetRobotPos()
			p.MoveTo(x, 400 * Ori + Offset)
		end,
		function()
			p.GripperMove(Left, 90)
		end
	)
	p.runparallel(
		function()
			x, y, phi = c.GetRobotPos()
			p.GoTo(x, 350 * Ori + Offset, 500, 100)
		end,
		function()
			p.GripperMove(Left, 130)
		end
	)
	Eject(false)
	c.print("Meccs ido ejectig: ", c.matchtime() / 1000)
	c.GripperMove(Left, 90)

	local loopStart = c.gettimeofday()
	repeat
		local x, y, phi = c.GetRobotPos()
		local status, err = pcall(function()
			p.GoSafe((y * Ori + Offset) - 700)
		end);
		if (not status) then
			c.print("Hiba", err);
			p.MotionStop(MAX_DEC)
		end
		x, y, phi = c.GetRobotPos()
		y = y * Ori + Offset
	until (y > 680 or c.getelapsedtime(loopStart) > 90000)
	
	-- gomb benyomas
	ResetActuators()

	local loopStart = c.gettimeofday()
	repeat
		local x, y, phi = c.GetRobotPos()
		local status, err = pcall(function()
			p.MoveToSafe(1795, 700 * Ori + Offset)
		end);
		if (not status) then
			c.print("Hiba", err);
			p.MotionStop(MAX_DEC)
		end
		x, y, phi = c.GetRobotPos()
		y = y * Ori + Offset
	until (x > 1775 or c.getelapsedtime(loopStart) > 10000)
	p.Go(-50)

	x, y, phi = c.GetRobotPos()
	y = y * Ori + Offset
	if (x > 1725) then
		button1 = false
		p.runparallel(
			function()
				x, y, phi = c.GetRobotPos()
				p.GoTo(1810, y, 100, 75)
			end,
			function()
				p.sleep(1500)
				p.MotionStop(MAX_DEC)
			end
		)
		p.sleep(1000)
		x, y, phi = c.GetRobotPos()
		p.Go(1700 - x);
		c.print("Meccs ido elso gombig: ", c.matchtime() / 1000)
	end

end);
if (not status) then
	c.print("Hiba", err);
	p.MotionStop(MAX_DEC)
end

c.print("Meccs ido elso ciklusig: ", c.matchtime() / 1000)

while (true) do

	local status, err = pcall(function()

		local deadpos = true;

		if (button1 and ActuatorsClosed()) then
			if (c.simulate(PushButton, false)) then
				deadpos = false
				PushButton(false)
				c.print("Meccs ido elso gombig: ", c.matchtime() / 1000)
			end
		end

		if (rose) then
			rose = false
			if (c.simulate(PickUpFrom, 1700, 1400 * Ori + Offset, false)) then
				deadpos = false
				p.runparallel(
					function()
						p.MoveToSafe(1700, 1000 * Ori + Offset)
					end,
					function()
						p.sleep(1000)
						p.ClawMove(Left, 90)
						p.ClawMove(Right, 90)
						p.ArmMove(75)
					end
				)
				local cddist = 85
				PickUpFrom(1700, (1520 - cddist) * Ori + Offset, false)
				PickUpFrom(1700 + cddist, 1520 * Ori + Offset, false)
				PickUpFrom(1700 - cddist, 1520 * Ori + Offset, false)
				PickUpFrom(1700, (1520 + cddist) * Ori + Offset, false)
				ResetActuators()
				c.print("Meccs ido rozsa vegeig: ", c.matchtime() / 1000)
				p.MoveToSafe(1700, 1963 * Ori + Offset)
				local x, y, phi = c.GetRobotPos()
				y = y * Ori + Offset
				p.Turn(math.pi / 2 * Ori - phi)
				p.Go(1913 - y)
			end
		end

		if (button2 and ActuatorsClosed()) then
			if (c.simulate(PushButton, true)) then
				deadpos = false
				PushButton(true)
				c.print("Meccs ido masodik gombig: ", c.matchtime() / 1000)
			end
		end

		if (totem1bottle) then
			x, y, phi = c.GetRobotPos()
			local doit = true
			if (math.abs(x - 1600) > 50 or math.abs(y - 1913) > 50) then
				doit = false
				if (c.simulate(p.MoveToSafe, 1600, 1913 * Ori + Offset)) then
					deadpos = false
					p.MoveToSafe(1600, 1913 * Ori + Offset)
					doit = true
				end
			end
			if (doit) then
				deadpos = false
				totem1bottle = false
				if (c.simulate(p.MoveToSafe, 1315, 1520 * Ori + Offset)) then
					p.runparallel(
						function()
							p.MoveToSafe(1315, 1520 * Ori + Offset)
						end,
						function()
							p.sleep(1000)
							p.ClawMove(Left, 90)
							p.ClawMove(Right, 90)
						end,
						function()
							p.sleep(1000)
							p.GripperMove(Left, 90)
							p.GripperMove(Right, 90)
						end
					)
					p.runparallel(
						function()
							p.TurnToSafe(1315, 940 * Ori + Offset)
						end,
						function()
							p.GripperMove(Right, 100)
						end,
						function()
							p.ClawMove(Right, 130)
						end
					)
					p.GoToSafe(1315, 940 * Ori + Offset)

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
					if (c.GetMyColor() == PURPLE) then
						p.Go(-300)
						ResetActuators()
						p.Turn(-math.pi)
						p.Go(-300)
						p.ConsoleMove(140, 1000, 15)
						p.Go(300)
					else
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
						p.Go(-300)
					end

					c.print("Meccs ido totem1 uritesig: ", c.matchtime() / 1000)

					ResetActuators()
				end
			end
		end

		-- BERAGADAS FELOLDASA
		if (deadpos) then
			if (not ActuatorsClosed()) then
				if (c.simulate(p.GoSafe, -200)) then
					p.GoSafe(-200)
					ResetActuators()
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