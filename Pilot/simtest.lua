
c.CalibrateDeadreckoning()
x, y, phi = c.GetRobotPos()
assert(x == 200, "Calibrate X changed")
assert(y == 200, "Calibrate Y changed")
assert(math.abs(phi - 1.5708) < 0.001, "Calibrate Phi changed")

c.print("Deadreckoning beallitva")

function test1()
	p.GoToSafe(350, 2200)
	p.GoToSafe(1550, 2200)
	p.GoToSafe(1550, 800)
	p.GoToSafe(350, 800)
end

function GoToTest(points)
	for i, point in ipairs(points) do
		p.TurnToSafe(point.x, point.y)
		p.GoToSafe(point.x, point.y)
	end
end

function GoFromTest(from, points)
	c.SetRobotPos(from.x, from.y)
	p.TurnTo(points[1].x, points[1].y)
	for i, point in ipairs(points) do
		p.TurnToSafe(point.x, point.y)
		p.GoToSafe(point.x, point.y)
	end
end

startTime = c.gettimeofday()
c.simulate(p.GoToSafe, 350, 2200)
c.print("GoTo:", c.getelapsedtime(startTime) / 1000000)

c.print("Test1 without obstacles:", c.simulate(test1))
c.AddTestObstacles()
c.print("Test1 with obstacles:", c.simulate(test1))
c.ClearDynObstacles()
c.print("test1:", c.getelapsedtime(startTime) / 1000000)

points = {}
for i=0,120 do
	points[i+1] = { x = math.floor(i / 5) * 175 + 175, y = i % 5 * 175 + 625 }
end

c.AddTestObstacles()

for i,line in ipairs(points) do
	print(i, line.x, line.y, c.simulate(GoToTest, {line}))
end
c.print("step1:", c.getelapsedtime(startTime) / 1000000)

for i,c1 in ipairs(points) do
	for j,c2 in ipairs(points) do
		if (j ~= i) then
			if (c.simulate(GoToTest, {c1, c2})) then
				print(i, j)
			end
		end
	end
end
c.print("step2:", c.getelapsedtime(startTime) / 1000000)

--[[
for i,c1 in ipairs(points) do
	for j,c2 in ipairs(points) do
		for k,c3 in ipairs(points) do
			if (i ~= j and j ~= k) then
				if (c.simulate(GoToTest, {c1, c2, c3})) then
					print(i, j, k)
				end
			end
		end
	end
end
c.print("step3:", c.getelapsedtime(startTime) / 1000000)
]]

for i,c1 in ipairs(points) do
	for j,c2 in ipairs(points) do
		if (i < j) then
			if (c.simulate(GoFromTest, c1, {c2})) then
				print(i, "=>", j)
			end
		end
	end
end
c.print("step4:", c.getelapsedtime(startTime) / 1000000)

c.ClearDynObstacles()

c.exit()