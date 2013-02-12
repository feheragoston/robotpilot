c.CalibrateDeadreckoning()
p.GripperMove(true, 90)
p.GripperMove(false, 45)
p.ClawMove(true, 90)
p.ClawMove(false, 35)
c.GoSafe(0)
c.simulate(c.MotionInProgress)
p.sleep(1000)

c.exit()

p.runparallel(
	function ()
		for i = 0, 90 do
			p.GripperMove(true, i)
			c.simulate(c.MotionInProgress)
			p.sleep(10)
		end
	end,
	function ()
		for i = 0, 130 do
			p.GripperMove(false, i)
			c.simulate(c.MotionInProgress)
			p.sleep(10)
		end
	end
)

if (p.MoveToSafe(1565, 175)) then
	c.print("Megcsináltuk a lehetetlent!")
else
	p.sleep(1000)
	if (p.MoveToSafe(210, 810)) then
		if (p.MoveToSafe(895, 680)) then
			p.GripperMove(false, 0)
			p.GripperMove(true, 1)
			if (p.MoveToSafe(860, 230)) then
				if (p.MoveToSafe(1200, 190)) then
					p.MoveToSafe(1655, 175)
				end
			end
		end
	end
	c.print("fail")
end

p.sleep(1000)

c.exit()