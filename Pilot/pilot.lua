
local control = control
local coroutine = coroutine

module(...)

function sleep(milliseconds)
	if (control.in_simulate()) then
		return
	end

	local callTime = control.gettimeofday();
	while (control.getelapsedtime(callTime) < milliseconds * 1000) do
		process()
	end
end

function process()
	if (control.in_simulate()) then
		control.wait();
	else
		if (coroutine.running()) then
			coroutine.yield();
		else
			control.wait();
		end
	end
end

-- coroutine-ok parhuzamos futtatasa
function runparallel(t1, t2)
	local c1 = coroutine.create(t1)
	local c2 = coroutine.create(t2)
	while (coroutine.status(c1) == "suspended" or coroutine.status(c2) == "suspended") do
		if (coroutine.status(c1) == "suspended") then
			coroutine.resume(c1);
		end
		if (coroutine.status(c2) == "suspended") then
			coroutine.resume(c2);
		end
		process()
	end
end


local function MotionInProgress()
	while (control.MotionInProgress()) do
		process()
	end
end

local function Motion(motion, ...)
	if (not motion(...)) then
		control.music("error")
		control.print("(pilot) Motion hiba")
		return false
	end
	MotionInProgress()
	return (control.GetMotionError() == 0)
end

local function MotionSafe(motionSafe, ...)
	if (not motionSafe(...)) then
		control.music("error")
		control.print("(pilot) MotionSafe hiba")
		return false
	end
	MotionInProgress()
	return (control.GetMotionError() == 0)
end

function Go(...)
	return Motion(control.Go, ...)
end

function GoSafe(...)
	return MotionSafe(control.GoSafe, ...)
end

function GoTo(...)
	return Motion(control.GoTo, ...)
end

function GoToSafe(...)
	return MotionSafe(control.GoToSafe, ...)
end

function Turn(...)
	return Motion(control.Turn, ...)
end

function TurnSafe(...)
	return MotionSafe(control.TurnSafe, ...)
end

function TurnTo(...)
	return Motion(control.TurnTo, ...)
end

function TurnToSafe(...)
	return MotionSafe(control.TurnToSafe, ...)
end

function MotionStop(...)
	if (control.MotionStop(...)) then
		while (control.MotionStopInProgress()) do
			process()
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
			process()
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
			process()
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
			process()
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
			process()
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
			process()
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
			process()
		end
		return control.RefreshPawnPositionsFinished()
	end
	return false
end
