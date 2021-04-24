
//Pins for the motor controller, LM298N
#define LEFT1 12
#define LEFT2 11
#define LEFT 9

#define RIGHT1 10
#define RIGHT2 8
#define RIGHT 7

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
bool instruction;

void setup(){
	// Declare all the tank tracks as outputs
	for(int i=7;i<=12;i++){ pinMode(i,OUTPUT); }

	Serial.begin(9600);
 // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}

void loop(){
  if (stringComplete) {
    //Serial.println(inputString);
    // If the input data is information about motors
    if((inputString.toInt()>5)||(inputString.toInt()==0)){
      Serial.println(inputString);
      int angulo=(inputString.substring(1,5).toInt())/10;
      Serial.print("Angulo de entrada es ");
      Serial.println(angulo);
      // The last 3 chars are the velocity
      int pedal=(inputString.substring(5).toInt())/10;
      Serial.print("Gas es ");
      Serial.println(pedal);
      // Write the data to the motors
      write_motors(angulo,pedal);
    }
    // If it is a button
    else{
      Serial.println("Instrucción Recibida");
    }
    inputString = "";
    stringComplete = false;
  }
}

void write_motors(int angle, int gas){
	// Converting to radians and
	// Calculate both X and Y components
	float y=cos((angle*3.1416)/180)*(gas);
	float x=sin((angle*3.1416)/180)*(gas);

  Serial.println(x);
  Serial.println(y);
  
  
	// Intermidiate step
	float V=(100-abs(x))*(y/100)+y; // R+L
	float W=(100-abs(y))*(x/100)+x; // R-L
	// Final values for the tracks, map it to be a byte for PWM
  
	print_right(map(((V+W)/2),-100,100,-255,255));
	print_left(map(((V-W)/2),-100,100,-255,255));
}

// Function to print to the left track
void print_left(int vel){
	if(vel>=0){
		digitalWrite(LEFT1,HIGH);
		digitalWrite(LEFT2,LOW);
	}
	else{
		digitalWrite(LEFT1,LOW);
		digitalWrite(LEFT2,HIGH);
	}
	analogWrite(LEFT,abs(vel));
  Serial.print("Left wheel: ");
  Serial.println(vel);
}

// Function to print to the right track
void print_right(int vel){
	if(vel>=0){
		digitalWrite(RIGHT1,HIGH);
		digitalWrite(RIGHT2,LOW);
	}
	else{
		digitalWrite(RIGHT1,LOW);
		digitalWrite(RIGHT2,HIGH);
	}
	analogWrite(RIGHT,abs(vel));
  Serial.print("Right wheel: ");
  Serial.println(vel);
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
