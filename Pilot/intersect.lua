
Offset = 0;
Ori = 1;

-- CALIBRATE
dofile("Pilot/calibration.lua")
-- CALIBRATE END

if (c.GetMyColor()) then
	c.print("Kekek vagyunk");
	Offset = 3000;
	Ori = -1;
end

p.RefreshPawnPositions()

p.Go(100)

function go1()
	p.TurnToSafe(350, Offset + Ori * 800)
	p.GoToSafe(350, Offset + Ori * 800)
end
function go2()
	p.TurnToSafe(350, Offset + Ori * 2200)
	p.GoToSafe(350, Offset + Ori * 2200)
end
function go3()
	p.TurnToSafe(1600, Offset + Ori * 2200)
	p.GoToSafe(1600, Offset + Ori * 2200)
end
function go4()
	p.TurnToSafe(1600, Offset + Ori * 800)
	p.GoToSafe(1600, Offset + Ori * 800)
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
		p.RefreshPawnPositions()
		
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
		p.MotionStop(2000)
	end
until (c.GetStopButton());