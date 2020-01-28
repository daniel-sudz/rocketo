#include <ArduinoBLE.h>
#include <Arduino_LPS22HB.h>
#include <Arduino_LSM9DS1.h>
#include <chrono>

// CONFIGURATION STEPS
// Install arduino 33 port manager: tools>board>board manager>arduino 33
// ble>install Install the barometer class LPS22HB for the arduino 33 ble
// Include the barometer library: sketch>include_library>arduiono_LPS22HB
// //ALREADY INCLUDED, NOT NECESSARY CONFIGURATION STEPS

// BAROMETER INFORMATION
// Hieght

// GLOBAL VARIABLES

float initial_pressure;  // pressure at setup
float current_pressure;  // pressure at loop

float reading_1_pressure;
float reading_2_pressure;

float x_gyro_acc = 0;
float y_gyro_acc = 0;
float z_gyro_acc = 0;

float x_gyro_angle = 0;
float y_gyro_angle = 0;
float z_gyro_angle = 0;

// const PinName ON_BOARD_LED_GREEN = PinName::p16;
// const PinName ON_BOARD_LED_RED = PinName::p24;
// const PinName ON_BOARD_LED_BLUE = PinName::p6;

int ON_BOARD_LED_GREEN = 22;
int ON_BOARD_LED_RED = 23;
int ON_BOARD_LED_BLUE = 24;

long last_time_clock = 0;

void integrate_gyro() {
    if (IMU.gyroscopeAvailable()) {
        IMU.readGyroscope(x_gyro_acc, y_gyro_acc, z_gyro_acc);

        long t_diff = micros() - last_time_clock;
        double t_diff_dec = (double)t_diff / (double)1000000;
        //  if (micros() % 100 == 1) {
        //    Serial.println(t_diff_dec, 10);
        //  }
        if (abs(x_gyro_acc) > 10) {
            x_gyro_angle += x_gyro_acc * t_diff_dec;
        }
        if (abs(y_gyro_acc) > 10) {
            y_gyro_angle += y_gyro_acc * t_diff_dec;
        }
        if (abs(z_gyro_acc) > 10) {
            z_gyro_angle += z_gyro_acc * t_diff_dec;
        }
        last_time_clock = micros();
    }
}

void on_board_led(int r, int g, int b) {
    analogWrite(ON_BOARD_LED_RED, r);
    analogWrite(ON_BOARD_LED_GREEN, g);
    analogWrite(ON_BOARD_LED_BLUE, b);
}

void setup() {
    // initialize IMU
    IMU.begin();

    // LED PINMODE INITIALIZATION
    pinMode(ON_BOARD_LED_GREEN, OUTPUT);
    pinMode(ON_BOARD_LED_RED, OUTPUT);
    pinMode(ON_BOARD_LED_BLUE, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);

    on_board_led(255, 0, 0);

    Serial.begin(
        9600);  // opens serial port, sets data rate to 9600 bps debug serial
   // while (!Serial) delay(10);
    Serial.println("Begin program");

    delay(100);
    on_board_led(0, 255, 0);
}

// the loop function runs over and over again forever
void loop() {
    // imu test
    integrate_gyro();
  //  if (micros() % 10 == 1) {
        //Serial.println(y_gyro_angle);
   // }
    if (abs(y_gyro_angle) > 100) {
        on_board_led(0, 18, 179);  // yellowish
        Serial.println("DEPLOY"); 
        while (true) {
          
        }
    }
    // delay(5);
}