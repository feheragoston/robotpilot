
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
	end
end

function GoSafe(...)
	if (control.GoSafe(...)) then
		MotionInProgress()
	else
		control.print("(pilot) GoSafe hiba")
	end
end

function GoTo(...)
	if (control.GoTo(...)) then
		MotionInProgress()
	else
		control.print("(pilot) GoTo hiba")
	end
end

function GoToSafe(...)
	if (control.GoToSafe(...)) then
		MotionInProgress()
	else
		control.print("(pilot) GoToSafe hiba")
	end
end

function Turn(...)
	if (control.Turn(...)) then
		MotionInProgress()
	else
		control.print("(pilot) Turn hiba")
	end
end

function TurnSafe(...)
	if (control.TurnSafe(...)) then
		MotionInProgress()
	else
		control.print("(pilot) TurnSafe hiba")
	end
end

function TurnTo(...)
	if (control.TurnTo(...)) then
		MotionInProgress()
	else
		control.print("(pilot) TurnTo hiba")
	end
end

function TurnToSafe(...)
	if (control.TurnToSafe(...)) then
		MotionInProgress()
	else
		control.print("(pilot) TurnToSafe hiba")
	end
end

function MotionStop(...)
	if (control.MotionStop(...)) then
		while (control.MotionStopInProgress()) do
			control.process()
		end
	else
		control.print("(pilot) MotionStop hiba")
	end
end

function GripperMove(...)
	if (control.GripperMove(...)) then
		while (control.GripperMoveInProgress()) do
			control.process()
		end
	end
end

function ConsoleMove(...)
	if (control.ConsoleMove(...)) then
		while (control.ConsoleMoveInProgress()) do
			control.process()
		end
	end
end

function ConsoleStop(...)
	if (control.ConsoleStop(...)) then
		while (control.ConsoleStopInProgress()) do
			control.process()
		end
	end
end

function ArmMove(left, ...)
	if (control.ArmMove(left, ...)) then
		while (control.ArmMoveInProgress(left)) do
			control.process()
		end
	end
end

function Magnet(left, ...)
	if (control.Magnet(left, ...)) then
		while (control.MagnetInProgress(left)) do
			control.process()
		end
	end
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
