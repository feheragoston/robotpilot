
goSpeed = 800
goAcc = 800
turnSpeed = 6
turnAcc = 10

c.music("start")

repeat p.process() until (c.GetStartButton());
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

repeat p.process() until (c.GetStartButton());
c.print("Startgomb lenyomva, indulunk");

repeat
	p.TurnTo(1000, 0, turnSpeed, turnAcc)
	p.GripperMove(90)
	
	pawnInGripper = false
	moveFinished = false
	p.runparallel(
		function()
			p.Go(800)
			moveFinished = true
		end,
		function()
			repeat
				p.process()
			until (c.PawnInGripper() or moveFinished)
			pawnInGripper = c.PawnInGripper()
			if (pawnInGripper) then
				c.print("Parasztot talaltam!")
			end
			if (c.MotionInProgress()) then
				p.MotionStop(500)
			end
		end
	)
	
	
	c.print("Varunk...")
	p.sleep(100)
	
	if (pawnInGripper) then
		x, y, phi = c.GetRobotPos()
		c.print(x, y, phi)
		p.Go(-200, goSpeed, goAcc)
		p.GripperMove(0)
		tx, ty = c.GetStoragePos(STORAGE_LEFT, x + ROBOT_FRONT_PAWN * math.cos(phi), y + ROBOT_FRONT_PAWN * math.sin(phi))
		c.print(tx, ty)
		p.TurnTo(tx, ty, turnSpeed, turnAcc)
		p.GoTo(tx, ty, goSpeed, goAcc)
		p.Magnet(true, 1)
		p.ArmMove(true, 130)
		p.sleep(10)
		p.ArmMove(true, 0)
	
		p.TurnTo(200, 0, turnSpeed, turnAcc)
		p.GoTo(200, 0, goSpeed, goAcc)
		p.TurnTo(-1000, 0, turnSpeed, turnAcc)
		
		p.ArmMove(true, 130)
		p.Magnet(true, -1)
		p.ArmMove(true, 0)
		p.Magnet(true, 0)
		
		p.Turn(-math.pi)
	else
		x, y, phi = c.GetRobotPos()
		p.TurnTo(x, y - 265, turnSpeed, turnAcc)
		p.GoTo(x, y - 265, goSpeed, goAcc)
		p.TurnTo(x - 1000, y - 265, turnSpeed, turnAcc)
		
		pawnInGripper = false
		moveFinished = false
		p.runparallel(
			function()
				p.Go(800)
				moveFinished = true
			end,
			function()
				repeat
					p.process()
				until (c.PawnInGripper() or moveFinished)
				pawnInGripper = c.PawnInGripper()
				if (pawnInGripper) then
					c.print("Parasztot talaltam!")
				end
				if (c.MotionInProgress()) then
					p.MotionStop(500)
				end
			end
		)
		
		
		c.print("Varunk...")
		p.sleep(100)
	
		if (pawnInGripper) then
			x, y, phi = c.GetRobotPos()
			c.print(x, y, phi)
			p.Go(-200, goSpeed, goAcc)
			p.GripperMove(0)
			tx, ty = c.GetStoragePos(STORAGE_LEFT, x + ROBOT_FRONT_PAWN * math.cos(phi), y + ROBOT_FRONT_PAWN * math.sin(phi))
			c.print(tx, ty)
			p.TurnTo(tx, ty, turnSpeed, turnAcc)
			p.GoTo(tx, ty, goSpeed, goAcc)
			p.Magnet(true, 1)
			p.ArmMove(true, 130)
			p.sleep(10)
			p.ArmMove(true, 0)
		
			p.TurnTo(600, -265, turnSpeed, turnAcc)
			p.GoTo(600, -265, goSpeed, goAcc)
			p.TurnTo(1000, -265, turnSpeed, turnAcc)
			
			p.ArmMove(true, 130)
			p.Magnet(true, -1)
			p.ArmMove(true, 0)
			p.Magnet(true, 0)
			
			p.Turn(-math.pi)
			p.GoTo(0, -265, goSpeed, goAcc)
			p.TurnTo(0, 0, turnSpeed, turnAcc)
			p.GoTo(0, 0, goSpeed, goAcc)
		else
			p.TurnTo(0, 0, turnSpeed, turnAcc)
			p.GoTo(0, 0, goSpeed, goAcc)
		end
	end
until (c.GetStopButton())
