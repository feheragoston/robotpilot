dofile("Pilot/calibration.lua")

dofile("Pilot/functions.lua")

p.Go(30)

p.MoveToSafe(250, 500)

p.TurnToSafe(250, 1500)

p.runparallel(
	function()
		p.GripperMove(Right, 90)
		p.GripperMove(Left, 90)
	end,
	function()
		p.ClawMove(Right, 90)
		p.ClawMove(Left, 90)
	end
)
p.ArmMove(81) -- felenk nez: 89

p.runparallel(
	function()
		p.GoToSafe(250, 2500)
	end,
	function()
		p.Compressor(true)
		repeat
			x, y, phi = c.GetRobotPos()
			p.process()
		until (y > 690) -- felenk nez: 720
		p.ArmMove(100, 1000, 3000)
		p.sleep(200)
		p.ArmMove(10, 1000, 200)
		p.Valve(true)
		p.sleep(100)
		p.Valve(false)
		p.ArmMove(0)
		p.Compressor(false)
	end
)
