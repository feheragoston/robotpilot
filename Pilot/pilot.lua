
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
	end
end

function GoSafe(...)
	if (control.GoSafe(...)) then
		MotionInProgress()
	end
end

function GoTo(...)
	if (control.GoTo(...)) then
		MotionInProgress()
	end
end

function GoToSafe(...)
	if (control.GoToSafe(...)) then
		MotionInProgress()
	end
end

function Turn(...)
	if (control.Turn(...)) then
		MotionInProgress()
	end
end

function TurnSafe(...)
	if (control.TurnSafe(...)) then
		MotionInProgress()
	end
end

function TurnTo(...)
	if (control.TurnTo(...)) then
		MotionInProgress()
	end
end

function TurnToSafe(...)
	if (control.TurnToSafe(...)) then
		MotionInProgress()
	end
end

function MotionStop(...)
	if (control.MotionStop(...)) then
		while (control.MotionStopInProgress()) do
			control.process()
		end
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

function ArmMove(...)
	if (control.ArmMove(...)) then
		while (control.ArmMoveInProgress()) do
			control.process()
		end
	end
end

function Magnet(...)
	if (control.Magnet(...)) then
		while (control.MagnetInProgress()) do
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