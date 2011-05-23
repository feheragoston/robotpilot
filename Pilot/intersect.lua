
PI = 3.141592;
Offset = 0;
Ori = 1;

repeat Control(); until (CalibrateDeadreckoning() ~= 0);
Print("Calibrate finished");

if (GetMyColor()) then
	Print("Kekek vagyunk");
	Offset = 3000;
	Ori = -1;
end

repeat Control(); until (RefreshPawnPositions() ~= 0);

repeat Control(); until (Go(100) ~= 0);

function go1()
	while (TurnTo(350, Offset + Ori * 800) == 0) do Control(); end;
	while (GoTo(350, Offset + Ori * 800) == 0) do Control(); end;
end
function go2()
	while (TurnTo(350, Offset + Ori * 2200) == 0) do Control(); end;
	while (GoTo(350, Offset + Ori * 2200) == 0) do Control(); end;
end
function go3()
	while (TurnTo(1600, Offset + Ori * 2200) == 0) do Control(); end;
	while (GoTo(1600, Offset + Ori * 2200) == 0) do Control(); end;
end
function go4()
	while (TurnTo(1600, Offset + Ori * 800) == 0) do Control(); end;
	while (GoTo(1600, Offset + Ori * 800) == 0) do Control(); end;
end

function resolveDeadpos1()
	while (Go(-1000) == 0) do Control(); end;
end

function resolveDeadpos2()
	while (Go(600) == 0) do Control(); end;
end

function resolveDeadpos3()
	while (Go(-100) == 0) do Control(); end;
end

function resolveDeadpos4()
	while (Go(150) == 0) do Control(); end;
end

function resolveDeadpos5()
	while (Turn(PI/2) == 0) do Control(); end;
	while (Go(300) == 0) do Control(); end;
end

function resolveDeadpos6()
	while (Turn(-PI/2) == 0) do Control(); end;
	while (Go(300) == 0) do Control(); end;
end

repeat
	if (not pcall(function()
		Print("KEZDUNK");
		repeat Control(); until (RefreshPawnPositions() ~= 0);
		
		deadpos = true;
		
		if (Simulate(go1)) then
			deadpos = false;
			go1();
		else
			Print("go1 kihagyas");
		end
		if (Simulate(go2)) then
			deadpos = false;
			go2();
		else
			Print("go2 kihagyas");
		end
		if (Simulate(go3)) then
			deadpos = false;
			go3();
		else
			Print("go3 kihagyas");
		end
		if (Simulate(go4)) then
			deadpos = false;
			go4();
		else
			Print("go4 kihagyas");
		end
		
		if (deadpos) then
			if (Simulate(resolveDeadpos5)) then
				resolveDeadpos5();
			elseif (Simulate(resolveDeadpos6)) then
				resolveDeadpos6();
			elseif (Simulate(resolveDeadpos1)) then
				resolveDeadpos1();
			elseif (Simulate(resolveDeadpos2)) then
				resolveDeadpos2();
			elseif (Simulate(resolveDeadpos3)) then
				resolveDeadpos3();
			elseif (Simulate(resolveDeadpos4)) then
				resolveDeadpos4();
			end
		end
	end)) then
		while (MotionStop(2000) == 0) do Control(); end;
	end
until (GetStopButton());