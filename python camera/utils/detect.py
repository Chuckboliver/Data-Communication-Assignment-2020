import cv2
import numpy as np
import random


class detect:
    def __init__(self):
        self.y = 10
        self.h = 105
        self.x = 20
        self.w = 105

        self.color_boundaries = {
            1: ([255, 255, 255], [255, 255, 255]),
            0: ([0, 0, 0], [0, 0, 0]),
        }

    def analyze(self, img):
        image = cv2.imread('./images/' + img + '.bmp')
        image = image[self.y:self.y + self.h, self.x:self.x + self.w]
        image2 = cv2.threshold(
            image, 55, 255, cv2.THRESH_BINARY)[1]

        pos = {
            'topLeft': [(20, 20), (20, 30), (30, 20), (30, 30)],
            'bottomLeft': [(20, 60), (20, 70), (30, 60), (30, 70)],
            'bottomRight': [(60, 60), (60, 70), (70, 60), (70, 70)],
            'topRight': [(60, 20), (60, 30), (70, 20), (70, 30)],
        }
        pos2 = ""
        data = ""
        data2 = ""
        for i, p in pos.items():
            averrage = 0
            averrage2 = 0
            for v in p:
                for color_name, (lower, upper) in self.color_boundaries.items():
                    lower = np.array(lower, dtype=np.uint8)
                    upper = np.array(upper, dtype=np.uint8)
                    mask = cv2.inRange(image2[v[1]][v[0]], lower, upper)
                    pos2 += str(int(v[0]//10)) + str(int(v[1]//10))

                    if mask.any():
                        data += str(image[v[1]][v[0]][0]) + '/'
                        averrage += color_name
                        averrage2 += image[v[1]][v[0]][0]
            averrage = int(round(averrage/4,0))
            averrage2 = averrage2//4
            data += str(averrage2) + '/'
            data2 += str(averrage)
            
        return data, data2
