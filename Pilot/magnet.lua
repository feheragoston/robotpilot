
dofile("Pilot/calibration.lua")
dofile("Pilot/functions.lua")

	goSpeed = 400
	goAcc = 500
	turnSpeed = 4
	turnAcc = 8

p.TurnTo(200, 400)
p.GoTo(200, 400)
p.TurnTo(350, 800)
p.GoTo(350, 800)
p.TurnTo(350, 2200)

p.ArmMove(false, 110)

p.runparallel(function()
	p.GoTo(350, 2200)
end,
function()
	while (c.MotionInProgress()) do
		local x, y, phi = c.GetRobotPos()
		local z = y - 400
		z = z % 700 
		c.print(z)
		if (z < 175 or z > 350) then
			p.Magnet(false, 1)
		else
			p.Magnet(false, -1)
		end
		p.process()
	end
end)
