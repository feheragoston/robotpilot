
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