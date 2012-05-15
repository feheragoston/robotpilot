
-- felszedes karral
function Felszedes(waitforit)
	p.Compressor(true)
	p.sleep(200)
	p.ArmMove(105)
	p.sleep(200)
	p.ArmMove(10, 1000, 200)
	p.Valve(true)
	p.sleep(150)
	c.ArmMove(0)
	c.Valve(false)
	c.Compressor(false)
	if (waitforit) then
		while (control.CompressorInProgress()) do
			process()
		end
		while (control.ValveInProgress()) do
			process()
		end
		while (control.ArmMoveInProgress()) do
			process()
		end
	end
end

-- urites
function Urites(waitforit)
	-- Urites
	p.ConsoleMove(140, 1000, 15)
	
	-- Vissza
	p.ConsoleMove(20, 400, 15)
	c.CalibrateConsole()
	if (waitforit) then
		while (control.CalibrateConsoleInProgress()) do
			process()
		end
	end
end

-- Beszorulas feloldasa
function ResolveDeadpos(turn, go)
	if (turn ~= 0) then
		p.TurnSafe(turn)
	end;
	p.GoSafe(go)
end
-- Beszorulas feloldasa
