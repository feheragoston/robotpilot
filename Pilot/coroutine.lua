
p.runparallel(
50000,
function()
	for i = 1, 10 do
		c.print("Elso thread", i);
		p.process()
		error("Hiba!")
	end
end,
function()
	p.sleep(500)
	for i = 1, 5 do
		c.print("Masodik thread", i);
		p.process()
	end
end,
function(x)
	p.sleep(1000)
	for i = 1, 5 do
		c.print("Harmadik thread", i);
		p.process()
	end
end
);


c.exit();