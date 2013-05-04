
dofile("Pilot/2013/defines.lua")

dofile("Pilot/2013/functions.lua")

dofile("Pilot/2013/calibration.lua")

pickup1_done = false
pickup2_done = false
eating_done = false
fire_done = false


p.Go(50)
p.TurnTo(250, 274 * Ori + Offset)
p.sleep(500)
p.GoTo(250, 274 * Ori + Offset)
p.sleep(500)
x, y, phi = c.GetRobotPos()
p.sleep(1000)
p.TurnTo(500, 1000 * Ori + Offset)

c.music("start")
repeat
	p.process()
	x, y, phi = c.GetRobotPos()
until(c.GetStartButton())

c.StartMatch(true); -- !!!!!!!!!!!!!!!!!!
c.print("Meccs elkezdodott");

-- elsot felszedi
pickup1_done = try(0, function()
	
	MoveToSafe(435,200)	--felso talca
	
end);

-- masodikat felszedi
pickup2_done = try(0,function()
end);

