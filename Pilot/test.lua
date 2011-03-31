repeat ControlWait(); until (Go(100) ~= 0);
Print("Odaertunk");
repeat ControlWait(); until (GoTo(250, 1500) ~= 0);
Print("GoTo finished");
x, y, phi = GetRobotPos();
repeat ControlWait(); until (Turn(-phi) ~= 0);
Print("Forogtunk");
repeat ControlWait(); until (Go() ~= 0);
Print("Odaertunk");

Exit();