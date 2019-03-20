//http://www.beefrankly.org/blog/2011/12/21/read-out-4-pin-cpu-fan-speed/
int fanPulse   = D3;
int fanControl = D2;
unsigned long pulseDuration;
int p = 0;
void setup()
{
Serial.begin(9600);
pinMode(fanPulse, INPUT);
digitalWrite(fanPulse,HIGH);
pinMode(fanControl, OUTPUT);
}

void readPulse() {
pulseDuration = pulseIn(fanPulse, LOW);
double frequency = 1000000/pulseDuration;

Serial.print("pulse duration:");
Serial.println(pulseDuration);

Serial.print("time for full rev. (microsec.):");
Serial.println(pulseDuration*2);
Serial.print("freq. (Hz):");
Serial.println(frequency/2);
Serial.print("RPM:");
Serial.println(frequency/2*60);

}

void loop()
{
analogWrite(fanControl,1200);
delay(5000);
readPulse();


analogWrite(fanControl,200);
delay(5000);
readPulse();


}
