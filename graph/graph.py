""" Flowmeter graph with matplotlib """
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np
import serial
port = serial.Serial("/dev/ttyUSB0")
port.flushInput()

""" Variables """
x_range = 60
plt_count = 0
total_sensors = 2
figure = plt.figure()
axis_1 = plt.subplot2grid((7, 1), (0, 0), rowspan=3, colspan=1, fig=figure)
#plt.title("Gasto")
#plt.xlabel("Segundos")
#plt.ylabel("L/min")
axis_2 = plt.subplot2grid((7, 1), (4, 0), rowspan=3, colspan=1, fig=figure)
#plt.xlabel("Segundos")
#plt.ylabel("L/min")
time = [[],[]]
flow = [[],[]]

def animate(i):
    getSerialData()
    axis_1.clear()
    axis_2.clear()
    axis_1.plot(time[0], flow[0], label="Seccion 1")
    axis_2.plot(time[1], flow[1], label="Seccion 2")

def getSerialData():
    port_bytes = port.readline()
    decoded_bytes = str(port_bytes[0:len(port_bytes)-2].decode("utf-8")).split(",")
    data = [float(i) for i in decoded_bytes]
    flow[0].append(data[0])
    flow[1].append(data[1])
    global plt_count
    time[0].append(plt_count)
    time[1].append(plt_count)
    plt_count += 1
    #print(plt_count)
    #print(flow)

ani = animation.FuncAnimation(figure, animate, interval=1000)
plt.show()
