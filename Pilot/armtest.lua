
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

repeat p.process() until (c.GetStartButton());
--c.StartMatch();
--c.print("Meccs elkezdodott");

if (c.GetMyColor()) then
	c.print("Kekek vagyunk");
	Offset = 3000;
	Ori = -1;
end

p.GoTo(250, Offset + Ori * 800)

px, py, dist = c.FindPawn(FIG_PAWN)
tx, ty = c.GetStoragePos(STORAGE_RIGHT, px, py)
p.TurnTo(tx, ty)
p.GoTo(tx, ty)

c.music("starwars")
armtest(false, 1000, 100)

p.Turn(math.pi)
p.Go(30)

armtest(true, 1000, 100)


--[[
p.runparallel(
function ()
	armtest(true)
end,
function ()
	armtest(false)
end)
]]