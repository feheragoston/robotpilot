
goSpeed = 500
goAcc = 800
turnSpeed = 3
turnAcc = 6

c.music("start")

repeat c.process() until (c.GetStartButton());
c.print("Startgomb lenyomva, kalibrálunk");

p.sleep(1000)
c.SetMotorSupply(true)
c.print("Motortap bekapcsolva");

p.sleep(2000)
p.Magnet(true, 0)
p.Magnet(false, 0)
p.ArmMove(true, 0)
c.print("Balkar behajtva");
p.ArmMove(false, 0)
c.print("Jobbkar behajtva");

p.GripperMove(0)
c.print("Gripper behajtva");

c.CalibrateConsole();

c.music("calibrate")


repeat

repeat c.process() until (c.GetStartButton());
c.print("Startgomb lenyomva, indulunk");

		p.Magnet(true, 1)
		p.ArmMove(true, 130)
		p.sleep(10)
		p.ArmMove(true, 0)
		p.Turn(3.1416,turnSpeed,turnAcc)
		p.ArmMove(true, 130)
		p.Magnet(true,-1)
		p.sleep(10)
		p.ArmMove(true, 0)
		p.Magnet(true,0)
		p.GoTo(-100,0,goSpeed,goAcc)
		p.Turn(-3.1416,turnSpeed,turnAcc)

		p.GoTo(40,0,goSpeed,goAcc)
		p.TurnTo(1000,0,turnSpeed,turnAcc)
		p.Magnet(false, 1)
		p.ArmMove(false, 130)
		p.sleep(10)
		p.ArmMove(false, 0)
		p.Turn(-3.1416,turnSpeed,turnAcc)
		p.GoTo(-40,0,goSpeed,goAcc)
		p.ArmMove(false, 130)
		p.Magnet(false,-1)
		p.sleep(10)
		p.ArmMove(false, 0)
		p.Magnet(false,0)
		p.GoTo(-100,0,goSpeed,goAcc)
		p.Turn(3.1416,turnSpeed,turnAcc)
		p.TurnTo(1000,0,turnSpeed,turnAcc)
		p.GoTo(0,0,goSpeed,goAcc)

	c.print("Varunk...")
	p.sleep(2000)
	
until (c.GetStopButton())
