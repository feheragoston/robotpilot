
dofile("Pilot/calibration.lua")

dofile("Pilot/functions.lua")

p.Go(30)
x, y, phi = c.GetRobotPos()
p.TurnTo(x, 1500)
p.Go(140)
p.TurnToSafe(685, 2510 * Ori + Offset)
p.Go(-15)

cd1 = true
cd2 = true

c.music("start")
repeat
	p.process()
until(c.GetStartButton())

--c.StartMatch(true); -- !!!!!!!!!!!!!!!!!!
c.print("Meccs elkezdodott");

if (c.GoTo(685, 2510 * Ori + Offset, 1000, 3000)) then
	while (c.MotionInProgress() or c.MotionStopInProgress()) do
		if (c.MotionInProgress()) then
			for i = 1, OPPONENT_NUM do
				local x, y, validity = c.GetOpponentPos(i-1)
				if (validity > SONAR_TIMEOUT or y * Ori + Offset < 2500 or x > 500) then
					c.MotionStop(MAX_DEC)
				end
			end
		end
		local x, y, phi = c.GetRobotPos()
		if (cd1 and x > 600) then
			cd1 = false
		elseif (cd2 and x > 1600) then
			cd2 = false
		end
		process()
	end
end

if (cd1) then
end

if (cd2) then
end
