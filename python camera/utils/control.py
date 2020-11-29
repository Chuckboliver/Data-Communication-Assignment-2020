import sys
import serial
from utils.servo import servo
from utils.camera import camera
from utils.detect import detect


class control:
    def __init__(self):
        port = [None, None, None, None]

        for i in range(len(sys.argv)):
            port[i] = sys.argv[i]

        if port[1] == None:
            port[1] = input('PC2 serial port : ')

        self.dect = detect()
        self.servo = servo(port[2])
        self.cam = camera(port[3])
        self.binary = {}

        self.ser = serial.Serial(port=port[1], baudrate=9600, timeout=1)

    def sendCommand(self, command):
        if command == '00':
            self.goto('left')
            self.goto('center')
            self.goto('right')
            self.ser.write(
                ('01' + self.binary['left'] + '10' + self.binary['center'] + '11' + self.binary['right'] + '\n').encode())
        elif command in self.binary.values():
            for p, v in self.binary.items():
                if command == v:
                    self.servo.move(p)
                    self.cam.capture(p)
                    if command != self.dect.analyze(p)[1]:
                        self.ser.write((self.dect.analyze(p)[0]+'0/').encode())
                    else:
                        self.ser.write((self.dect.analyze(p)[0]+'1/').encode())

    def goto(self, pos):
        self.servo.move(pos)
        self.cam.capture(pos)
        self.binary[pos] = self.dect.analyze(pos)[1]
