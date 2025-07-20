import serial
import csv
import time
from datetime import datetime

SERIAL_PORT = 'COM3'  
BAUD_RATE = 9600
CSV_FILE = 'sensor_data.csv'

def main():
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE)
    time.sleep(2)

    with open(CSV_FILE, mode='a', newline='') as file:
        writer = csv.writer(file)

        while True:
            print("Reading Serial Monitor...")
            line = ser.readline().decode('utf-8').strip()
            if line:
                print(line)

                data = line.split(',')
                if len(data) >= 2:  
                    now = datetime.now()
                    hour_min = now.hour * 100 + now.minute 

                    temperature = data[0]
                    humidity = data[1]

                    writer.writerow([hour_min, temperature, humidity])
                    file.flush()

if __name__ == '__main__':
    main()
