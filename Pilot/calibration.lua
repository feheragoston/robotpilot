
PI = 3.141592;
Offset = 0;
Ori = 1;
GripperGrab = 67;

repeat Control(); until (MotorSupply(true) ~= 0);
Print("Motortap bekapcsolva");

repeat Control(); until (Sleep(2 * 1000 * 1000) ~= 0);
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
repeat Control(); until (Sleep(1 * 1000 * 1000) ~= 0);

repeat Control(); until (Go(30) ~= 0);
x, y, phi = GetRobotPos();
repeat Control(); until (TurnTo(x, 1500) ~= 0);

repeat Control(); until (Sleep(1 * 1000 * 1000) ~= 0);
