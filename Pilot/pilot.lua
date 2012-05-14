
local control = control
local coroutine = coroutine
local type = type
local error = error

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

function process(wait)
	if (wait == nil) then
		wait = PRIMITIVES_WAIT
	end
	
	if (control.in_simulate()) then
		control.wait(wait);
	else
		if (coroutine.running()) then
			coroutine.yield();
		else
			control.wait(wait);
		end
	end
end

-- functionok parhuzamos futtatasa
function runparallel(...)
	local params = {...}
	local threads = {}
	local wait = PRIMITIVES_WAIT
	
	for i = 1, #params do
		if (type(params[i]) == "function") then
			threads[#threads + 1] = coroutine.create(params[i])
		elseif (type(params[i]) == "number") then
			wait = params[i]
		end
	end

	local running = true
	while (running) do
		running = false
		for i = 1, #threads do
			if (coroutine.status(threads[i]) == "suspended") then
				running = true
				local status, message = coroutine.resume(threads[i])
				if (not status) then
					error(message)
				end
			end
		end
		process(wait)
	end
end


local function SetEyeColor(...)
	if (control.SetEyeColor(...)) then
		while (control.SetEyeColorInProgress()) do
			process()
		end
	else
		control.print("(pilot) SetEyeColor hiba")
		return false
	end
	return true
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

function MoveToSafe(x, y)
	if (control.simulate(TurnToSafe, x, y)) then
		if (not TurnToSafe(x, y)) then
			return false
		end
		if (control.simulate(GoToSafe, x, y)) then
			return GoToSafe(x, y)
		else
			return false
		end
	else
		return false
	end
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

function GripperMove(left, ...)
	if (control.GripperMove(left, ...)) then
		while (control.GripperMoveInProgress(left)) do
			process()
		end
	else
		control.print("(pilot) GripperMove hiba", left)
		return false
	end
	return true
end

function ClawMove(left, ...)
	if (control.ClawMove(left, ...)) then
		while (control.ClawMoveInProgress(left)) do
			process()
		end
	else
		control.print("(pilot) ClawMove hiba", left)
		return false
	end
	return true
end

function ArmMove(...)
	if (control.ArmMove(...)) then
		while (control.ArmMoveInProgress()) do
			process()
		end
	else
		control.print("(pilot) ArmMove hiba")
		return false
	end
	return true
end

function CalibrateConsole(...)
	if (control.CalibrateConsole(...)) then
		while (control.CalibrateConsoleInProgress()) do
			process()
		end
	else
		control.print("(pilot) CalibrateConsole hiba")
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

function Compressor(...)
	if (control.Compressor(...)) then
		while (control.CompressorInProgress()) do
			process()
		end
	else
		control.print("(pilot) Compressor hiba")
		return false
	end
	return true
end

function Valve(...)
	if (control.Valve(...)) then
		while (control.ValveInProgress()) do
			process()
		end
	else
		control.print("(pilot) Valve hiba")
		return false
	end
	return true
end

function ResetPressure()
	if (control.ResetPressure(true)) then
		while (control.ResetPressureInProgress()) do
			process()
		end
	else
		control.print("(pilot) ResetPressure hiba: true")
		return false
	end
	if (control.ResetPressure(false)) then
		while (control.ResetPressureInProgress()) do
			process()
		end
	else
		control.print("(pilot) ResetPressure hiba: false")
		return false
	end
	return true
end