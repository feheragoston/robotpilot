
PI = 3.141592;
Offset = 0;
Ori = 1;
GripperGrab = 67;

dofile("Pilot/calibration.lua")

repeat c.process() until (c.GetStartButton());
c.StartMatch();
c.print("Meccs elkezdodott");

homologated = false;

if (c.GetMyColor()) then
	c.print("Kekek vagyunk");
	Offset = 3000;
	Ori = -1;
end

p.GoTo(250, Offset + Ori * 800)

-- Beszorulas feloldasa
function resolveDeadpos(turn, go)
	if (turn ~= 0) then
		p.TurnSafe(turn)
	end;
	p.GoSafe(go)
end
-- Beszorulas feloldasa

function homologate()
	p.TurnToSafe(1250, Offset + Ori * 800)
	p.GoToSafe(1250, Offset + Ori * 800)
	p.TurnToSafe(1250, Offset + Ori * 300)
	p.GripperMove(90)
	p.GoToSafe(1250, Offset + Ori * 300)
	
	if (c.PawnInGripper()) then
		c.print("Van paraszt!");
	end
	p.GripperMove(GripperGrab)
	x, y, phi = c.GetRobotPos();
	p.GoSafe(-500)
	p.GripperMove(90)
	p.GoSafe(-300)
end

local errorNum = 0;
repeat
	local status, err = pcall(function()
		c.print("KEZDUNK");
		local deadpos = true;

		if (c.simulate(homologate)) then
			deadpos = false;
			homologate();
			homologated = true;
		end		

		if (deadpos) then
			local deadPosResolved = false;
			repeat
				local turn = 0;
				if (math.random() > 0.3) then
					turn = (math.random() - 0.5) * math.pi * 2;
				end
				local go = math.random(-1000, 1000);
				if (c.simulate(resolveDeadpos, turn, go)) then
					deadPosResolved = true;
					resolveDeadpos(turn, go);
				end
			until (deadPosResolved);
		end
		
	end);
	if (not status) then
		c.print("Hiba", err);
		errorNum = errorNum + 1;
		if (errorNum == 5) then
			errorNum = 0;
			c.print("Motor powercycle kihagyas");
			--[[
			c.print("Motortap kikapcsolas");
			c.MotorSupply(false)
			p.sleep(5 * 1000 * 1000)
			c.print("Motortap bekapcsolas");
			c.MotorSupply(true)
			p.sleep(2 * 1000 * 1000)
			c.print("Motor powercycle vege");
			]]
		end
		p.MotionStop(2000)
	end
until (homologated or c.GetStopButton());
