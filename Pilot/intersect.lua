
repeat Control(); until (CalibrateDeadreckoning() ~= 0);
Print("Calibrate finished");

repeat Control(); until (RefreshPawnPositions() ~= 0);

repeat Control(); until (Go(100) ~= 0);

function go1()
	while (TurnTo(350, 2200) == 0) do Control(); end;
	while (GoTo(350, 2200) == 0) do Control(); end;
end
function go2()
	while (TurnTo(1600, 2200) == 0) do Control(); end;
	while (GoTo(1600, 2200) == 0) do Control(); end;
end
function go3()
	while (TurnTo(1600, 800) == 0) do Control(); end;
	while (GoTo(1600, 800) == 0) do Control(); end;
end
function go4()
	while (TurnTo(350, 800) == 0) do Control(); end;
	while (GoTo(350, 800) == 0) do Control(); end;
end

repeat
	if (not pcall(function()
		Print("KEZDUNK");
		go1();
		go4();
		go2();
		go4();
		go3();
		go4();
	end)) then
		while (MotionStop(2000) == 0) do Control(); end;
		while (pcall(go4) ~= true) do Control(); end;
	end
until (GetStopButton());