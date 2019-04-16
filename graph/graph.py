""" Flowmeter graph with matplotlib """
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import serial
port = serial.Serial("/dev/ttyACM0")
ser.flushInput()
""" Variables """
total_sensors = 2
figure = plt.figure()
axis_1 = plt.subplot2grid((7, 1), (0, 0), rowspan=3, colspan=1, fig=figure)
#plt.title("Gasto")
#plt.xlabel("Segundos")
#plt.ylabel("L/min")
axis_2 = plt.subplot2grid((7, 1), (4, 0), rowspan=3, colspan=1, fig=figure)
#plt.xlabel("Segundos")
#plt.ylabel("L/min")

time = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
flow = [0, 4, 7, 5, 6, 3, 2, 1, 3, 4]
""" Main loop """
def animate(i):
    port_bytes = port.readLine()
    decoded_bytes = float(ser_bytes[0:len(ser_bytes)-2].decode("utf-8"))
    print(decoded_bytes)
    axis_1.clear()
    axis_2.clear()
    axis_1.plot(time, flow, label="Seccion 1")
    axis_2.plot(time, flow, label="Seccion 2")

ani = animation.FuncAnimation(figure, animate, interval=1000)
plt.show()
