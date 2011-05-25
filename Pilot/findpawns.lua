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

function GoToNextPawn(x, y)
	while (SetGripperPos(90) == 0) do Control(); end;
	while (TurnToSafe(x, y) == 0) do Control(); end;
	while (GoToSafe(x, y) == 0) do Control(); end;
	if (PawnInGripper()) then
		while (SetGripperPos(0) == 0) do Control(); end;
	end
end

function DeployPawn(x1, y1, x2, y2)
	while (TurnToSafe(x1, y1) == 0) do Control(); end;
	while (GoToSafe(x1, y1) == 0) do Control(); end;
	while (TurnToSafe(x2, y2) == 0) do Control(); end;
	while (SetGripperPos(90) == 0) do Control(); end;
	while (GoSafe(-200) == 0) do Control(); end;
end

repeat
	local status, err = pcall(function()
		Print("KEZDUNK");
		
		while (not PawnInGripper()) do
			repeat Control(); until (RefreshPawnPositions() ~= 0);
			px, py, x, y = FindPawn(1);
			if (x) then
				if (Simulate(GoToNextPawn, x, y)) then
					GoToNextPawn(x, y);
				else
					
				end
			else
				Print("Nincs tobb paraszt!");
				break;
			end
		end;
		
		priorityChange = 1; -- ennyivel kell modositanunk a prioritast
		priorityChanged = 0; -- ennyiszer modositottunk mar adott priorityChange-el
		while (PawnInGripper()) do
			x1, y1, x2, y2, target, priority = GetDeployPoint();
			if (x1) then
				if (Simulate(DeployPawn, x1, y1, x2, y2)) then
					DeployPawn(x1, y1, x2, y2);
					SetDeployPointPriority(target, 1); -- jeloljuk, hogy a mezo foglalt
				else
					SetDeployPointPriority(target, priority + priorityChange);
					if (priorityChange == priorityChanged) then
						-- ha mar priorityChange+1 -szer modositottunk, noveljuk priorityChanget
						priorityChange = priorityChange + 1;
						if (priority + priorityChange <= 0) then
							-- ha a prioritas elerne a 0-t, akkor korbeertunk, nem tudunk lerakni
							Print("Nem tudunk egyik lerako poziciohoz sem odamenni");
							Control();
							break;
						end
						priorityChanged = 0;
					else
						priorityChanged = priorityChanged + 1;
					end
				end
			else
				Print("Nincs tobb lerako pozicio")
				Control();
				break;
			end
		end;
		
	end);
	if (not status) then
		Print("Hiba", err);
		while (MotionStop(2000) == 0) do Control(); end;
	end
until (GetStopButton());