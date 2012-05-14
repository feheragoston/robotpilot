
-- felszedes karral
function PickupWithArm()
	p.Compressor(true)
	p.sleep(200)
	p.ArmMove(100)
	p.sleep(200)
	p.ArmMove(10, 1000, 200)
	p.Valve(true)
	p.sleep(100)
	p.Valve(false)
	p.ArmMove(0)
	p.Compressor(false)
end

-- Beszorulas feloldasa
function ResolveDeadpos(turn, go)
	if (turn ~= 0) then
		p.TurnSafe(turn)
	end;
	p.GoSafe(go)
end
-- Beszorulas feloldasa
