
dofile("Pilot/calibration.lua")
dofile("Pilot/functions.lua")

	goSpeed = 400
	goAcc = 500
	turnSpeed = 2
	turnAcc = 2

c.runparallel(
function()
	repeat
		if (not p.RefreshPawnPositions()) then
			p.sleep(500)
		end
		c.process()
	until (c.GetStartButton())
end,
function()
	p.sleep(3000)
	repeat
		c.process()
	until(c.GetStartButton())
	
	c.StartMatch(false); -- !!!!!!!!!!!!!!!!!!
	c.print("Meccs elkezdodott");
	
	p.TurnTo(200, Offset + Ori * 800, turnSpeed, 4)
	p.GoTo(200, Offset + Ori * 800, goSpeed, goAcc)
end)

pawnInGripper = false
pawnInLeft = false
pawnInRight = false

safe1Deployed = false;
safe2Deployed = false;
bonus1Deployed = false;

repeat
	c.print("Felszedo fazis indul")
	
	ignoreRadius = ROBOT_RADIUS;
	
	repeat
		local status, err = pcall(function()
			leftIR = MAX_DISTANCE
			rightIR = MAX_DISTANCE
			gripperGIR = MAX_DISTANCE -- green area
			gripperBIR = MAX_DISTANCE -- board
			
			if (not pawnInLeft) then
				leftIR, leftPX, leftPY, leftX, leftY = SearchWithArmFromBoard(true, ignoreRadius)
			end
			if (not pawnInRight) then
				rightIR, rightPX, rightPY, rightX, rightY = SearchWithArmFromBoard(false, ignoreRadius)
			end
			if (not pawnInGripper) then
				ir = math.max(ignoreRadius, ROBOT_FRONT_MAX + PAWN_RADIUS)
				if (safe1Deployed and bonus1Deployed) then
					gripperGIR, gripperGPX, gripperGPY, gripperGX, gripperGY = SearchWithGripperFromSides(ir, FIG_KING)
				else
					gripperBIR, gripperBPX, gripperBPY, gripperBX, gripperBY = SearchWithGripperFromBoard(ir, FIG_PAWN)
					gripperGIR, gripperGPX, gripperGPY, gripperGX, gripperGY = SearchWithGripperFromSides(ir, FIG_PAWN)
				end
			end
			
			ignoreRadius = math.min(leftIR, rightIR, gripperGIR, gripperBIR)
			if (ignoreRadius < MAX_DISTANCE) then
				if (ignoreRadius == leftIR) then
					c.SetPawnType(leftPX, leftPY, FIG_WENT_OVER)
					PickupWithArmFromBoard(true, leftX, leftY)
					pawnInLeft = true
					c.SetPawnType(leftPX, leftPY, FIG_PICKED_UP)
				elseif (ignoreRadius == rightIR) then
					c.SetPawnType(rightPX, rightPY, FIG_WENT_OVER)
					PickupWithArmFromBoard(false, rightX, rightY)
					pawnInRight = true
					c.SetPawnType(rightPX, rightPY, FIG_PICKED_UP)
				elseif (ignoreRadius == gripperBIR) then
					c.SetPawnType(gripperBPX, gripperBPY, FIG_WENT_OVER)
					if (PickupWithGripperFromBoard(gripperBX, gripperBY)) then
						pawnInGripper = true;
						c.SetPawnType(gripperBPX, gripperBPY, FIG_PICKED_UP)
					end
				elseif (ignoreRadius == gripperGIR) then
					c.SetPawnType(gripperGPX, gripperGPY, FIG_WENT_OVER)
					if (PickupWithGripperFromSides(gripperGPX, gripperGPY, gripperGX, gripperGY)) then
						pawnInGripper = true;
						c.SetPawnType(gripperGPX, gripperGPY, FIG_PICKED_UP)
					end
					p.GoSafe(-250, goSpeed, goAcc)
				end
				ignoreRadius = ROBOT_RADIUS;
			end
		end);
		if (not status) then
			c.print("Hiba", err);
			p.MotionStop(MAX_DEC)
			ignoreRadius = ROBOT_RADIUS;
		end
		c.process()
	until (ignoreRadius == MAX_DISTANCE)
	
	c.print("Felszedo fazis kesz: ", pawnInLeft, pawnInRight, pawnInGripper)
	
	ignorePriority = -1000;
	while (pawnInGripper) do
		local status, err = pcall(function()
			c.print("Paraszt uritese");
			x1, y1, x2, y2, target, ignorePriority = c.GetDeployPoint(1, ignorePriority);
			if (x1) then
				if (target > 29) then
					if (c.simulate(DeployFromGripper, x1, y1, x2, y2)) then
						DeployFromGripper(x1, y1, x2, y2);
						c.SetDeployPointPriority(target, 1); -- jeloljuk, hogy a mezo foglalt
						pawnInGripper = false
						if (target == 30 or target == 35) then
							safe1Deployed = true
						elseif (target == 32 or target == 33) then
							bonus1Deployed = true
						elseif (target == 31 or target == 34) then
							safe2Deployed = true
						end
						p.GoSafe(-250)
					else
						--
					end
				else
					if (pawnInLeft) then
						x1, y1, x2, y2, dist = c.FindPawn(2, 1, x2, y2)
						if (x1) then
							if (pawnInRight) then
								if (c.simulate(DeployFullTower, x2, y2)) then
									DeployFullTower(x2, y2)
									c.SetDeployPointPriority(target, 1);
									pawnInLeft = false;
									pawnInRight = false;
									pawnInGripper = false;
									p.GoSafe(-200, goSpeed, goAcc)
									p.GripperMove(0)
									p.ConsoleMove(0)
								end
							else
								--TODO
							end
						end
					else
						x1, y1, x2, y2, dist = c.FindPawn(3, 1, x2, y2)
						if (x1) then
							--TODO
						end
					end
				end
			else
				c.print("Nincs tobb lerako pozicio")
				c.process()
				--TODO
			end
		end);
		if (not status) then
			c.print("Hiba", err);
			p.MotionStop(MAX_DEC)
			ignorePriority = -1000;
		end
		c.process()
	end
until (c.GetStopButton())
