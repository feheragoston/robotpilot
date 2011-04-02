repeat ControlWait(); until (CalibrateDeadreckoning() ~= 0);
Print("Calibrate finished");
repeat ControlWait(); until (Go(100) ~= 0);
Print("Odaertunk");
repeat ControlWait(); until (GoTo(250, 1500) ~= 0);
Print("GoTo finished");
x, y, phi = GetRobotPos();
repeat ControlWait(); until (Turn(-phi) ~= 0);
Print("Forogtunk");
repeat ControlWait(); until (Go() ~= 0);
Print("Odaertunk");
while (SetSpeed(100, 0.5) == 0) do ControlWait(); end;

Exit();