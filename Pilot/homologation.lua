
PI = 3.141592;
Offset = 0;
Ori = 1;
GripperGrab = 67;

repeat Control(); until (MotorSupply(true) ~= 0);
Print("Motortap bekapcsolva");

--repeat Control(); until (Sleep(2 * 1000 * 1000) ~= 0);
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

homologated = false;

if (GetMyColor()) then
	Print("Kekek vagyunk");
	Offset = 3000;
	Ori = -1;
end

repeat Control(); until (GoTo(250, Offset + Ori * 800) ~= 0);

-- Beszorulas feloldasa
function resolveDeadpos(turn, go)
	if (turn ~= 0) then
		while (TurnSafe(turn) == 0) do Control(); end;
	end;
	while (GoSafe(go) == 0) do Control(); end;
end
-- Beszorulas feloldasa

function homologate()
	repeat Control(); until (TurnToSafe(1250, Offset + Ori * 800) ~= 0);
	repeat Control(); until (GoToSafe(1250, Offset + Ori * 800) ~= 0);
	repeat Control(); until (TurnToSafe(1250, Offset + Ori * 300) ~= 0);
	repeat Control(); until (SetGripperPos(90) ~= 0);
	repeat Control(); until (GoToSafe(1250, Offset + Ori * 300) ~= 0);
	
	if (PawnInGripper()) then
		Print("Van paraszt!");
	end
	repeat Control(); until (SetGripperPos(GripperGrab) ~= 0);
	x, y, phi = GetRobotPos();
	repeat Control(); until (GoSafe(-500) ~= 0);
	repeat Control(); until (SetGripperPos(90) ~= 0);
	repeat Control(); until (GoSafe(-300) ~= 0);
end

local errorNum = 0;
repeat
	local status, err = pcall(function()
		Print("KEZDUNK");
		local deadpos = true;

		if (Simulate(homologate)) then
			deadpos = false;
			homologate();
			homologated = true;
		end		

		if (deadpos) then
			local deadPosResolved = false;
			repeat
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
		errorNum = errorNum + 1;
		if (errorNum == 5) then
			errorNum = 0;
			Print("Motor power cycle skip");
			--Print("Motortap kikapcsolas");
			--repeat Control(); until (MotorSupply(false) ~= 0);
			--repeat Control(); until (Sleep(5 * 1000 * 1000) ~= 0);
			--Print("Motortap bekapcsolas");
			--repeat Control(); until (MotorSupply(true) ~= 0);
			--repeat Control(); until (Sleep(2 * 1000 * 1000) ~= 0);
			--Print("Motor powercycle vege");
		end
		while (MotionStop(2000) == 0) do Control(); end;
	end
until (homologated);
