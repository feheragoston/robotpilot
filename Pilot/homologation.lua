
dofile("Pilot/calibration.lua")

p.Go(50, 100, 100)
p.TurnTo(300, Offset + Ori * 790)

repeat p.process() until (c.GetStartButton());
c.StartMatch();
c.print("Meccs elkezdodott");

p.GoToSafe(300, Offset + Ori * 790)


p.TurnToSafe(860, 720 * Ori + Offset)
if (c.simulate(p.GoToSafe, 860, 720 * Ori + Offset)) then
	p.GoToSafe(860, 720 * Ori + Offset)
end
if (c.simulate(p.TurnToSafe, 860, 500 * Ori + Offset)) then
	p.TurnToSafe(860, 500 * Ori + Offset)
end
if (c.simulate(p.GoToSafe, 860, 500 * Ori + Offset)) then
	p.GoToSafe(860, 500 * Ori + Offset)
end
if (c.simulate(p.GoSafe, -220)) then
	repeat p.GoSafe(-220) until (c.GetMotionError() == 0)
end


if (c.simulate(p.TurnToSafe, 1775, 670 * Ori + Offset)) then
	p.TurnToSafe(1775, 670 * Ori + Offset)
	if (c.simulate(p.GoToSafe, 1775, 670 * Ori + Offset)) then
		p.GoToSafe(1775, 670 * Ori + Offset)
		p.ClawMove(false, 30)
		p.ClawMove(false, 2)
		if (c.simulate(p.GoSafe, -220)) then
			repeat p.GoSafe(-220) until (c.GetMotionError() == 0)
		end
	end
end




if (c.simulate(p.TurnToSafe, 1700, Offset + Ori * 790)) then
	p.TurnToSafe(1700, Offset + Ori * 790)
	if (c.simulate(p.GoToSafe, 1700, Offset + Ori * 790)) then
		p.GoToSafe(1700, Offset + Ori * 790)
		if (c.simulate(p.TurnToSafe, 600, Offset + Ori * 790)) then
			p.TurnToSafe(600, Offset + Ori * 790)
			p.GripperMove(false, 95)
			p.GripperMove(true, 95)
			p.ClawMove(false, 120)
			p.ClawMove(true, 120)
			if (c.simulate(p.GoToSafe, 600, Offset + Ori * 790)) then
				p.GoToSafe(600, Offset + Ori * 790)
				p.GripperMove(true, 55)
				p.GripperMove(false, 55)
				p.ClawMove(false, 55)
				p.ClawMove(true, 55)
				if (c.simulate(p.TurnToSafe, 720, Offset + Ori * 700)) then
					p.TurnToSafe(720, Offset + Ori * 700)
				end
				if (c.simulate(p.GoToSafe, 720, Offset + Ori * 700)) then
					p.GoToSafe(720, Offset + Ori * 700)
				end
				if (c.simulate(p.TurnToSafe, 720, Offset + Ori * 400)) then
					p.TurnToSafe(720, Offset + Ori * 400)
				end
				if (c.simulate(p.GoToSafe, 720, Offset + Ori * 400)) then
					p.GoToSafe(720, Offset + Ori * 400)
				end
				p.GripperMove(true, 95)
				p.GripperMove(false, 95)
				if (c.simulate(p.GoSafe, -300)) then
					repeat p.GoSafe(-300) until (c.GetMotionError() == 0)
				end
				p.GripperMove(true, 0)
				p.GripperMove(false, 1)
				p.ClawMove(true, 0)
				p.ClawMove(false, 2)
			end
		end
	end
end



if (c.simulate(p.TurnToSafe, 400, 2000 * Ori + Offset)) then
	p.TurnToSafe(400, 2000 * Ori + Offset)
	if (c.simulate(p.GoToSafe, 400, 2000 * Ori + Offset)) then
		p.GoToSafe(400, 2000 * Ori + Offset)
		if (c.simulate(p.TurnToSafe, 860, 2280 * Ori + Offset)) then
			p.TurnToSafe(860, 2280 * Ori + Offset)
			if (c.simulate(p.GoToSafe, 860, 2280 * Ori + Offset)) then
				p.GoToSafe(860, 2280 * Ori + Offset)
				p.runparallel(
					function()
						p.GripperMove(false, 95)
					end,
					function()
						p.GripperMove(true, 95)
					end
				)
				if (c.simulate(p.TurnToSafe, 860, 2600 * Ori + Offset)) then
					p.TurnToSafe(860, 2600 * Ori + Offset)
					if (c.simulate(p.GoToSafe, 860, 2600 * Ori + Offset)) then
						p.GoToSafe(860, 2600 * Ori + Offset)
						p.runparallel(
							function()
								p.GripperMove(false, 55)
							end,
							function()
								p.GripperMove(true, 55)
							end
						)
						if (c.simulate(p.GoSafe, -300)) then
							repeat p.GoSafe(-300) until (c.GetMotionError() == 0)
							if (c.simulate(p.TurnToSafe, 400, 2000 * Ori + Offset)) then
								p.TurnToSafe(400, 2000 * Ori + Offset)
								if (c.simulate(p.GoToSafe, 400, 2000 * Ori + Offset)) then
									p.GoToSafe(400, 2000 * Ori + Offset)
									if (c.simulate(p.TurnToSafe, 250, 400 * Ori + Offset)) then
										p.TurnToSafe(250, 400 * Ori + Offset)
										if (c.simulate(p.GoToSafe, 250, 400 * Ori + Offset)) then
											p.GoToSafe(250, 400 * Ori + Offset)
											p.runparallel(
												function()
													p.GripperMove(false, 95)
												end,
												function()
													p.GripperMove(true, 95)
												end
											)
										end
									end
								end
							end
						end
					end
				end
			end
		end
	end
end
