c.SetMotorSupply(true)
p.sleep(500)
p.CalibrateConsole()

p.MoveToSafe(700, 495)
p.TurnToSafe(2000, 495)

-- Urites
p.ConsoleMove(140, 1000, 15)

-- Vissza
p.ConsoleMove(20, 400, 15)
p.CalibrateConsole()


-- Lassu
-- Urites
p.ConsoleMove(140, 100, 15)

-- Vissza
p.ConsoleMove(20, 400, 15)
p.CalibrateConsole()
