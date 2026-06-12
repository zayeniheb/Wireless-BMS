import serial
import time
import csv
import os

csv_file = "data2.csv"
file_exists = os.path.isfile(csv_file)

ser = serial.Serial('COM11', 115200, timeout=1)
time.sleep(2)

print("Reading and saving serial data...")

with open(csv_file, 'a', newline='') as file:
    writer = csv.writer(file, delimiter=';')

    if not file_exists:
        writer.writerow(["Timestamp", "IP", "MAC", "tension", "temperature", "current"])

    while True:
        try:
            # Read and parse each line
            ip = mac = tension = temp = current = ""

            while ser.in_waiting > 0:
                line = ser.readline().decode('utf-8', errors='ignore').strip()
                print("Received:", line)

                if line.startswith("IP:"):
                    ip = line.split("IP:")[1]
                elif line.startswith("adresse MAC:"):
                    mac = line.split("adresse MAC:")[1]
                elif line.startswith("tension:"):
                    tension = line.split("tension:")[1]
                elif line.startswith("temperature:"):
                    temp = line.split("temperature:")[1]
                elif line.startswith("current:"):
                    current = line.split("current:")[1]

                    # When 'current' is received last, write the full line
                    timestamp = time.strftime("%Y-%m-%d %H:%M:%S")
                    writer.writerow([timestamp, ip, mac, tension, temp, current])
                    file.flush()
                    print("✔ Data saved.")

        except Exception as e:
            print("Error:", e)
