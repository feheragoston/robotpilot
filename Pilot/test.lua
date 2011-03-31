repeat ControlWait(); until (Go() ~= 0);
Print("Odaertunk");
repeat ControlWait(); until (Turn(-3.14/2) ~= 0);
Print("Forogtunk");
repeat ControlWait(); until (Go() ~= 0);
Print("Odaertunk");

Exit();