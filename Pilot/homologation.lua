
dofile("Pilot/calibration.lua")

p.Go(50, 100, 100)
p.TurnTo(300, Offset + Ori * 790)

repeat p.process() until (c.GetStartButton());
c.StartMatch();
c.print("Meccs elkezdodott");

p.GoToSafe(300, Offset + Ori * 790)


p.TurnToSafe(860, 720 * Ori + Offset)
if (c.simulate(p.GoToSafe, 860, 720 * Ori + Offset)) then
	p.GoToSafe(860, 720 * Ori + Offset)
end
if (c.simulate(p.TurnToSafe, 860, 500 * Ori + Offset)) then
	p.TurnToSafe(860, 500 * Ori + Offset)
end
if (c.simulate(p.GoToSafe, 860, 500 * Ori + Offset)) then
	p.GoToSafe(860, 500 * Ori + Offset)
end
if (c.simulate(p.Go, -220)) then
	p.Go(-220)
end






if (c.simulate(p.TurnToSafe, 300, Offset + Ori * 790)) then
	p.TurnToSafe(300, Offset + Ori * 790)
	if (c.simulate(p.GoToSafe, 300, Offset + Ori * 790)) then
		p.GoToSafe(300, Offset + Ori * 790)
		if (c.simulate(p.TurnToSafe, 1400, Offset + Ori * 790)) then
			p.TurnToSafe(1400, Offset + Ori * 790)
			p.GripperMove(false, 95)
			p.GripperMove(true, 95)
			p.ClawMove(false, 120)
			p.ClawMove(true, 120)
			if (c.simulate(p.GoToSafe, 1400, Offset + Ori * 790)) then
				p.GoToSafe(1400, Offset + Ori * 790)
				p.GripperMove(true, 55)
				p.GripperMove(false, 55)
				if (c.simulate(p.TurnToSafe, 700, Offset + Ori * 700)) then
					p.TurnToSafe(700, Offset + Ori * 700)
				end
				if (c.simulate(p.GoToSafe, 700, Offset + Ori * 700)) then
					p.GoToSafe(700, Offset + Ori * 700)
				end
				if (c.simulate(p.TurnToSafe, 700, Offset + Ori * 400)) then
					p.TurnToSafe(700, Offset + Ori * 400)
				end
				if (c.simulate(p.GoToSafe, 700, Offset + Ori * 400)) then
					p.GoToSafe(700, Offset + Ori * 400)
				end
				p.GripperMove(true, 95)
				p.GripperMove(false, 95)
				if (c.simulate(p.GoSafe, -300)) then
					p.GoSafe(-300)
				end
				p.GripperMove(true, 0)
				p.GripperMove(false, 1)
				p.ClawMove(true, 0)
				p.ClawMove(false, 2)
			end
		end
	end
end
