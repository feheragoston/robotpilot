-- CALIBRATE
p.sleep(1000)
c.SetMotorSupply(true)
c.print("Motortap bekapcsolva");

p.sleep(2000)
repeat c.process() until (c.GetStartButton());
c.print("Startgomb lenyomva felvesszuk a kezdopoziciot");

p.ArmMove(true, 0)
c.print("Balkar behajtva");
p.ArmMove(false, 0)
c.print("Jobbkar behajtva");

c.CalibrateConsole();

p.GripperMove(0)
c.print("Gripper behajtva");

c.music("calibrate")

c.CalibrateDeadreckoning()
c.print("Deadreckoning beallitva");
p.sleep(1000)

p.Go(30)
x, y, phi = c.GetRobotPos();
p.TurnTo(x, 1500)

p.Go(100)

p.sleep(1000)
c.print("Calibrate finished");
-- CALIBRATE END