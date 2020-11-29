import serial


class servo:
    def __init__(self, port):
        if port == None:
            port = input('Servo serial port : ')

        self.ser = serial.Serial(port=port, baudrate=115200, timeout=1)

    def move(self, mode):
        print('+ Moving to ' + mode + '...')
        self.ser.write((mode + '\n').encode())
        isMoving = True
        while isMoving:
            while self.ser.inWaiting():
                s = self.ser.read_until().decode('utf-8')
                if 'Moved' in s:
                    print('+ Moved')
                    isMoving = not isMoving
