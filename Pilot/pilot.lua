
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

local function Motion(motion, ...)
	if (not motion(...)) then
		control.music("error")
		pilot.sleep(100)
		if (not motion(...)) then
			control.music("error")
			pilot.sleep(1000)
			if (not motion(...)) then
				control.music("error")
				control.print("(pilot) Motion hiba")
				return false
			end
		end
	end
	MotionInProgress()
	return not control.GetMotionError()
end

function Go(...)
	return Motion(control.Go, ...)
end

function GoSafe(...)
	return Motion(control.GoSafe, ...)
end

function GoTo(...)
	return Motion(control.GoTo, ...)
end

function GoToSafe(...)
	return Motion(control.GoToSafe, ...)
end

function Turn(...)
	return Motion(control.Turn, ...)
end

function TurnSafe(...)
	return Motion(control.TurnSafe, ...)
end

function TurnTo(...)
	return Motion(control.TurnTo, ...)
end

function TurnToSafe(...)
	return Motion(control.TurnToSafe, ...)
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
	end
	return false
end
