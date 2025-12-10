# ArduinoWiegandIO

This repository contains a simple Arduino sketch (`Wiegand.ino`) to read Wiegand-format data from RFID/card readers and 8-bit keypads. It is intended as a small, practical example to interface a common Wiegand reader with an Arduino for access-control experiments and hobby projects.

**Hardware used:**
- **Reader:** the reader used for testing is the one linked here: https://it.aliexpress.com/item/1005001453021419.html

**Features**
- **Reads Wiegand data:** Supports common 26-bit RFID and simple keypad Wiegand output.
- **Serial output:** Prints received codes and key presses to the serial console for debugging.

**Wiring (typical)**
- **GND:** Connect the reader ground to the Arduino `GND`.
- **DATA0 (D0):** Connect to the Arduino digital pin used for D0 (commonly `2`).
- **DATA1 (D1):** Connect to the Arduino digital pin used for D1 (commonly `3`).

Note: Pin numbers in `Wiegand.ino` can be changed to match your wiring. 

**Usage**
1. Open the project file `Wiegand.ino` in the Arduino IDE.
2. Ensure the pin definitions match your wiring (search for `D0`, `D1` or similar variables in the sketch).
3. Select the correct board and serial port in the Arduino IDE, then upload the sketch.
4. Open the Serial Monitor at the baud rate specified in the sketch (commonly `9600` or `115200`).
5. Present a card or press keys on the keypad — the sketch will print the decoded Wiegand code and/or key presses.

**Example Serial Output**
```
Wiegand: 26 bits, facility: 12, id: 34567
Keypad: pressed 5
```

**Troubleshooting**
- If you see no data: check wiring, power, and ground connections.
- If values seem wrong: confirm the reader uses the Wiegand format you expect (26-bit is common but other lengths exist).

**License**
- See the `LICENSE` file in this repository.

**Credits & Links**
- Original sketch and project by `nchetto` (this repository).
- Reader used for testing: the AliExpress product linked above.
