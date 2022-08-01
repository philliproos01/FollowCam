import sensor, time, image
import pyb, ustruct


sensor.reset()

sensor.set_contrast(3)
sensor.set_gainceiling(16)

sensor.set_framesize(sensor.HQVGA)
sensor.set_pixformat(sensor.GRAYSCALE)


face_cascade = image.HaarCascade("frontalface", stages=25)
print(face_cascade)


clock = time.clock()

text = "This is a test!\n"
data = ustruct.pack("<%ds" % len(text), text)
pos1 = ""


bus = pyb.I2C(2, pyb.I2C.SLAVE, addr=0x12)
bus.deinit()
bus = pyb.I2C(2, pyb.I2C.SLAVE, addr=0x12)
print("Waiting for Arduino...")

while (True):
    clock.tick()


    img = sensor.snapshot()


    objects = img.find_features(face_cascade, threshold=0.75, scale_factor=1.25)


    for r in objects:
        img.draw_rectangle(r)
        print (r)
        pos1 = "POS_1 = " + str(r[0])+ "\n"
        pos2 = "POS_2 = " + str(r[1])+ "\n"
        pos3 = "POS_3 = " + str(r[2])+ "\n"
        pos4 = "POS_4 = " + str(r[3])+ "\n"

        try:
            bus.send(ustruct.pack("<h", len(pos1)), timeout=10000)
            try:
                bus.send(pos1, timeout=10000)


                print("Sent Data 1!")
            except OSError as err:
                pass
        except OSError as err:
            pass
        try:
            bus.send(ustruct.pack("<h", len(pos2)), timeout=10000)
            try:
                bus.send(pos2, timeout=10000)


                print("Sent Data 2!")
            except OSError as err:
                pass
        except OSError as err:
            pass

        try:
            bus.send(ustruct.pack("<h", len(pos3)), timeout=10000)
            try:
                bus.send(pos3, timeout=10000)


                print("Sent Data 3!")
            except OSError as err:
                pass
        except OSError as err:
            pass
