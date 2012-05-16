
dofile("Pilot/calibration.lua")

dofile("Pilot/functions.lua")

p.Go(30)
x, y, phi = c.GetRobotPos()
p.TurnTo(x, 1500)
p.Go(140)
p.TurnToSafe(685, 2510 * Ori + Offset)
p.Go(-15)

c.music("start")
repeat
	p.process()
until(c.GetStartButton())

--c.StartMatch(true); -- !!!!!!!!!!!!!!!!!!
c.print("Meccs elkezdodott");

local status, err = pcall(function()
	p.GoToSafe(685, 2510 * Ori + Offset, 1000, 3000)
end);
if (not status) then
	c.print("Hiba", err);
	p.MotionStop(MAX_DEC)
end

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
p.GoToSafe(685, 2280 * Ori + Offset) -- Palyahoz igazitani
p.GripperMove(Left, 80, 500, 400)
if (c.GetMyColor() == PURPLE) then
	p.GripperMove(Left, 60, 500, 400)
end
p.GripperMove(Left, 100)
Felszedes()


p.MoveToSafe(685, 2150 * Ori + Offset)
Felszedes()

p.MoveToSafe(685, 1700 * Ori + Offset)

p.GripperMove(Left, 130)
p.GoToSafe(685, 1485 * Ori + Offset) -- Palyahoz igazitani
p.GripperMove(Left, 100)
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
