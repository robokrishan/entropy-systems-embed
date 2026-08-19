#include <Servo.h>

#define USER_CONTROLLED 1
#define STOP_SPOOL      90
#define LOWER_NOZZLE    180
#define RAISE_NOZZLE    0 

const uint8_t pumpSwitch = 5;
const uint8_t pumpSignal = 3;
const uint8_t servoSignal = 9;

Servo myServo;

const unsigned long pwmTimeoutUs = 30000;
const unsigned int pumpThresholdUs = 1500;

void setup()
{
    pinMode(pumpSwitch, OUTPUT);
    pinMode(pumpSignal, INPUT);

    // Always start with the pump off.
    digitalWrite(pumpSwitch, LOW);

    // Initialize servo
    myServo.attach(servoSignal);
    myServo.writeMicroseconds(1500);

    Serial.begin(115200);

    Serial.println("Board initialized");
    Serial.println("0:\tHalt operations");
    Serial.println("1:\tPower on pump");
    Serial.println("2:\tPower off pump");
    Serial.println("3:\tLower pump nozzle");
    Serial.println("4:\tRaise pump nozzle");
    Serial.println("5:\tStop spool");
}

void loop()
{
#ifdef USER_CONTROLLED

    if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n');
        input.trim();

        if (input.length() == 0) {
            return;
        }

        const int userInput = input.toInt();

        switch (userInput) {
            case 0:
                Serial.println("Stopping all operations...");
                digitalWrite(pumpSwitch, LOW);
                myServo.writeMicroseconds(1500);
                break;

            case 1:
                Serial.println("Turning on pump...");
                digitalWrite(pumpSwitch, HIGH);
                break;

            case 2:
                Serial.println("Turning off pump...");
                digitalWrite(pumpSwitch, LOW);
                break;

            case 3:
                Serial.println("Lowering pump nozzle...");
                myServo.writeMicroseconds(1800);
                break;

            case 4:
                Serial.println("Raising pump nozzle...");
                myServo.writeMicroseconds(1200);
                break;

            case 5:
                Serial.println("Stopping spool...");
                myServo.writeMicroseconds(1500);
                break;

            default:
                Serial.print("Invalid option: ");
                Serial.println(userInput);
                break;
        }
    }


#else

    const unsigned long pulseWidth = pulseIn(pumpSignal, HIGH, pwmTimeoutUs);

    Serial.print("PWM pulse: ");
    Serial.print(pulseWidth);
    Serial.println(" us");

    // A timeout returns 0. Treat this as pump OFF.
    if (pulseWidth >= 800 &&
        pulseWidth <= 2200 &&
        pulseWidth > pumpThresholdUs) {

        digitalWrite(pumpSwitch, HIGH);
        Serial.println("Pump: ON");
    } else {
        digitalWrite(pumpSwitch, LOW);
        Serial.println("Pump: OFF");
    }

    delay(100);

#endif
}