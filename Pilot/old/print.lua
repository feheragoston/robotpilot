
repeat Control(); until (CalibrateDeadreckoning() ~= 0);

repeat
	Print(GetRobotPos());
	while (Sleep(2000000) == 0) do Control(); end;
until (GetStopButton());