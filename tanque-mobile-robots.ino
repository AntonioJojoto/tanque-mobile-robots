#include <Servo.h>

//Pins for the motor controller, LM298N
#define LEFT1 12
#define LEFT2 11
#define LEFT 6

#define RIGHT1 8
#define RIGHT2 7
#define RIGHT 5

// Values for the servos
#define turrent 3
#define cannon 10

Servo torreta;
Servo laser;

//Min values for the turrent
#define angle_min 80
#define angle_max 180

// Degrees for the servos
int angle_turrent=90;
int angle_cannon=90;

// Lowest value to get the tank moving


String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
bool instruction;
int MIN=55;

void setup(){
	// Declare all the tank tracks as outputs
	for(int i=5;i<=13;i++){ pinMode(i,OUTPUT); }
	// Attach the servos
	torreta.attach(turrent);
	laser.attach(cannon);
	for(int a=90;a>=150;a++){
		laser.write(a);
		delay(100);
	}

	Serial.begin(9600);
	// reserve 200 bytes for the inputString:
	inputString.reserve(200);
}

void loop(){

	if (stringComplete) {
		BT_mode(inputString);
		inputString = "";
		stringComplete = false;
	}
}

int write_cannon(int angle){
	if(angle>angle_max){ return angle_max; }
	else if(angle<angle_min){return angle_min;}
	else{laser.write(angle);  return angle;}
}

int write_torreta(int angle){
	if(angle>180){ return 180; }
	else if(angle<0){return 0;}
	else{torreta.write(angle); return angle;}
}


// Function to use with BT
void BT_mode(String inputString){
	Serial.println(inputString);
    switch(inputString.substring(5).toInt()){
      case 1:
        Serial.println("Boton A");
	angle_cannon+=2;
	angle_cannon=write_cannon(angle_cannon);
        break;
      case 2:
        Serial.println("Boton B");
	for(int i=0;i<=4;i++){
		angle_turrent++;
		angle_turrent=write_torreta(angle_turrent);
		delay(200/5);
	}
        break;
      case 3:
        Serial.println("Boton C");
	angle_cannon-=2;
	angle_cannon=write_cannon(angle_cannon);
	break;
      case 4:
        Serial.println("Boton D");
	for(int i=0;i<=4;i++){
		angle_turrent--;
		angle_turrent=write_torreta(angle_turrent);
		delay(200/5);
	}
	break;
      default:
        receive_data(inputString,MIN);
    }
}




void receive_data(String inputString, int MIN){
      int angulo=(inputString.substring(0,4).toInt())/10;
      Serial.print("Angulo de entrada es ");
      Serial.println(angulo);
      // The last 3 chars are the velocity
      int pedal=(inputString.substring(4).toInt())/10;
      //if(pedal==9){ pedal=100; }
      Serial.print("Gas es ");
      Serial.println(pedal);
      
      // Write the data to the motors
      write_motors(angulo,pedal,MIN);
}

void write_motors(int angle, int gas, int MIN){
	// Converting to radians and
	// Calculate both X and Y components
	float y=cos((angle*3.1416)/180)*(gas);
	float x=sin((angle*3.1416)/180)*(gas);

  //Serial.println(x);
  //Serial.println(y);
  
  
	// Intermidiate step
	float V=(100-abs(x))*(y/100)+y; // R+L
	float W=(100-abs(y))*(x/100)+x; // R-L
	// Final values for the tracks, map it to be a byte for PWM

  int R=(V+W)/2;
  int L=(V-W)/2;

  Serial.println(R);
  Serial.println(L);
 print_right(map(map_motors(R,MIN),-100,100,-255,255));
print_left(map(map_motors(L,MIN),-100,100,-255,255));
}

// Function to map a value according to the motors
int map_motors(int in, int MIN){
	if(in>0){ return map(in,0,100,MIN,100); }
	if(in==0){ return 0; }
	if(in<0){return map(in,-100,0,-100,-MIN);}
}

// Function to print to the left track
void print_left(int vel){
	if(vel>=0){
    Serial.println("LEFT UP");
		digitalWrite(LEFT1,HIGH);
		digitalWrite(LEFT2,LOW);
	}
	else{
    Serial.println("LEFT DOWN");
		digitalWrite(LEFT1,LOW);
		digitalWrite(LEFT2,HIGH);
	}
  if(abs(vel)==0){ digitalWrite(LEFT,LOW); }
	else{ analogWrite(LEFT,abs(vel)); }
  Serial.print("Left wheel: ");
  Serial.println(vel);
}

// Function to print to the right track
void print_right(int vel){
	if(vel>=0){
    Serial.println("RIGHT UP");
		digitalWrite(RIGHT1,HIGH);
		digitalWrite(RIGHT2,LOW);
	}
	else{
    Serial.println("RIGHT DOWN");
		digitalWrite(RIGHT1,LOW);
		digitalWrite(RIGHT2,HIGH);
	}
  if(abs(vel)==0){ digitalWrite(RIGHT,LOW); }
  else{ analogWrite(RIGHT,abs(vel)); }
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
    if (inChar == '#') {
      stringComplete = true;
    }
  }
}
