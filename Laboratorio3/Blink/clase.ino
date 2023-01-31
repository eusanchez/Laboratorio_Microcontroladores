int val=0;
int ledPin=13;
int inPin=7;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(inPin, INPUT);
  // put your setup code here, to run once:

}

void loop() {
  val = digitalRead(inPin);
  digitalWrite(ledPin, val);
  delay(2000);
  // put your main code here, to run repeatedly:

}
