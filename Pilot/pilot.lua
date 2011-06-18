
local control = control

module(...)

function sleep(milliseconds)
	local callTime = control.gettimeofday();
	while (control.getelapsedtime(callTime) < milliseconds * 1000) do
		control.process()
	end
end

local function MotionInProgress()
	while (control.MotionInProgress()) do
		control.process()
	end
end

function Go(...)
	if (control.Go(...)) then
		MotionInProgress()
	else
		control.print("(pilot) Go hiba")
		return false
	end
	return true
end

function GoSafe(...)
	if (control.GoSafe(...)) then
		MotionInProgress()
	else
		control.print("(pilot) GoSafe hiba")
		return false
	end
	return true
end

function GoTo(...)
	if (control.GoTo(...)) then
		MotionInProgress()
	else
		control.print("(pilot) GoTo hiba")
		return false
	end
	return true
end

function GoToSafe(...)
	if (control.GoToSafe(...)) then
		MotionInProgress()
	else
		control.print("(pilot) GoToSafe hiba")
		return false
	end
	return true
end

function Turn(...)
	if (control.Turn(...)) then
		MotionInProgress()
	else
		control.print("(pilot) Turn hiba")
		return false
	end
	return true
end

function TurnSafe(...)
	if (control.TurnSafe(...)) then
		MotionInProgress()
	else
		control.print("(pilot) TurnSafe hiba")
		return false
	end
	return true
end

function TurnTo(...)
	if (control.TurnTo(...)) then
		MotionInProgress()
	else
		control.print("(pilot) TurnTo hiba")
		return false
	end
	return true
end

function TurnToSafe(...)
	if (control.TurnToSafe(...)) then
		MotionInProgress()
	else
		control.print("(pilot) TurnToSafe hiba")
		return false
	end
	return true
end

function MotionStop(...)
	if (control.MotionStop(...)) then
		while (control.MotionStopInProgress()) do
			control.process()
		end
	else
		control.print("(pilot) MotionStop hiba")
		return false
	end
	return true
end

function GripperMove(...)
	if (control.GripperMove(...)) then
		while (control.GripperMoveInProgress()) do
			control.process()
		end
	else
		control.print("(pilot) GripperMove hiba")
		return false
	end
	return true
end

function ConsoleMove(...)
	if (control.ConsoleMove(...)) then
		while (control.ConsoleMoveInProgress()) do
			control.process()
		end
	else
		control.print("(pilot) ConsoleMove hiba")
		return false
	end
	return true
end

function ConsoleStop(...)
	if (control.ConsoleStop(...)) then
		while (control.ConsoleStopInProgress()) do
			control.process()
		end
	else
		control.print("(pilot) ConsoleStop hiba")
		return false
	end
	return true
end

function ArmMove(left, ...)
	if (control.ArmMove(left, ...)) then
		while (control.ArmMoveInProgress(left)) do
			control.process()
		end
	else
		control.print("(pilot) ArmMove hiba")
		return false
	end
	return true
end

function Magnet(left, ...)
	if (control.Magnet(left, ...)) then
		while (control.MagnetInProgress(left)) do
			control.process()
		end
	else
		control.print("(pilot) Magnet hiba")
		return false
	end
	return true
end

function RefreshPawnPositions(...)
	if (control.RefreshPawnPositions(...)) then
		while (control.RefreshPawnPositionsInProgress()) do
			control.process()
		end
		return control.RefreshPawnPositionsFinished()
	else
		control.print("(pilot) RefreshPawnPositions hiba")
	end
	return false
end
