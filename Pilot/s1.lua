
dofile("Pilot/calibration.lua")

dofile("Pilot/functions.lua")

p.Go(30)
p.TurnToSafe(265, 305 * Ori + Offset)
p.GoToSafe(265, 305 * Ori + Offset)
p.TurnToSafe(685, 2510 * Ori + Offset)

c.music("start")
repeat
	p.process()
until(c.GetStartButton())

--c.StartMatch(true); -- !!!!!!!!!!!!!!!!!!
c.print("Meccs elkezdodott");

p.GoToSafe(685, 2510 * Ori + Offset)
p.TurnToSafe(685, 1500)

p.runparallel(
	function()
		p.GripperMove(Right, 90)
		p.GripperMove(Left, 130)
	end,
	function()
		p.ClawMove(Right, 90)
		p.ClawMove(Left, 130)
	end
)


-- felszedes
p.GoToSafe(685, 2275 * Ori + Offset) -- Palyahoz igazitani
p.GripperMove(Left, 80, 500, 400)
PickupWithArm()

p.GripperMove(Left, 105)

p.MoveToSafe(685, 2150 * Ori + Offset)
PickupWithArm()

p.MoveToSafe(685, 1700 * Ori + Offset)

p.GripperMove(Left, 130)
p.GoToSafe(685, 1480 * Ori + Offset) -- Palyahoz igazitani
p.GripperMove(Left, 105)
p.GoToSafe(685, 900 * Ori + Offset)

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

p.runparallel(
	function()
		p.GoSafe(-300)
	end,
	function()
		p.ConsoleMove(20, 400, 15)
		p.CalibrateConsole()
	end
)
