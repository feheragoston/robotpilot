
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
p.GoToSafe(1400, Offset + Ori * 800)
p.GripperMove(true, 55)
p.GripperMove(false, 55)
p.TurnToSafe(700, Offset + Ori * 700)
p.GoToSafe(700, Offset + Ori * 700)
p.TurnToSafe(700, Offset + Ori * 400)
p.GoToSafe(700, Offset + Ori * 400)
p.GripperMove(true, 95)
p.GripperMove(false, 95)
p.GoSafe(-300)
p.GripperMove(true, 0)
p.GripperMove(false, 1)
p.ClawMove(true, 0)
p.ClawMove(false, 2)
