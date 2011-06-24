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

while (Go(100) == 0) do Control(); end;
Print("Odaertunk");

repeat
	while (TurnTo(350, 2200) == 0) do Control(); end;
	while (GoTo(350, 2200) == 0) do Control(); end;
	while (TurnTo(1600, 2200) == 0) do Control(); end;
	while (GoTo(1600, 2200) == 0) do Control(); end;
	while (TurnTo(1600, 800) == 0) do Control(); end;
	while (GoTo(1600, 800) == 0) do Control(); end;
	while (TurnTo(350, 800) == 0) do Control(); end;
	while (GoTo(350, 800) == 0) do Control(); end;
until (GetStopButton());

Exit();
