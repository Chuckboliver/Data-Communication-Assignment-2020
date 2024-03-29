import serial
from PIL import Image
import time
import sys


class camera:
    def __init__(self, port):
        self.width = 160
        self.height = 120
        self.command = '*RDY*'

        if port == None:
            port = input('Camera serial port : ')

        self.ser = serial.Serial(
            port=port,
            baudrate=1000000,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            bytesize=serial.EIGHTBITS)

    def read_image(self, angle):
        start = time.time()
        print('- Reading...')
        data = self.ser.read(self.width * self.height)
        end = time.time()
        timeTaken = int((end - start) * 1000)
        print('- Received Time taken {} ms'.format(timeTaken))
        image = Image.frombytes('L', (self.width, self.height), data)
        image.save('./images/' + angle + '.bmp')

    def isFound(self, index):
        if index < len(self.command):
            try:
                if self.command[index] == self.ser.read().decode('utf-8'):
                    return self.isFound(index+1)
                else:
                    return False
            except:
                return False
        return True

    def capture(self, angle):
        print('- Looking for image...')
        for i in range(3):
            self.clear()
        while not self.isFound(0):
            continue
        self.read_image(angle)

    def clear(self):
        while not self.isFound(0):
            continue
        self.read_image('test')
