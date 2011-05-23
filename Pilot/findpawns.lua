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

repeat Control(); until (Go(200) ~= 0);
Print("Odaertunk");

while (SetGripperPos(90) == 0) do Control(); end;

function GoToNextPawn()
	repeat Control(); until (RefreshPawnPositions() ~= 0);
	px, py, x, y = FindPawn(1);
	if (x) then
		while (TurnToSafe(x, y) == 0) do Control(); end;
		while (GoToSafe(x, y) == 0) do Control(); end;
		while (SetGripperPos(0) == 0) do Control(); end;

		x1, y1, x2, y2 = GetDeployPoint();
		if (x1) then
			while (TurnToSafe(x1, y1) == 0) do Control(); end;
			while (GoTo(x1, y1) == 0) do Control(); end;
			while (TurnTo(x2, y2) == 0) do Control(); end;
			while (SetGripperPos(90) == 0) do Control(); end;
			while (Go(-200) == 0) do Control(); end;
		else
			Print("Nincs tobb lerako pozicio")
		end
	else
		Print("Nincs tobb paraszt!");
	end
end

repeat
	if (not pcall(function()
		GoToNextPawn();
	end)) then
		Print("Hiba");
		while (MotionStop(2000) == 0) do Control(); end;
	end
until (GetStopButton());