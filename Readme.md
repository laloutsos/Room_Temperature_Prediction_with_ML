# Room Temperature Logger with Arduino & Python

This project is an ongoing attempt to explore how accurately we can **predict room temperature and humidity** using **sensor data** and basic 
**machine learning techniques**. It starts by collecting temperature and humidity 
data from a **DHT11 sensor** using an **Arduino**, and stores this data in real-time via a **Python serial logger** into a CSV file.

##  Project Goals

* Measure and log **room temperature and humidity** at regular intervals.
* Store the data with a **timestamp** for future training of predictive models.
* Build a pipeline that can later be used to train models for **time series forecasting**.

##  Technologies Used

* **Arduino Uno** with **DHT11** sensor
* **Python 3** for serial communication and CSV logging
* **CSV** format for easy data import/export

##  How It Works

### Arduino Sketch

* Reads from a DHT11 sensor every 60 seconds
* Sends the data via serial in the format:

  ```
  <seconds_since_start>,<temperature>,<humidity>
  ```

### Python Logger

* Listens to the Arduino serial output
* Gets current system time in **HHMM** format
* Saves each reading as:

  ```
  <time(HHMM)>,<temperature>,<humidity>
  ```
* Logs to `sensor_data.csv`

##  Live Demo / Progress

This project is **under active development**. I'll be updating this repository frequently as I:

* Improve data formatting
* Add real-time visualizations
* Train ML models to predict future room temperature values


---

