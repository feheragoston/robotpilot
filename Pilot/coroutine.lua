--[[
function Control()
	if (coroutine.running()) then
		Print("	Control hivas coroutine-bol");
		coroutine.yield();
	else
		Print("> Control hivas mainbol");
		ControlWait();
	end
end
]]

-- coroutine-ok parhuzamos futtatasa
--[[
function RunParallel(t1, t2)
	while (coroutine.status(t1) == "suspended" or coroutine.status(t2) == "suspended") do
		if (coroutine.status(t1) == "suspended") then
			coroutine.resume(t1);
		end
		if (coroutine.status(t2) == "suspended") then
			coroutine.resume(t2);
		end
		if (coroutine.status(t1) == "suspended" or coroutine.status(t2) == "suspended") then
			Control();
		end
	end
end
]]

Test()
RunParallel(
function(x)
	Print(x);
	Control();
	Print(x);
end,
"Elso thread",
function(x)
	Print(x);
	Control();
	Test1();
	Control();
	Test1();
	Control();
end,
"Masodik thread",
function(x)
	Print(x);
	Control();
	Test2();
	Control();
	Test2();
	Control();
	Test2();
	Control();
	Test2();
	Control();
end,
"Harmadik thread"
);


Exit();