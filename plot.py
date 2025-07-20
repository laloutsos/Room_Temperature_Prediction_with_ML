import csv
import matplotlib.pyplot as plt

CSV_FILE = 'sensor_data.csv'

def read_data(csv_file):
    times = []
    humidities = []
    temperatures = []

    with open(csv_file, mode='r') as file:
        reader = csv.reader(file)
        for row in reader:
            if len(row) >= 3:
                time_int = int(row[0])
                hour = time_int // 100
                minute = time_int % 100
                time_str = f'{hour:02}:{minute:02}'
                times.append(time_str)

                humidities.append(float(row[2]))
                temperatures.append(float(row[1]))

    return times, humidities, temperatures

def plot_data(times, humidities, temperatures):
    plt.figure(figsize=(12, 6))

    # Temperature plot
    plt.subplot(2, 1, 1)
    plt.plot(times, temperatures, marker='o', color='red')
    plt.title('Temperature Over Time')
    plt.xlabel('Time')
    plt.ylabel('Temperature (°C)')
    plt.grid(True)

    # Humidity plot
    plt.subplot(2, 1, 2)
    plt.plot(times, humidities, marker='o', color='blue')
    plt.title('Humidity Over Time')
    plt.xlabel('Time')
    plt.ylabel('Humidity (%)')
    plt.grid(True)

    plt.tight_layout()
    plt.xticks(rotation=45)
    plt.show()

if __name__ == '__main__':
    times, humidities, temperatures = read_data(CSV_FILE)
    plot_data(times, humidities, temperatures)
