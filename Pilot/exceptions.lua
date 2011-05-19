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

repeat Control(); until (Go(100) ~= 0);
Print("Odaertunk");

move = protect(function()
	repeat Control(); until (Go(1000) ~= 0);
	repeat Control(); until (Go(-1000) ~= 0);
	return true;
end);

domove = protect(function()
	local try = newtry(function()
		Print("Finalize");
	end);

	while true do
		try(move());
	end;
end);

domove();

Print("End");

Exit();