// -------------------------------
// Wiegand Reader for Arduino UNO
// Supports:
//  - 8-bit Wiegand keypads
//  - 26-bit RFID Wiegand readers
// -------------------------------

#define PIN_D0 2   // Wiegand DATA0 input pin
#define PIN_D1 3   // Wiegand DATA1 input pin

volatile uint32_t dataW = 0;          // Accumulates the received Wiegand bits
volatile uint8_t bitCount = 0;        // Number of bits received so far
volatile unsigned long lastBitTime = 0; // Timestamp of the last received bit

// -------------------------------
// Interrupt handlers
// These are triggered on the falling edge of DATA0/1.
// DATA0 = logical 0 bit
// DATA1 = logical 1 bit
// -------------------------------

void handleD0() {
  dataW <<= 1;      // Shift left and append a 0 bit
  bitCount++;
  lastBitTime = millis();
}

void handleD1() {
  dataW <<= 1;      // Shift left...
  dataW |= 1;       // ...and append a 1 bit
  bitCount++;
  lastBitTime = millis();
}

// --------------------------------------------------------
// 8-bit Wiegand keypad decoding
// Many numeric keypads send codes that decrease by 15
// for each digit (e.g., '0' = 240, '1' = 225, ... '9' = 105).
// ESC and ENTER have fixed codes.
// --------------------------------------------------------

char decodeKey(uint8_t code) {

  // Numeric keys: regular sequence (difference of 15)
  if (code <= 240 && code >= 105) {
    int diff = 240 - code;

    if (diff % 15 == 0) {               // Must follow the 15-step pattern
      int digit = diff / 15;

      if (digit >= 0 && digit <= 9)
        return '0' + digit;             // Convert to ASCII digit
    }
  }

  // Special keys
  if (code == 90) return 'E';           // ESC
  if (code == 75) return 'A';           // ENTER

  return '?';                           // Unknown / unsupported key
}

// -------------------------------
// Setup
// -------------------------------

void setup() {
  Serial.begin(9600);

  pinMode(PIN_D0, INPUT_PULLUP);
  pinMode(PIN_D1, INPUT_PULLUP);

  // Attach Wiegand interrupts (falling edge = new bit)
  attachInterrupt(digitalPinToInterrupt(PIN_D0), handleD0, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_D1), handleD1, FALLING);

  Serial.println("Wiegand Reader: Keypad (8bit) + RFID (26bit) ready...");
}

// -------------------------------
// Main loop
// Checks if a Wiegand transmission is complete.
// A gap >25 ms indicates end of packet.
// -------------------------------

void loop() {

  // If bits were received and no new bit has arrived for 25 ms:
  if (bitCount > 0 && (millis() - lastBitTime) > 25) {

    // --------- 8-bit KEYPAD ---------
    if (bitCount == 8) {
      uint8_t code = dataW;
      char key = decodeKey(code);

      Serial.print("[KEYPAD] Code: ");
      Serial.print(code);
      Serial.print(" → Key: ");
      Serial.println(key);
    }

    // --------- 26-bit RFID TAG ---------
    else if (bitCount == 26) {

      uint32_t raw = dataW;

      // Wiegand 26 bit layout:
      // P | 8-bit Facility Code | 16-bit Card ID | P
      byte facilityCode = (raw >> 17) & 0xFF;
      uint16_t cardID   = (raw >> 1) & 0xFFFF;

      Serial.println("[RFID] Wiegand 26 tag received!");

      Serial.print("RAW: ");
      Serial.println(raw, BIN);

      Serial.print("Facility Code: ");
      Serial.println(facilityCode);

      Serial.print("Card ID: ");
      Serial.println(cardID);
    }

    // --------- Unsupported format ---------
    else {
      Serial.print("[??] Unknown packet (");
      Serial.print(bitCount);
      Serial.println(" bits).");

      Serial.println(dataW, BIN);
    }

    // Reset for next transmission
    dataW = 0;
    bitCount = 0;
  }
}
