import serial
import time
import glob
import sys
import threading

def find_stm32_serial():
    """
    Finds the first USB CDC (STM32) device by scanning /dev/ttyACM*
    """
    devices = glob.glob('/dev/ttyACM*')
    if not devices:
        print("No STM32 device found!")
        sys.exit(1)
    # Optionally, you can try to filter by checking USB vendor/product ID with lsusb
    print(f"Found serial devices: {devices}")
    return devices[0]  # pick the first one

# Auto-detect STM32 port
port = find_stm32_serial()
baudrate = 115200

try:
    ser = serial.Serial(port, baudrate, timeout=1)
    time.sleep(2)  # wait for STM32 to reset if needed
    print(f"Connected to {port} at {baudrate} baud.")
except Exception as e:
    print(f"Failed to open serial port: {e}")
    sys.exit(1)

# Shared state
current_char = b'3'  # default to training
running = True

def user_input_thread():
    global current_char, running
    while running:
        cmd = input("Enter '3' for training, '4' for inference, 'q' to quit: ").strip()
        if cmd == '3':
            current_char = b'3'
            print("Switched to training mode.")
        elif cmd == '4':
            current_char = b'4'
            print("Switched to inference mode.")
        elif cmd.lower() == 'q':
            running = False
            print("Quitting...")
        else:
            print("Invalid input, choose '3', '4', or 'q'.")

# Start the input thread
threading.Thread(target=user_input_thread, daemon=True).start()

# Continuous send loop
while running:
    try:
        ser.write(current_char)
        time.sleep(0.01)  # adjust the frequency if needed
    except KeyboardInterrupt:
        running = False
        break

ser.close()
print("Serial port closed.")

# # Continuous send loop
# while True:
#     try:
#         # Example: send '3' for training, '4' for inference
#         ser.write(b'3')  
#         time.sleep(0.01)  # small delay, adjust if needed
#     except KeyboardInterrupt:
#         print("Stopping...")
#         ser.close()
#         break