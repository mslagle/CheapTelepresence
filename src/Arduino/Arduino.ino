const int MOTORLEFT_BACKWARDPIN = 0;
const int MOTORLEFT_FORWARDPIN = 1;
const int MOTORLEFT_SPEEDPIN = 9;

const int MOTORRIGHT_BACKWARDPIN = 2;
const int MOTORRIGHT_FORWARDPIN = 3;
const int MOTORRIGHT_SPEEDPIN = 10;

const int STEPPER1_DIRECTIONPIN = 4;
const int STEPPER1_STEPPIN = 5;

const int STEPPER_INTERVAL = 3;

char input[31];
char* params;

const int motorMin = 64;	// Adjust min and max depending on motors
const int motorMax = 255;

const long controlFactor = 1000L;	// Adjust depending on motors

// Current motor controls
int setRightPower = 500;
int setLeftPower = -500;
int currentRightPower = 0;
int currentLeftPower = 0;
int leftMotorValue = 0;
int rightMotorValue = 0;

unsigned long stepper1Interval = 0;
int stepper1Value = LOW;
int stepper1Direction = 1;

// Set right motor power in [-1000, 1000]
void motorRight(int power)
{
	power = constrain(power, -1000, 1000);

	digitalWrite(MOTORRIGHT_BACKWARDPIN, (power < 0 ? HIGH : LOW));
	digitalWrite(MOTORRIGHT_FORWARDPIN, (power > 0 ? HIGH : LOW));

	if (power != 0)
	{
		int mapped = map(abs(power), 0, 1000, motorMin, motorMax);
		analogWrite(MOTORRIGHT_SPEEDPIN, mapped);
	}
	else
	{
		analogWrite(MOTORRIGHT_SPEEDPIN, 0);
	}
}

// Set left motor power in [-1000, 1000]
void motorLeft(int power)
{
	power = constrain(power, -1000, 1000);

	digitalWrite(MOTORLEFT_BACKWARDPIN, (power < 0 ? HIGH : LOW));
	digitalWrite(MOTORLEFT_FORWARDPIN, (power > 0 ? HIGH : LOW));

	if (power != 0)
	{
		int mapped = map(abs(power), 0, 1000, motorMin, motorMax);
		analogWrite(MOTORLEFT_SPEEDPIN, mapped);
	}
	else
	{
		analogWrite(MOTORLEFT_SPEEDPIN, 0);
	}
}

void stepper1(int value)
{
	unsigned long currentMillis = millis();
	if (value != 0 && (currentMillis - stepper1Interval) > STEPPER_INTERVAL)
	{
		digitalWrite(STEPPER1_DIRECTIONPIN, (value > 0 ? HIGH : LOW));
		digitalWrite(STEPPER1_STEPPIN, stepper1Value);
		stepper1Value = stepper1Value > LOW ? LOW : HIGH;
		stepper1Interval = currentMillis;
	}
}

void communication()
{
	while (Serial.available())
	{
		char input[30 + 1];
		byte size = Serial.readBytesUntil('\n', input, 30);
		input[size] = 0;

		Serial.print("Size: ");
		Serial.println(size);

		Serial.print("All: ");
		Serial.println(input);

		char cmd = input[0];
		switch (cmd)
		{
		case 'A':
			// Read the command
			params = strtok(input, " ");

			// Read arm motor
			params = strtok(NULL, " ");
			stepper1Direction = atoi(params);

			Serial.print("Arm Direction: ");
			Serial.println(stepper1Direction);

			break;
		case 'M': // motors
			// Read the command
			params = strtok(input, " ");

			// Read left motor
			params = strtok(NULL, " ");
			leftMotorValue = atoi(params);

			// Read right motor
			params = strtok(NULL, " ");
			rightMotorValue = atoi(params);

			setLeftPower = constrain(leftMotorValue, -100, 100) * 10;
			setRightPower = constrain(rightMotorValue, -100, 100) * 10;

			/*
			Serial.print("Left: ");
			Serial.println(setLeftPower);
			Serial.print("Right: ");
			Serial.println(setRightPower);
			*/

			break;
		case 'B': // battery
			Serial.print("B ");
			Serial.print(100);
			Serial.println();
			break;
		default:
			Serial.println("E");
			break;
		}
	}
}

// the setup function runs once when you press reset or power the board
void setup()
{
	// Init pins
	pinMode(MOTORRIGHT_BACKWARDPIN, OUTPUT);
	pinMode(MOTORRIGHT_FORWARDPIN, OUTPUT);
	pinMode(MOTORRIGHT_SPEEDPIN, OUTPUT);

	pinMode(MOTORLEFT_BACKWARDPIN, OUTPUT);
	pinMode(MOTORLEFT_FORWARDPIN, OUTPUT);
	pinMode(MOTORLEFT_SPEEDPIN, OUTPUT);

	pinMode(STEPPER1_DIRECTIONPIN, OUTPUT);
	pinMode(STEPPER1_STEPPIN, OUTPUT);

	// Init serials
	Serial.begin(9600);
	Serial.println("Init");
}

// the loop function runs over and over again until power down or reset
void loop()
{
	// Set the left power
	if (currentLeftPower > setLeftPower)
	{
		--currentLeftPower;
	}
	if (currentLeftPower < setLeftPower)
	{
		++currentLeftPower;
	}

	// Set the right power
	if (currentRightPower > setRightPower)
	{
		--currentRightPower;
	}
	if (currentRightPower < setRightPower)
	{
		++currentRightPower;
	}

	motorLeft(currentLeftPower);
	motorRight(currentRightPower);

	stepper1(stepper1Direction);

	communication();
}
