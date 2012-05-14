
dofile("Pilot/calibration.lua")
dofile("Pilot/functions.lua")

c.StartMatch(false)

p.TurnTo(200, Offset + Ori * 800, turnSpeed, 4)
p.GoTo(200, Offset + Ori * 800, goSpeed, goAcc)
p.TurnTo(1050, 1500, turnSpeed, turnAcc)

p.RefreshPawnPositions()
px, py, ignoreRadius = c.FindPawn(0);
if (c.simulate(PickupWithGripperFromBoard, x, y)) then
	PickupWithGripperFromBoard(x, y)
end
