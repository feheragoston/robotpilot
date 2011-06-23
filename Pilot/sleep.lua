package.path = package.path .. ";./Pilot/?.lua"

c = control
p = require("pilot")

c.print("Indul");

p.runparallel(
function()
	local i = 0;
	repeat
		p.sleep(1000);
		c.print("1 sec");
		i = i + 1;
	until (i >= 2);
end,
function()
	local i = 0;
	repeat
		p.sleep(2000);
		c.print("2 sec");
		i = i + 1;
	until (i >= 2);
end);

--c.exit();
