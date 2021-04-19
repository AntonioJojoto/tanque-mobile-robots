
//Pins for the motor controller, LM298N
#define LEFT1 12
#define LEFT2 11
#define LEFT 9

#define RIGHT1 10
#define RIGHT2 8
#define RIGHT 7

void setup(){
	// Declare all the tank tracks as outputs
	for(int i=7;i<=12;i++){ pinMode(i,OUTPUT); }

	Serial.begin(9600);
}

void loop(){

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
	analogWrite(abs(vel));
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
	analogWrite(abs(vel));
}
