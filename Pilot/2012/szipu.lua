c.SetMotorSupply(true)

p.sleep(1000)

p.Compressor(true)
p.sleep(200)
p.ArmMove(100)
p.sleep(200)
p.ArmMove(10, 1000, 200)
p.Valve(true)
p.sleep(300)
p.Valve(false)
p.ArmMove(45)
p.Compressor(false)

