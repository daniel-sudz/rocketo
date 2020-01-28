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
bool are_we_finding_offsets = false;

float initial_pressure;  // pressure at setup
float current_pressure;  // pressure at loop

float reading_1_pressure;
float reading_2_pressure;

float x_gyro_acc = 0;
float y_gyro_acc = 0;
float z_gyro_acc = 0;

float x_gyro_offset = 0.5;
float y_gyro_offset = 0.63;
float z_gyro_offset = -0.69;

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
long count = 0;

void integrate_gyro() {
        IMU.readGyroscope(x_gyro_acc, y_gyro_acc, z_gyro_acc);
        //  Serial.println(y_gyro_acc);
        auto time = micros();
        long t_diff = time - last_time_clock;
        last_time_clock = time;
        count++;
        if ((count % 100) == 0) {
          // Serial.println(t_diff, 10);
        }
        x_gyro_acc -= x_gyro_offset;
        y_gyro_acc -= y_gyro_offset;
        z_gyro_acc -= z_gyro_offset;
        // Serial.println(x_gyro_acc);
        if (abs(x_gyro_acc) > 5) {
            x_gyro_angle += x_gyro_acc * t_diff;
        }
        if (abs(y_gyro_acc) > 5) {
            y_gyro_angle += y_gyro_acc * t_diff;
        }
        if (abs(z_gyro_acc) > 5) {
            z_gyro_angle += z_gyro_acc * t_diff;
        }
}

void on_board_led(int r, int g, int b) {
    analogWrite(ON_BOARD_LED_RED, r);
    analogWrite(ON_BOARD_LED_GREEN, g);
    analogWrite(ON_BOARD_LED_BLUE, b);
}

void setup() {
    on_board_led(255, 0, 0);
    Serial.begin(
        9600);  // opens serial port, sets data rate to 9600 bps debug serial
   // while (!Serial) delay(10);  // wait for computer to connect (debug)
    Serial.println("Begin program");

    // initialize IMU
    IMU.begin();
    while (!IMU.begin()) {
        Serial.println("fail int IMU");
        IMU.end();
        delay(100);
        IMU.begin();
    }

    // garbage read
    for (int i = 0; i < 2000; i++) {
        IMU.readGyroscope(x_gyro_acc, y_gyro_acc, z_gyro_acc);
    }
    // calibrate IMU offsets
    if (are_we_finding_offsets) {
      float found_x_gyro_offset = 0; float found_y_gyro_offset = 0; float found_z_gyro_offset = 0; 
        for (int i = 0; i < 10000; i++) {
            IMU.readGyroscope(x_gyro_acc, y_gyro_acc, z_gyro_acc);
            found_x_gyro_offset += x_gyro_acc;
            found_y_gyro_offset += y_gyro_acc;
            found_z_gyro_offset += z_gyro_acc;
        }
         found_x_gyro_offset = found_x_gyro_offset / (float)10000;
         found_y_gyro_offset = found_y_gyro_offset / (float)10000;
         found_z_gyro_offset = found_z_gyro_offset / (float)10000;

        Serial.println(found_x_gyro_offset);
        Serial.println(found_y_gyro_offset);
        Serial.println(found_z_gyro_offset);
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
    integrate_gyro();
    if (micros() % 100 == 1 && !are_we_finding_offsets) {
         Serial.print(y_gyro_angle / 1000000); Serial.print(" "); Serial.print(z_gyro_angle / 1000000); Serial.println(); 
    }
    if (abs(y_gyro_angle) > 100 * 1000000 || abs(z_gyro_angle) > 100 * 1000000 ) {
        on_board_led(0, 18, 179);  // yellowish
        Serial.println("DEPLOY");
        while (true) {
        }
    }
    //delay(1);
}