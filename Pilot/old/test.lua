
PI = 3.141592;
Offset = 0;
Ori = 1;

repeat Control(); until (CalibrateDeadreckoning() ~= 0);
Print("Calibrate finished");

i = 0;
repeat
RunParallel(
function()
	--Control();
	--Print("Test", i);
	--Control();
	--Print("Test", i);
end);
i = i + 1;
until (i > 10);

RunParallel(
function()
	Control();
	--Print("Test", i);
	Control();
	--Print("Test", i);
end);

RunParallel(
function()
	Control();
	--Print("Test", i);
	Control();
	--Print("Test", i);
end);

repeat Control(); until (RefreshPawnPositions() ~= 0);

repeat Control(); until (GetStartButton());

if (GetMyColor()) then
	Print("Kekek vagyunk");
	Offset = 3000;
	Ori = -1;
end

repeat Control(); until (RefreshPawnPositions() ~= 0);

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
RunParallel(
	function()
		repeat Control(); until (SetGripperPos(0) ~= 0);
	end,
	function()
		repeat Control(); until (GoTo(1770, Offset + Ori * 700) ~= 0);
	end
);

RunParallel(
	function()
		repeat Control(); until (SetGripperPos(70) ~= 0);
	end,
	function()
		repeat Control(); until (SetConsolePos(100) ~= 0);
		repeat Control(); until (Go(-360) ~= 0);
	end
);

repeat Control(); until (RefreshPawnPositions() ~= 0);

-- paraszt kihuzasa a zold teruletrol
x, y, phi = GetRobotPos();
RunParallel(
	function()
		repeat Control(); until (SetGripperPos(90) ~= 0);
	end,
	function()
		repeat Control(); until (SetConsolePos(0) ~= 0);
	end,
	function()
		repeat Control(); until (TurnTo(690+2*280, y) ~= 0);
		repeat Control(); until (GoTo(690+2*280, y) ~= 0);
	end
);

x, y, phi = GetRobotPos();
repeat Control(); until (TurnTo(x, Offset + Ori * 280) ~= 0);
repeat Control(); until (GoTo(x, Offset + Ori * 280) ~= 0);
repeat Control(); until (SetGripperPos(0) ~= 0);
repeat Control(); until (Go(-450) ~= 0);
repeat Control(); until (SetGripperPos(90) ~= 0);
repeat Control(); until (SetConsolePos(50) ~= 0);
repeat Control(); until (Go(-120) ~= 0);

repeat Control(); until (RefreshPawnPositions() ~= 0);

-- kiraly felszedese
x, y, phi = GetRobotPos();
repeat Control(); until (TurnTo(690+3*280, y) ~= 0);
RunParallel(function()
	repeat Control(); until (SetConsolePos(0) ~= 0);
end, function()
	repeat Control(); until (GoTo(690+3*280, y) ~= 0);
end);

x, y, phi = GetRobotPos();
repeat Control(); until (TurnTo(x, Offset + Ori * 280) ~= 0);
repeat Control(); until (GoTo(x, Offset + Ori * 280) ~= 0);
repeat Control(); until (SetGripperPos(0) ~= 0);
repeat Control(); until (SetConsolePos(60) ~= 0);
repeat Control(); until (TurnTo(690+2*280, Offset + Ori * 550) ~= 0);
repeat Control(); until (GoTo(690+2*280, Offset + Ori * 550) ~= 0);

repeat Control(); until (RefreshPawnPositions() ~= 0);

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

repeat Control(); until (RefreshPawnPositions() ~= 0);

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

repeat Control(); until (RefreshPawnPositions() ~= 0);

Exit();
