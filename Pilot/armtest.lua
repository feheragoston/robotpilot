
function armtest(left)
	p.ArmMove(left, 125)
	p.Magnet(left, 1)
	p.sleep(10)
	p.ArmMove(left, 0)
	
	p.sleep(100)
	p.ArmMove(left, 125)
	p.Magnet(left, 0)
	p.sleep(10)
	p.Magnet(left, -1)
	p.ArmMove(left, 0)
	p.Magnet(left, 0)
end

c.runparallel(
function ()
	armtest(true)
end,
function ()
	armtest(false)
end)
