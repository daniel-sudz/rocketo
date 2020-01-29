#include <ArduinoBLE.h>
#include <Arduino_LPS22HB.h>
#include <Arduino_LSM9DS1.h>
#include <SD.h>
#include <SPI.h>
#include <Servo.h>

// CONFIGURATION STEPS
// Install arduino 33 port manager: tools>board>board manager>arduino 33
// ble>install Install the barometer class LPS22HB for the arduino 33 ble
// Include the barometer library: sketch>include_library>arduiono_LPS22HB
// //ALREADY INCLUDED, NOT NECESSARY CONFIGURATION STEPS

// GLOBAL VARIABLES
float x_acceleration = 0;
float y_acceleration = 0;
float z_acceleration = 0;

int ON_BOARD_LED_GREEN = 22;
int ON_BOARD_LED_RED = 23;
int ON_BOARD_LED_BLUE = 24;

Servo our_servo;

// HARDWARE SETUP
int SERVO_PORT = 3;
int LED_PORT = 0;
int SD_CARD = 0;

void on_board_led(int r, int g, int b) {
    analogWrite(ON_BOARD_LED_RED, r);
    analogWrite(ON_BOARD_LED_GREEN, g);
    analogWrite(ON_BOARD_LED_BLUE, b);
}

void setup() {
    on_board_led(255, 0, 0);

    Serial.begin(9600);
    // while (!Serial) delay(10);  // wait for computer to connect (debug)
    Serial.println("Begin program");

    our_servo.attach(2);
    our_servo.write(0);
    
        for (int i = 0; i < 20; i ++) {
            Serial.println(i);
           if (SD.begin(i)) {
              Serial.println(i);
              break;
         }
     }

    // initialize IMU
    IMU.begin();
    while (!IMU.begin()) {
        Serial.println("fail int IMU");
        IMU.end();
        delay(100);
        IMU.begin();
    }

    // garbage read
    for (int i = 0; i < 300; i++) {
        IMU.readAcceleration(x_acceleration, y_acceleration, z_acceleration);
    }

    // LED PINMODE INITIALIZATION
    pinMode(ON_BOARD_LED_GREEN, OUTPUT);
    pinMode(ON_BOARD_LED_RED, OUTPUT);
    pinMode(ON_BOARD_LED_BLUE, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);

    delay(100);
    on_board_led(0, 255, 0);
}

// the loop function runs over and over again forever
void loop() {
    // imu test
    if (IMU.readAcceleration(x_acceleration, y_acceleration, z_acceleration)) {
        if (x_acceleration > 2) {
            delay(10);
            long currtime = millis();
            Serial.println("LAUNCH");
            while (true) {
                if (IMU.readAcceleration(x_acceleration, y_acceleration, z_acceleration)) {
                    if (abs(x_acceleration) > 2) {
                        Serial.println(x_acceleration);
                    }
                    if ((abs(x_acceleration) < 0.2 &&
                         (millis() - currtime) > 2000) ||
                        (millis() - currtime > 3500)) {
                        Serial.println("DEPLOY");
                        while (true) {
                            // deploy code
                            our_servo.write(180);
                            delay(5000);
                        }
                        delay(10);
                    }
                    delay (10);
                }
            }
        }
    }
    delay(10);
}