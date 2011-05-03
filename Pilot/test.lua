function Control()
	if (coroutine.running()) then
		coroutine.yield();
	else
		ControlWait();
	end
end

-- coroutine-ok parhuzamos futtatasa
function RunParallel(t1, t2)
	while (coroutine.status(t1) == "suspended" or coroutine.status(t2) == "suspended") do
		if (coroutine.status(t1) == "suspended") then
			coroutine.resume(t1);
		end
		if (coroutine.status(t2) == "suspended") then
			coroutine.resume(t2);
		end
		Control();
	end
end

PI = 3.141592;
Offset = 0;
Ori = 1;

repeat Control(); until (CalibrateDeadreckoning() ~= 0);
Print("Calibrate finished");

repeat Control(); until (GetStartButton());

if (GetMyColor()) then
	Print("Kekek vagyunk");
	Offset = 3000;
	Ori = -1;
end

repeat Control(); until (Go(100) ~= 0);
Print("Odaertunk");

repeat Control(); until (SetGripperPos(90) ~= 0);

-- korong berakasa a biztos helyre
repeat Control(); until (GoTo(350, Offset + Ori * 800) ~= 0);
repeat Control(); until (Go(-200) ~= 0);
x, y, phi = GetRobotPos();
repeat Control(); until (Turn(-phi) ~= 0);
repeat Control(); until (GoTo(560, Offset + Ori * 740) ~= 0);
repeat Control(); until (GoTo(1600, Offset + Ori * 800) ~= 0);
local c1 = coroutine.create(function()
	repeat Control(); until (SetGripperPos(0) ~= 0);
end);
local c2 = coroutine.create(function()
	repeat Control(); until (GoTo(1770, Offset + Ori * 700) ~= 0);
end);
RunParallel(c1, c2);

local c1 = coroutine.create(function()
	repeat Control(); until (SetGripperPos(70) ~= 0);
end);
local c2 = coroutine.create(function()
	repeat Control(); until (SetConsolePos(100) ~= 0);
	repeat Control(); until (Go(-360) ~= 0);
end);
RunParallel(c1, c2);

-- paraszt kihuzasa a zold teruletrol
x, y, phi = GetRobotPos();
local c1 = coroutine.create(function()
	local c1 = coroutine.create(function()
		repeat Control(); until (SetGripperPos(90) ~= 0);
	end);
	local c2 = coroutine.create(function()
		repeat Control(); until (SetConsolePos(0) ~= 0);
	end);
	RunParallel(c1, c2);
end);
local c2 = coroutine.create(function()
	repeat Control(); until (TurnTo(690+2*280, y) ~= 0);
	repeat Control(); until (GoTo(690+2*280, y) ~= 0);
end);
RunParallel(c1, c2);

x, y, phi = GetRobotPos();
repeat Control(); until (TurnTo(x, Offset + Ori * 280) ~= 0);
repeat Control(); until (GoTo(x, Offset + Ori * 280) ~= 0);
repeat Control(); until (SetGripperPos(0) ~= 0);
repeat Control(); until (Go(-450) ~= 0);
repeat Control(); until (SetGripperPos(90) ~= 0);
repeat Control(); until (SetConsolePos(50) ~= 0);
repeat Control(); until (Go(-120) ~= 0);

-- kiraly felszedese
x, y, phi = GetRobotPos();
repeat Control(); until (TurnTo(690+3*280, y) ~= 0);
local c1 = coroutine.create(function()
	repeat Control(); until (SetConsolePos(0) ~= 0);
end);
local c2 = coroutine.create(function()
	repeat Control(); until (GoTo(690+3*280, y) ~= 0);
end);
RunParallel(c1, c2);

x, y, phi = GetRobotPos();
repeat Control(); until (TurnTo(x, Offset + Ori * 280) ~= 0);
repeat Control(); until (GoTo(x, Offset + Ori * 280) ~= 0);
repeat Control(); until (SetGripperPos(0) ~= 0);
repeat Control(); until (SetConsolePos(60) ~= 0);
repeat Control(); until (TurnTo(690+2*280, Offset + Ori * 550) ~= 0);
repeat Control(); until (GoTo(690+2*280, Offset + Ori * 550) ~= 0);

-- kiraly rarakasa a parasztra
x, y, phi = GetRobotPos();
repeat Control(); until (SetGripperPos(70) ~= 0);

-- felmegyunk az otthagyott paraszthoz
repeat Control(); until (SetConsolePos(0) ~= 0);
repeat Control(); until (SetGripperPos(0) ~= 0);
repeat Control(); until (TurnTo(700, Offset + Ori * 975) ~= 0);
repeat Control(); until (GoTo(700, Offset + Ori * 975) ~= 0);
repeat Control(); until (SetConsolePos(70) ~= 0);
repeat Control(); until (TurnTo(275, Offset + Ori * 975) ~= 0);
repeat Control(); until (GoTo(275, Offset + Ori * 975) ~= 0);
-- lerakjuk a tornyot
repeat Control(); until (SetGripperPos(90) ~= 0);
repeat Control(); until (Go(-220) ~= 0);

-- masik oldali kiraly felszedese
repeat Control(); until (SetConsolePos(0) ~= 0);
repeat Control(); until (SetGripperPos(45) ~= 0);
repeat Control(); until (TurnTo(690+3*280, Offset + Ori * 2400) ~= 0);
repeat Control(); until (GoTo(690+3*280, Offset + Ori * 2400) ~= 0);
repeat Control(); until (TurnTo(690+3*280, Offset + Ori * 2720) ~= 0);
repeat Control(); until (SetGripperPos(90) ~= 0);
repeat Control(); until (GoTo(690+3*280, Offset + Ori * 2720) ~= 0);
repeat Control(); until (SetGripperPos(0) ~= 0);
repeat Control(); until (SetConsolePos(60) ~= 0);

Exit();

repeat Control(); until (GoTo(275, 975) ~= 0);

repeat Control(); until (Go(-200) ~= 0);
x, y, phi = GetRobotPos();
repeat Control(); until (Turn(PI/2 - phi, 2, 2) ~= 0);
repeat Control(); until (GoTo(475, 1500) ~= 0);
x, y, phi = GetRobotPos();
repeat Control(); until (Turn(-phi, 2, 2) ~= 0);
repeat Control(); until (GoTo(1050, 1500) ~= 0);
repeat Control(); until (GoTo(1800, 1325) ~= 0);

repeat Control(); until (Go(-200) ~= 0);

--Exit();
