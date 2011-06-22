
function armtest(left, speed, acc)
	p.ArmMove(left, 130, speed, acc)
	p.Magnet(left, 1)
	--p.sleep(10)
	p.ArmMove(left, 0, speed, acc)
	
	p.sleep(100)
	p.ArmMove(left, 130, speed, acc)
	p.Magnet(left, 0)
	--p.sleep(10)
	p.Magnet(left, -1)
	p.ArmMove(left, 0, speed, acc)
	p.Magnet(left, 0)
end

Offset = 0;
Ori = 1;

dofile("Pilot/calibration.lua")

repeat c.process() until (c.GetStartButton());
--c.StartMatch();
--c.print("Meccs elkezdodott");

if (c.GetMyColor()) then
	c.print("Kekek vagyunk");
	Offset = 3000;
	Ori = -1;
end

p.GoTo(250, Offset + Ori * 800)

x1, y1, x2, y2 = c.FindPawn(3)
p.TurnTo(x2, y2)
p.GoTo(x2, y2)

c.music("starwars")
armtest(false, 1000, 100)

p.Turn(math.pi)
p.Go(30)

armtest(true, 1000, 100)


--[[
c.runparallel(
function ()
	armtest(true)
end,
function ()
	armtest(false)
end)
]]