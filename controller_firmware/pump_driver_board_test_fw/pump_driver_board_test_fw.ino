#include <Servo.h>

const uint8_t servoPin = 4;
const uint8_t pumpSwitch = 5;
const uint8_t pumpSignal = 6;

uint8_t servoAngle = 0;

Servo loweringServo;

void sweepServo() {
    Serial.print("Angle:\t");
    for(int i = 0; i <= 180; i++) {
        loweringServo.write(i);
        if(i%25 == 0){
            Serial.println(" ");
        }
        Serial.print(i); Serial.print(", ");
        delay(20);
    }
    Serial.println(" ");
    for(int i = 180; i >= 0; i--) {
        loweringServo.write(i);
        if(i%25 == 0){
            Serial.println(" ");
        }
        Serial.print(i); Serial.print(", ");
        delay(20);
    }
}

void setup()
{
    pinMode(pumpSwitch, OUTPUT);
    pinMode(pumpSignal, INPUT);
    pinMode(servoPin, OUTPUT);

    loweringServo.attach(servoPin);
    loweringServo.write(90);

    // Always start with the pump off.
    digitalWrite(pumpSwitch, LOW);

    Serial.begin(115200);

    Serial.println("Board initialized");
    Serial.println("0:\tHalt operations");
    Serial.println("1:\tPower on pump");
    Serial.println("2:\tPower off pump");
    Serial.println("3:\tSweep servo");
    Serial.println("4:\tRetract servo");
    Serial.println("5:\tExtend servo");
    Serial.println("6:\tStop servo");
}

void loop()
{
    if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n');
        input.trim();

        if (input.length() == 0) {
            return;
        }

        const int userInput = input.toInt();

        switch (userInput) {
            case 0:
                Serial.println("Stopping operation...");
                digitalWrite(pumpSwitch, LOW);
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
                Serial.println("Sweeping servo...");
                sweepServo();
                break;

            case 4:
                Serial.println("Retracting servo...");
                // loweringServo.write(135);
                loweringServo.writeMicroseconds(1750);
                break;

            case 5:
                Serial.println("Extending servo...");
                // loweringServo.write(45);
                loweringServo.writeMicroseconds(1250);
                break;

            case 6:
                Serial.println("Stopping servo...");
                // loweringServo.write(90);
                loweringServo.writeMicroseconds(1500);
                break;

            default:
                Serial.print("Invalid option: ");
                Serial.println(userInput);
                break;
        }
    }
}