PI = 3.141592;
Offset = 0;
Ori = 1;
GripperGrab = 67;

-- START ---------------------------------------------

Print("Sleep");
repeat Control(); until (Sleep(5000000) ~= 0);
Print("Calibrate");
repeat Control(); until (CalibrateDeadreckoning() ~= 0);

if (GetMyColor()) then
	Print("Kekek vagyunk");
	Offset = 3000;
	Ori = -1;
end

Print("GoTo");
repeat Control(); until (GoTo(250, Offset + Ori * 800) ~= 0);
