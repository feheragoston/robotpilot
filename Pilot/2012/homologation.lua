
dofile("Pilot/calibration.lua")
dofile("Pilot/functions.lua")

p.Go(50, 100, 100)
p.TurnTo(300, Offset + Ori * 790)

c.music("start")
repeat p.process() until (c.GetStartButton());
c.StartMatch();
c.print("Meccs elkezdodott");

local status, err = pcall(function()

	-- kimegy
	p.GoToSafe(300, Offset + Ori * 790)
	
end);
if (not status) then
	c.print("Hiba", err);
	p.MotionStop(MAX_DEC)
end

local status, err = pcall(function()

	-- aranyat betol
	p.TurnToSafe(860, 720 * Ori + Offset)
	if (c.simulate(p.GoToSafe, 860, 720 * Ori + Offset)) then
		p.GoToSafe(860, 720 * Ori + Offset)
	end
	if (c.simulate(p.TurnToSafe, 860, 500 * Ori + Offset)) then
		p.TurnToSafe(860, 500 * Ori + Offset)
	end
	if (c.simulate(p.GoToSafe, 860, 500 * Ori + Offset)) then
		p.GoToSafe(860, 500 * Ori + Offset)
	end
	if (c.simulate(p.GoSafe, -220)) then
		repeat p.GoSafe(-220) until (c.GetMotionError() == 0)
	end
	
end);
if (not status) then
	c.print("Hiba", err);
	p.MotionStop(MAX_DEC)
end

local status, err = pcall(function()

	-- gomb benyomas
	if (c.simulate(PushButton, false)) then
		PushButton(false)
	end

end);
if (not status) then
	c.print("Hiba", err);
	p.MotionStop(MAX_DEC)
end

local status, err = pcall(function()

	-- tavolabbi gomb benyomasa
	if (c.simulate(PushButton, true)) then
		PushButton(true)
	end

end);
if (not status) then
	c.print("Hiba", err);
	p.MotionStop(MAX_DEC)
end
