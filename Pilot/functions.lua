
function PickupWithGripperFromSides(ignoreRadius)
	local px, py, x, y
	px, py, x, y, ignoreRadius = c.FindPawn(4, ignoreRadius);
	if (x) then
		p.GripperMove(0)
		p.TurnToSafe(x, y, turnSpeed, turnAcc)
		p.GoToSafe(x, y, goSpeed, goAcc)
		p.TurnToSafe(px, py, turnSpeed, turnAcc)
		p.GripperMove(90)
		p.GoToSafe(px, py, goSpeed, goAcc)
		if (c.PawnInGripper()) then
			p.GripperMove(GripperGrab)
			pawnInGripper = true;
			p.GoSafe(-250, goSpeed, goAcc)
		end
	else
		return false
	end
	return ignoreRadius
end

function PickupWithArmFromBoard(left, ignoreRadius)
	local arm = 3
	local px, py, x, y
	if (left) then
		arm = 2
	end
	px, py, x, y, ignoreRadius = c.FindPawn(arm, ignoreRadius)
	if (x) then
		p.TurnToSafe(x, y, turnSpeed, turnAcc)
		p.GoToSafe(x, y, goSpeed, goAcc)
		p.Magnet(left, 1)
		p.ArmMove(left, 130)
		p.sleep(10)
		p.ArmMove(left, 0)
	else
		return false
	end
	return ignoreRadius
end
