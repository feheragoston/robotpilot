
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
	if (not TurnToSafe(x, y)) then
		return false
	end
	return GoToSafe(x, y)
end

function MoveTo(x, y)
	if (not TurnTo(x, y)) then
		return false
	end
	return GoTo(x, y)
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

-- aktuatorok kezeleses

-- arm
-- selector
-- flipper
-- contractor
-- firestopper

-- firewheel
-- caracole
--console

--ballcolor
-- StartButton
-- StopButton
-- MyColor switch


--Arm
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

--Selector
function SelectorMove(...)
	if (control.SelectorMove(...)) then
		while (control.SelectorMoveInProgress()) do
			process()
		end
	else
		control.print("(pilot) SelectorMove hiba")
		return false
	end
	return true
end

--Flipper
function FlipperMove(...)
	if (control.FlipperMove(...)) then
		while (control.FlipperMoveInProgress()) do
			process()
		end
	else
		control.print("(pilot) FlipperMove hiba")
		return false
	end
	return true
end

--TODO: toggle flipper

--Contractor
function ContractorMove(...)
	if (control.ContractorMove(...)) then
		while (control.ContractorMoveInProgress()) do
			process()
		end
	else
		control.print("(pilot) ContractorMove hiba")
		return false
	end
	return true
end

function SetContractorOpen()
	return ContractorMove(90) --update to open deg
end

function SetContractorClosed()
	return ContractorMove(0) --update to closed deg
end

--FireStopper
function FireStopperMove(...)
	if (control.FireStopperMove(...)) then
		while (control.FireStopperMoveInProgress()) do
			process()
		end
	else
		control.print("(pilot) FireStopperMove hiba")
		return false
	end
	return true
end


--dc motorok

--FireWheel
function FireWheelSetSpeed(...)
	if (control.FireWheelSetSpeed(...)) then
		while (control.FireWheelSetSpeedInProgress()) do
			process()
		end
	else
		control.print("(pilot) FireWheelSetSpeed hiba")
		return false
	end
	return true
end

function SetFireWheelOn()
	return FireWheelSetSpeed(fireWheelSpeed)
end

function SetFireWheelOff()
	return FireWheelSetSpeed(0)
end

--Caracole
function CaracoleSetSpeed(...)
	if (control.CaracoleSetSpeed(...)) then
		while (control.CaracoleSetSpeedInProgress()) do
			process()
		end
	else
		control.print("(pilot) CaracoleSetSpeed hiba")
		return false
	end
	return true
end

function SetCaracoleOn()
	return FireWheelSetSpeed(fireWheelSpeed)
end

function SetCaracoleOff()
	return FireWheelSetSpeed(0)
end

--Console
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