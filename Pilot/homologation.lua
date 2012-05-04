
dofile("Pilot/calibration.lua")

p.Go(-100, 100, 100)
p.TurnTo(300, Offset + Ori * 800)

repeat p.process() until (c.GetStartButton());
c.StartMatch();
c.print("Meccs elkezdodott");

p.GoToSafe(300, Offset + Ori * 800)
p.TurnToSafe(1400, Offset + Ori * 800)
p.GripperMove(false, 95)
p.GripperMove(true, 95)
p.ClawMove(false, 90)
p.ClawMove(true, 120)
if (c.simulate(p.GoToSafe, 1400, Offset + Ori * 800)) then
	p.GoToSafe(1400, Offset + Ori * 800)
end
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
