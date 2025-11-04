# Getting Started with V4 Runtime

This guide will help you get V4 Runtime running on an ESP32-C6 board in about 10 minutes.

## Prerequisites

### Hardware

- ESP32-C6 development board (M5Stack NanoC6 recommended)
- USB cable for programming and power

### Software

- Linux, macOS, or Windows with WSL2
- Git
- Python 3.8 or later
- ESP-IDF v5.1 or later

## Step 1: Install ESP-IDF

If you haven't installed ESP-IDF yet:

```bash
# Clone ESP-IDF
mkdir -p ~/esp
cd ~/esp
git clone --recursive https://github.com/espressif/esp-idf.git
cd esp-idf
git checkout v5.1.2

# Install ESP-IDF
./install.sh esp32c6

# Source the environment (add this to your ~/.bashrc or ~/.zshrc)
. $HOME/esp/esp-idf/export.sh
```

## Step 2: Clone V4 Runtime

```bash
# Clone the repository
git clone https://github.com/V4-project/V4-runtime.git
cd V4-runtime
```

## Step 3: Build and Flash Hello RTOS Example

```bash
# Navigate to the hello-rtos example
cd bsp/esp32c6/examples/nanoc6/hello-rtos

# Configure (optional - default settings work for NanoC6)
idf.py menuconfig

# Build
idf.py build

# Flash to device (connect your board via USB)
idf.py flash

# Monitor serial output
idf.py monitor
```

Expected output:

```
V4 Runtime v1.0.0
Starting scheduler...
[Task 1] Hello from task 1!
[Task 2] Hello from task 2!
[Task 1] Message received: Hello from task 2
[Task 2] Message received: Hello from task 1
```

Press `Ctrl+]` to exit the monitor.

## Step 4: Understanding the Example

The hello-rtos example demonstrates:

1. **Task Creation** - Two concurrent tasks running
2. **Preemptive Scheduling** - Tasks automatically switch
3. **Message Passing** - Inter-task communication

Key files:

- `main/main.c` - Application entry point
- `main/CMakeLists.txt` - Component build configuration
- `CMakeLists.txt` - Project configuration
- `sdkconfig` - ESP-IDF configuration

## Step 5: Modify and Rebuild

Try modifying the example:

```c
// In main/main.c, change the message:
v4_send_message(task2_id, "Hello from modified task 1!");
```

Rebuild and flash:

```bash
idf.py build flash monitor
```

## Next Steps

Now that you have V4 Runtime running:

1. **Explore Examples** - Check other examples in `bsp/esp32c6/examples/`
2. **Read Architecture** - Understanding the [Architecture](architecture.md)
3. **API Reference** - Learn the [Kernel API](api-reference/kernel.md)
4. **Build Your Own** - Create your own V4 Runtime application

## Troubleshooting

### Build fails with "idf.py not found"

Make sure ESP-IDF is sourced:

```bash
. $HOME/esp/esp-idf/export.sh
```

### Flash fails with "no serial port found"

1. Check USB connection
2. Install USB-to-UART drivers if needed
3. Check permissions: `sudo usermod -a -G dialout $USER` (Linux)
4. Log out and back in

### Board doesn't respond after flashing

1. Press the reset button
2. Try erasing flash: `idf.py erase-flash`
3. Reflash: `idf.py flash`

## Additional Resources

- [ESP-IDF Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/)
- [M5Stack NanoC6 Guide](https://docs.m5stack.com/en/core/M5NanoC6)
- [V4 Runtime Architecture](architecture.md)
- [API Reference](api-reference/)

## Getting Help

- [GitHub Issues](https://github.com/V4-project/V4-runtime/issues)
- [Discussions](https://github.com/V4-project/V4-runtime/discussions)
