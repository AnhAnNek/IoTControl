import serial
import matplotlib.pyplot as plt
import json

# Initialize the serial connection
SERIAL_PORT = 'COM3'  # Replace with your actual COM port
BAUD_RATE = 115200
ser = serial.Serial(SERIAL_PORT, BAUD_RATE)

# Create lists to hold x and y coordinates
x = []
y = []

# Set up the plot
plt.ion()  # Interactive mode on
fig, ax = plt.subplots()
ax.set_xlim(-200, 200)  # Adjust as necessary
ax.set_ylim(-200, 200)  # Adjust as necessary
line, = ax.plot([], [], 'b-')  # Blue line for the plot

# Function to update the plot
def update_plot():
    line.set_data(x, y)
    plt.draw()
    plt.pause(0.01)

# Read data from the serial port and plot it
while True:
    if ser.in_waiting > 0:
        data = ser.readline().decode('utf-8').strip()  # Read and decode the data
        if data:
            print("Received data:", data)  # Log the received data
            try:
                # Parse the JSON data
                data_dict = json.loads(data)

                # Extract values from the JSON data
                angle = data_dict["angle"]
                distance = data_dict["distance"]
                newX = data_dict["x"]
                newY = data_dict["y"]

                # Append the new data to the lists
                x.append(newX)
                y.append(newY)

                # Update the plot incrementally
                update_plot()

            except json.JSONDecodeError:
                print("Failed to decode JSON:", data)
            except KeyError as e:
                print(f"Missing key in data: {e}")
            except ValueError:
                print("Invalid data format:", data)

    plt.pause(0.1)  # Allow time for plotting updates
