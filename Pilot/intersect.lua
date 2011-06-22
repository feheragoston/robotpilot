
Offset = 0;
Ori = 1;

GoSpeed = 800;
GoAcc = 600;
TurnSpeed = 6;
TurnAcc = 6;

-- CALIBRATE
dofile("Pilot/calibration.lua")
-- CALIBRATE END

if (c.GetMyColor()) then
	c.print("Kekek vagyunk");
	Offset = 3000;
	Ori = -1;
end

p.RefreshPawnPositions()

p.TurnTo(200, Offset + Ori * 400, turnSpeed, 4)
p.GoTo(200, Offset + Ori * 400, goSpeed, goAcc)

function go1()
	p.TurnToSafe(350, Offset + Ori * 800, TurnSpeed, TurnAcc)
	p.GoToSafe(350, Offset + Ori * 800, GoSpeed, GoAcc)
end
function go2()
	p.TurnToSafe(350, Offset + Ori * 2200, TurnSpeed, TurnAcc)
	p.GoToSafe(350, Offset + Ori * 2200, GoSpeed, GoAcc)
end
function go3()
	p.TurnToSafe(1400, Offset + Ori * 2200, TurnSpeed, TurnAcc)
	p.GoToSafe(1400, Offset + Ori * 2200, GoSpeed, GoAcc)
end
function go4()
	p.TurnToSafe(1400, Offset + Ori * 800, TurnSpeed, TurnAcc)
	p.GoToSafe(1400, Offset + Ori * 800, GoSpeed, GoAcc)
end

function resolveDeadpos1()
	p.GoSafe(-1000)
end

function resolveDeadpos2()
	p.GoSafe(600)
end

function resolveDeadpos3()
	p.GoSafe(-100)
end

function resolveDeadpos4()
	p.GoSafe(150)
end

function resolveDeadpos5()
	p.TurnSafe(math.pi/2)
	p.GoSafe(300)
end

function resolveDeadpos6()
	p.TurnSafe(-math.pi/2)
	p.GoSafe(300)
end

repeat
	if (not pcall(function()
		c.print("KEZDUNK");
		--p.RefreshPawnPositions()
		
		deadpos = true;
		
		if (c.simulate(go1)) then
			deadpos = false;
			go1();
		else
			c.print("go1 kihagyas");
		end
		if (c.simulate(go2)) then
			deadpos = false;
			go2();
		else
			c.print("go2 kihagyas");
		end
		if (c.simulate(go3)) then
			deadpos = false;
			go3();
		else
			c.print("go3 kihagyas");
		end
		if (c.simulate(go4)) then
			deadpos = false;
			go4();
		else
			c.print("go4 kihagyas");
		end
		
		c.music("axelfoley")
		
		if (deadpos) then
			if (c.simulate(resolveDeadpos5)) then
				resolveDeadpos5();
			elseif (c.simulate(resolveDeadpos6)) then
				resolveDeadpos6();
			elseif (c.simulate(resolveDeadpos1)) then
				resolveDeadpos1();
			elseif (c.simulate(resolveDeadpos2)) then
				resolveDeadpos2();
			elseif (c.simulate(resolveDeadpos3)) then
				resolveDeadpos3();
			elseif (c.simulate(resolveDeadpos4)) then
				resolveDeadpos4();
			end
		end
	end)) then
		p.MotionStop(MAX_DEC)
	end
until (c.GetStopButton());