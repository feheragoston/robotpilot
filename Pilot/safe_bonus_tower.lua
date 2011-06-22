
dofile("Pilot/calibration.lua")
dofile("Pilot/functions.lua")
--[[
	goSpeed = 400
	goAcc = 500
	turnSpeed = 2
	turnAcc = 2
]]
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
				c.print("Left:", leftIR, leftPX, leftPY, leftX, leftY)
			end
			if (not pawnInRight) then
				rightIR, rightPX, rightPY, rightX, rightY = SearchWithArmFromBoard(false, ignoreRadius)
				c.print("Right:", rightIR, rightPX, rightPY, rightX, rightY)
			end
			if (not pawnInGripper) then
				ir = math.max(ignoreRadius, ROBOT_FRONT_MAX + PAWN_RADIUS)
				if (safe1Deployed and bonus1Deployed) then
					gripperPawnType = FIG_KING
					gripperGIR, gripperGPX, gripperGPY, gripperGX, gripperGY = SearchWithGripperFromSides(ir, gripperPawnType)
				else
					gripperPawnType = FIG_PAWN
					gripperBIR, gripperBPX, gripperBPY, gripperBX, gripperBY = SearchWithGripperFromBoard(ir, gripperPawnType)
					c.print("GripperB:", gripperBIR, gripperBPX, gripperBPY, gripperBX, gripperBY)
					gripperGIR, gripperGPX, gripperGPY, gripperGX, gripperGY = SearchWithGripperFromSides(ir, gripperPawnType)
				end
				c.print("GripperG:", gripperGIR, gripperGPX, gripperGPY, gripperGX, gripperGY)
			end
			
			ignoreRadius = math.min(leftIR, rightIR, gripperGIR, gripperBIR)
			c.print("ignoreRadius:", ignoreRadius)
			if (ignoreRadius < MAX_DISTANCE) then
				if (math.abs(ignoreRadius - leftIR) < 10) then
					c.print("Felszedes bal karral")
					c.SetPawnType(leftPX, leftPY, FIG_WENT_OVER)
					PickupWithArmFromBoard(true, leftX, leftY)
					pawnInLeft = true
					c.SetPawnType(leftPX, leftPY, FIG_PICKED_UP)
				elseif (math.abs(ignoreRadius - rightIR) < 10) then
					c.print("Felszedes jobb karral")
					c.SetPawnType(rightPX, rightPY, FIG_WENT_OVER)
					PickupWithArmFromBoard(false, rightX, rightY)
					pawnInRight = true
					c.SetPawnType(rightPX, rightPY, FIG_PICKED_UP)
				elseif (math.abs(ignoreRadius - gripperBIR) < 10) then
					c.print("Felszedes gripperrel tablarol")
					c.SetPawnType(gripperBPX, gripperBPY, FIG_WENT_OVER)
					if (PickupWithGripperFromBoard(gripperBX, gripperBY)) then
						pawnInGripper = gripperPawnType;
						c.SetPawnType(gripperBPX, gripperBPY, FIG_PICKED_UP)
					end
				elseif (math.abs(ignoreRadius - gripperGIR) < 10) then
					c.print("Felszedes gripperrel zold mezorol")
					c.SetPawnType(gripperGPX, gripperGPY, FIG_WENT_OVER)
					if (PickupWithGripperFromSides(gripperGPX, gripperGPY, gripperGX, gripperGY)) then
						pawnInGripper = gripperPawnType;
						c.SetPawnType(gripperGPX, gripperGPY, FIG_PICKED_UP)
					end
				end
				if (c.simulate(function() p.GoSafe(-250, goSpeed, goAcc); end)) then
					p.GoSafe(-250, goSpeed, goAcc)
				elseif (c.simulate(function() p.GoSafe(-200, goSpeed, goAcc); end)) then
					p.GoSafe(-200, goSpeed, goAcc)
				elseif (c.simulate(function() p.GoSafe(-100, goSpeed, goAcc); end)) then
					p.GoSafe(-100, goSpeed, goAcc)
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
			x1, y1, x2, y2, target, ignorePriority = c.GetDeployPoint(STORAGE_GRIPPER, ignorePriority);
			if (x1) then
				if (gripperPawnType == FIG_PAWN) then
					if (c.simulate(DeployFromGripper, x1, y1, x2, y2)) then
						DeployFromGripper(x1, y1, x2, y2);
						c.SetDeployPointPriority(target, 1, STORAGE_GRIPPER); -- jeloljuk, hogy a mezo foglalt
						pawnInGripper = false
						if (target == 30 or target == 35) then
							safe1Deployed = true
						elseif (target == 32 or target == 33) then
							bonus1Deployed = true
						elseif (target == 31 or target == 34) then
							safe2Deployed = true
						end
					else
						--
					end
				else
					if (pawnInLeft) then
						x1, y1, x2, y2, dist = c.FindPawn(STORAGE_LEFT, 1, x2, y2)
						if (x1) then
							if (pawnInRight) then
								if (c.simulate(DeployFullTower, x2, y2)) then
									DeployFullTower(x2, y2)
									c.SetDeployPointPriority(target, 1, STORAGE_GRIPPER); -- a tetejere gripperrel rakunk
									pawnInLeft = false;
									pawnInRight = false;
									pawnInGripper = false;
								end
							else
								if (c.simulate(DeployHalfTower, true, x2, y2)) then
									DeployHalfTower(true, x2, y2)
									c.SetDeployPointPriority(target, 1, STORAGE_GRIPPER); -- a tetejere gripperrel rakunk
									pawnInLeft = false;
									pawnInGripper = false;
								end
							end
						end
					else
						x1, y1, x2, y2, dist = c.FindPawn(STORAGE_RIGHT, 1, x2, y2)
						if (x1) then
							if (c.simulate(DeployHalfTower, false, x2, y2)) then
								DeployHalfTower(false, x2, y2)
								c.SetDeployPointPriority(target, 1, STORAGE_GRIPPER); -- a tetejere gripperrel rakunk
								pawnInRight = false;
								pawnInGripper = false;
							end
						end
					end
				end
				
				c.runparallel(
				function()
					if (c.simulate(function() p.GoSafe(-250, goSpeed, goAcc); end)) then
						p.GoSafe(-250, goSpeed, goAcc)
					elseif (c.simulate(function() p.GoSafe(-200, goSpeed, goAcc); end)) then
						p.GoSafe(-200, goSpeed, goAcc)
					elseif (c.simulate(function() p.GoSafe(-100, goSpeed, goAcc); end)) then
						p.GoSafe(-100, goSpeed, goAcc)
					end
					p.GripperMove(0)
				end,
				function()
					p.ConsoleMove(0)
				end)
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
