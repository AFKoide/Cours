import urx

rob = urx.Robot("192.168.0.100")
rob.set_tcp((0, 0, 0.1, 0, 0, 0))
rob.set_payload(2, (0, 0, 0.1))

sleep(0.2)  #leave some time to robot to process the setup commands

rob.movej((1, 2, 3, 4, 5, 6), a, v)
rob.movel((x, y, z, rx, ry, rz), a, v)


print("Current tool pose is: ",  rob.getl())
rob.movel((0.1, 0, 0, 0, 0, 0), a, v, relative=True)  # move relative to current pose
rob.translate((0.1, 0, 0), a, v)  #move tool and keep orientation
rob.stopj(a)

rob.movel((x, y, z, rx, ry, rz), wait=False)

while True :
    sleep(0.1)  #sleep first since the robot may not have processed the command yet
    if rob.is_program_running():
        break

rob.movel((x, y, z, rx, ry, rz), wait=False)
while rob.getForce() < 50:
    sleep(0.01)
    if not rob.is_program_running():
        break
rob.stopl()

try:
    rob.movel((0,0,0.1,0,0,0), relative=True)
except RobotError:
    print("Robot could not execute move (emergency stop for example), do something", ex)