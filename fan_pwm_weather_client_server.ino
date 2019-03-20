//https://handyman.dulare.com/client-server-communication-using-esp8266/
//https://codebender.cc/sketch:177208#PWM_Fan.ino
//http://forum.arduino.cc/index.php?topic=18742.15
//http://www.beefrankly.org/blog/2011/12/21/read-out-4-pin-cpu-fan-speed/
//https://www.picmicrolab.com/arduino-pwm-fan-controller/
//http://www.theorycircuit.com/reading-dc-fan-rpm-arduino/
//https://www.avrfreaks.net/forum/control-pwm-pc-12v-fan-speed

#include <ESP8266WiFi.h>
#include <DHT.h>

const char* ssid = ""; // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = ""; // The password of the Wi-Fi network

const char* getHost   = ""; // The data receiver host name (not URL)
const int httpGetPort = 80; // The data receiver host port
String getReceiverURL = ""; // The data receiver script
int mysql_channel     = 15;
int interruptCounter  = 0; // counter of interrupt executions

int count_loop = 0;
float t_avg    = 0; 
float h_avg    = 0; 
int period     = 300*60; //20MIN 300*60;
int t1         = 0;
int t2         = 0;
int send_data  = 0;

#define DHTPIN D7 // what pin we're connected to
DHT dht(DHTPIN,DHT11,15);
int fanPulse   = D3; 
int fanControl = D2;
unsigned long pulseDuration;

WiFiServer server(80); // set WiFiServer on port 80

void setup() {
  Serial.begin(115200); // Start the Serial communication to send messages to the computer
  dht.begin();
  pinMode(fanPulse, INPUT);
  digitalWrite(fanPulse,HIGH);
  pinMode(fanControl, OUTPUT);
  analogWrite(fanControl,500);
  
	delay(10);
	Serial.println('\n');

	WiFi.hostname("ESPboard-counter");
	WiFi.begin(ssid, password); // Connect to the network
	Serial.print("Connecting to ");
	Serial.print(ssid); Serial.println(" ...");

	int i = 0;
	while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
	delay(1000);
	Serial.print(++i); Serial.print(' ');
	}

	Serial.println('\n');
	Serial.println("Connection established!");
	Serial.print("IP address:\t");
	Serial.println(WiFi.localIP()); // Send the IP address of the ESP8266 to the computer

	// start HTTP server
	server.begin();
	Serial.printf("Web server started, open %s in a web browser\n", WiFi.localIP().toString().c_str());
}



// send data to the receiver host
void postData(float t, float h) {
	WiFiClient clientGet;
  double frequency = readPulse();
	// We now create and add parameters:
	String src = "ESP";
	int vint = interruptCounter;
      String postStr = "?esp";
      postStr +="&field1=";
      postStr += String(t);
      postStr +="&field2=";
      postStr += String(h);
      postStr +="&field_measure1=1";
      postStr +="&field_measure2=2";
      postStr +="&field_measure4=4";
      postStr +="&field_measure5=5";
      postStr +="&channel=";
      postStr += String(mysql_channel);
      postStr +="&ip=";
      postStr += String(WiFi.localIP().toString().c_str());
      postStr +="&field4=";
      postStr += String(frequency/4);
      postStr +="&field5=";
      postStr += String(frequency/4*60);


	String getReceiverURLtemp = getReceiverURL + postStr;

	Serial.println("-------------------------------");
	Serial.print(">>> Connecting to host: ");
	Serial.println(getHost);
	Serial.println(getReceiverURLtemp);
	if (!clientGet.connect(getHost, httpGetPort)) {
	Serial.print("Connection failed: ");
	Serial.print(getHost);
	} else {
   
		clientGet.print("GET " + getReceiverURLtemp + " HTTP/1.1\r\n" +
						   "Host: " + getHost + "\r\n" + 
						   "Connection: close\r\n\r\n");
		unsigned long timeoutP = millis();
		while (clientGet.available() == 0) {
  		if (millis() - timeoutP > 10000) {
  			Serial.print(">>> Client Timeout: ");
  			Serial.println(getHost);
  			clientGet.stop();
  			return;
  		}
		}

		//just checks the 1st line of the server response. Could be expanded if needed.
		while(clientGet.available()){
			String retLine = clientGet.readStringUntil('\r');
			Serial.print(">>> Host returned: ");
			Serial.println(retLine);
			// reset counter if successully connected
			if (retLine == "HTTP/1.1 200 OK") {
			Serial.println(">>> Communication successful");
			interruptCounter -= vint;
			Serial.print(">>> Changed interrupt counter to: ");
			Serial.println(interruptCounter);
			} else {
			Serial.println(">>> Communication failed!!!");
			}
			break;
		}
	} //end client connection if else

	Serial.print(">>> Closing host: ");
	Serial.println(getHost);
  
	clientGet.stop();
}

// prepare a web page to be send to a client (web browser)
String prepareHtmlPage()
{
	String htmlPage =
	String("HTTP/1.1 200 OK\r\n") +
	"Content-Type: text/html\r\n" +
	"Connection: close\r\n" + // the connection will be closed after completion of the response
	"Refresh: 30\r\n" + // refresh the page automatically every 5 sec
	"\r\n" +
	"<!DOCTYPE HTML>" +
	"<html><body>" +
	"<p>Interrupt counter: " + String(interruptCounter) + "</p>" +

	"<p>Current time: " + String(millis()) + "</p>" +

		"<br><br>"+
		"Click <a href=\"/fan=high\">here</a> fan high<br>"+
		"<br><br>"+
		"Click <a href=\"/fan=mid\">here</a> fan mid<br>"+
		"<br><br>"+
		"Click <a href=\"/fan=normal\">here</a> fan normal<br>"+
		"<br><br>"+
		"Click <a href=\"/fan=low\">here</a> fan lowh<br>"+
		"<br><br>"+
		"Click <a href=\"/send_data=1\">here</a> send data<br>"+
	"</body></html>" +
	"\r\n";
	return htmlPage;
}

// handle HTTP request to this board
void handle_request(WiFiClient client) {
	
	Serial.println("\n[Client connected]");
	while (client.connected())
	{
	// read line by line what the client (web browser) is requesting
		if (client.available())
		{
			String line = client.readStringUntil('\r');
			//Serial.print(line);
      
          if (line.indexOf("/fan=high") != -1) {
            speedFan(1200);
            Serial.println("HIGH");
            client.println(prepareHtmlPage());
            break;
          } 
         
         if (line.indexOf("/fan=mid") != -1) {
          speedFan(700);
          Serial.println("MID");
          client.println(prepareHtmlPage());
            break;
         } 
      
         if (line.indexOf("/fan=normal") != -1) {
          speedFan(500);
          Serial.println("Normal");
          client.println(prepareHtmlPage());
            break;
         } 
          
         if (line.indexOf("/fan=low") != -1) {
          speedFan(200);
          Serial.println("LOW");
          client.println(prepareHtmlPage());
            break;
         } 
        
        if (line.indexOf("/send_data=1") != -1) {
          send_data = 1;
          client.println(prepareHtmlPage());
            break;
          
        }
			
			// wait for end of client's request, that is marked with an empty line
			if (line.length() == 1 && line[0] == '\n')
			{
				client.println(prepareHtmlPage());
				break;
			}
		}
	}
	delay(100); // give the web browser time to receive the data

	// close the connection:
	client.stop();
	Serial.println("[Client disonnected]");
}

void speedFan(int speed_fan){
  Serial.println("speed fan:");
  Serial.print(speed_fan);
   readPulse();
  analogWrite(fanControl,speed_fan);
}


int readPulse() {
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
  return frequency;
}

// main loop of the board
void loop() {
  
  unsigned long current_time = millis();
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

   Serial.println(count_loop);
   Serial.println("");
   Serial.print("Temperature: ");
   Serial.print(t);
   Serial.print(" degrees Celcius Humidity: ");
   Serial.print(h);
    
    count_loop++;
    t_avg = t_avg+t;
    h_avg = h_avg+h;
    if(count_loop == period || send_data == 1){
      t_avg = t_avg/period;
      h_avg = h_avg/period;
      
      postData(t_avg,h_avg);
      count_loop = 0;
      t_avg = 0;
      h_avg = 0;
      send_data = 0;
    }



// wait for a client (web browser) to connect
	WiFiClient client = server.available();
	if (client){
		handle_request(client);
	}

//delay(20000);
delay(100);
}