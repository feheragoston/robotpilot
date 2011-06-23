
p.runparallel(
5000,
function()
	p.sleep(1000)
	for i = 1, 10 do
		c.print("Elso thread", i);
		p.process()
	end
end,
function()
	p.sleep(500)
	for i = 1, 90 do
		c.print("Masodik thread", i);
		p.process()
	end
end,
function(x)
	for i = 1, 100 do
		c.print("Harmadik thread", i);
		p.process()
	end
end
);


c.exit();