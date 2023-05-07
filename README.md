
# QR code and RFID data logger to Database using ESP32CAM
## Requirements

- ESP32CAM
- RDM6300
- Arduino RFID Library for RDM6300
- Arduino ESP32QRCodeReader Library
- Arduino IDE
- Arduino Core for ESP32CAM Using Arduino Boards Manager
- Jumper Wires
- XAMPP
- 0.96 inch OLED display (optional)
- White LED for better QR code scanning (optional)

## Connection Maps

### connection map for uploading code 

| ESP32CAM | RDM6300 |
|----------|---------|
| GPIO 3   | RX      |
| GPIO 2   | TX      |
| 5V       | 5V      |
| GND      | GND     |

![code uploading cercuit.jpg](img%2Fcode%20uploading%20cercuit.jpg)

### connection map for testing (without OLED Display)
| ESP32CAM | RDM6300       |
|----------| ------------- |
| GPIO 13  | Tx            |
| 5V       | 5V            |
| GND      | GND           |

![testing cercuit.jpg](img%2Ftesting%20cercuit.jpg)

### connection map for testing

**-- RDM6300 wiring --** 

| ESP32CAM | RDM6300       |
|----------| ------------- |
| GPIO 13  | Tx            |
| 5V       | 5V            |
| GND      | GND           |

**-- OLED display wiring --** 


| ESP32CAM | OLED display |
|----------|---------|
| GPIO 15  | SDA     |
| GPIO 14  | SDL     |
| 5V       | 5V      |
| GND      | GND     |

**-- LED wiring --**


| ESP32CAM | LED |
|----------|-----|
| GPIO 2   |  Anode |
| GND      | cathode     |

![test cercuit with Led and Oled Display .jpg](img%2Ftest%20cercuit%20with%20Led%20and%20Oled%20Display%20.jpg)