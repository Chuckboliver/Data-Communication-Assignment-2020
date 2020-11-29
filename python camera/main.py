from utils.control import control

controller = control()
while True:
    while controller.ser.inWaiting():
        command = controller.ser.read_until().decode('utf-8')
        print(command)
        command = command.replace('\r', '')
        command = command.replace('\n', '')
        controller.sendCommand(command)
