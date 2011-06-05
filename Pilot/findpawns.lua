PI = 3.141592;
Offset = 0;
Ori = 1;
GripperGrab = 67;

-- Beszorulas feloldasa
function resolveDeadpos(turn, go)
	if (turn ~= 0) then
		while (TurnSafe(turn) == 0) do Control(); end;
	end;
	while (GoSafe(go) == 0) do Control(); end;
end
-- Beszorulas feloldasa

function GoToNextPawn(x, y, px, py)
	while (SetGripperPos(0) == 0) do Control(); end;
	while (TurnToSafe(x, y) == 0) do Control(); end;
	while (GoToSafe(x, y) == 0) do Control(); end;
	while (TurnToSafe(px, py) == 0) do Control(); end;
	while (SetGripperPos(90) == 0) do Control(); end;
	while (GoToSafe(px, py) == 0) do Control(); end;
	if (PawnInGripper()) then
		while (SetGripperPos(65) == 0) do Control(); end;
	end
end

function DeployPawn(x1, y1, x2, y2)
	while (TurnToSafe(x1, y1) == 0) do Control(); end;
	while (GoToSafe(x1, y1) == 0) do Control(); end;
	while (TurnToSafe(x2, y2) == 0) do Control(); end;
	while (SetGripperPos(90) == 0) do Control(); end;
	while (GoSafe(-250) == 0) do Control(); end;
end


repeat Control(); until (MotorSupply(true) ~= 0);
Print("Motortap bekapcsolva");

repeat Control(); until (Sleep(2 * 1000 * 1000) ~= 0);
repeat Control(); until (GetStartButton());
Print("Startgomb lenyomva felvesszuk a kezdopoziciot");

repeat Control(); until (SetArmPos(true, 0) ~= 0);
Print("Balkar behajtva");
repeat Control(); until (SetArmPos(false, 0) ~= 0);
Print("Jobbkar behajtva");
repeat Control(); until (SetGripperPos(0) ~= 0);
Print("Gripper behajtva");

repeat Control(); until (CalibrateDeadreckoning() ~= 0);
Print("Calibrate finished");
repeat Control(); until (Sleep(2 * 1000 * 1000) ~= 0);

repeat Control(); until (Go(30) ~= 0);
x, y, phi = GetRobotPos();
repeat Control(); until (TurnTo(x, 1500) ~= 0);

repeat Control(); until (Sleep(5 * 1000 * 1000) ~= 0);

repeat Control(); until (GetStartButton());
StartMatch();
Print("Meccs elkezdodott");

if (GetMyColor()) then
	Print("Kekek vagyunk");
	Offset = 3000;
	Ori = -1;
end

--repeat Control(); until (RefreshPawnPositions() ~= 0);

repeat Control(); until (GoTo(250, Offset + Ori * 800) ~= 0);

repeat
	local status, err = pcall(function()
		Print("KEZDUNK");
		local deadpos = true;
		--repeat Control(); until (RefreshPawnPositions() ~= 0);
		
		local ignoreRadius = ROBOT_FRONT_MAX + PAWN_RADIUS;
		while (not PawnInGripper()) do
			--repeat Control(); until (RefreshPawnPositions() ~= 0);
			px, py, x, y, ignoreRadius = FindPawn(4, ignoreRadius);
			if (x) then
				if (Simulate(GoToNextPawn, x, y, px, py)) then
					deadpos = false;
					GoToNextPawn(x, y, px, py);
				else
					ignoreRadius = ignoreRadius + 1;
				end
			else
				Print("Nincs tobb paraszt!");
				break;
			end
		end;
		
		local priorityChange = 1; -- ennyivel kell modositanunk a prioritast
		local priorityChanged = 0; -- ennyiszer modositottunk mar adott priorityChange-el
		while (PawnInGripper()) do
			x1, y1, x2, y2, target, priority = GetDeployPoint();
			if (x1) then
				if (Simulate(DeployPawn, x1, y1, x2, y2)) then
					deadpos = false;
					DeployPawn(x1, y1, x2, y2);
					SetDeployPointPriority(target, 1); -- jeloljuk, hogy a mezo foglalt
				else
					SetDeployPointPriority(target, priority + priorityChange);
					if (priorityChange == priorityChanged) then
						-- ha mar priorityChange+1 -szer modositottunk, noveljuk priorityChanget
						priorityChange = priorityChange + 1;
						if (priority + priorityChange >= 0) then
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
		
		if (deadpos) then
			local deadPosResolved = false;
			repeat
				--repeat Control(); until (RefreshPawnPositions() ~= 0);
				local turn = 0;
				if (math.random() > 0.3) then
					turn = (math.random() - 0.5) * math.pi * 2;
				end
				local go = math.random(-1000, 1000);
				if (Simulate(resolveDeadpos, turn, go)) then
					deadPosResolved = true;
					resolveDeadpos(turn, go);
				end
			until (deadPosResolved);
		end
		
	end);
	if (not status) then
		Print("Hiba", err);
		while (MotionStop(2000) == 0) do Control(); end;
	end
	Control();
until (GetStopButton());