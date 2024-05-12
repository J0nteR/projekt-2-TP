# pip install pyserial
import serial.tools.list_ports
import time

ports = serial.tools.list_ports.comports()
serialInst = serial.Serial()
portsList = []

for one in ports:
    portsList.append(str(one))
    print(str(one))

com = input("Select Com Port for Arduino #: ")

for i in range(len(portsList)):
    if portsList[i].startswith("COM" + str(com)):
        use = "COM" + str(com)
        print(use)

serialInst.baudrate = 115200
serialInst.port = use
serialInst.open()



"""
Sätt att kommunicera med arduinon genom serial communication
"""
while True:
    command = input("Arduino Command (1/2/3/exit): ")
    serialInst.write(command.encode('utf-8'))

    if command == 'exit':
        exit()
        
