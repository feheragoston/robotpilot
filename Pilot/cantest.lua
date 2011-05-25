

repeat Control(); until (MotorSupply(true) ~= 0);

repeat Control(); until (GetStartButton());

repeat Control(); until (SetArmPos(true, 0) ~= 0);
repeat Control(); until (SetArmPos(false, 0) ~= 0);
repeat Control(); until (SetGripperPos(0) ~= 0);

repeat Control(); until (Go(1000) ~= 0); Print("Odaertunk");
repeat Control(); until (Go(1000) ~= 0); Print("Odaertunk");