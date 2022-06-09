#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
// Update these with values suitable for your network.
#define ONE_WIRE_BUS 4

// Setup oneWire
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature DS18B20(&oneWire);
char temperatureCString[7];
char temperatureFString[7];



const char* ssid = "town";
const char* password = "hafide212121";
const char* mqtt_server = "test.mosquitto.org"; // MQTT Broker
int mqtt_port = 1883;
 
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server,mqtt_port);
  //client.setCallback(callback);
  DS18B20.begin();
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    
    if (client.connect("Client0001")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

//String(Thermister()).toCharArray(msg,50);
    Serial.print("Publish message from Ali Benzerbadj: ");
    
    //client.publish("capteur/temp", msg);
  //  client.publish("M2122/capteur", msg);
  getTemperature();
  Serial.println(temperatureCString);
  client.publish("M2122/capteur",temperatureCString);
    delay(5000);
  }

void getTemperature() {
  float tempC;
  float tempF;
  do {
    DS18B20.requestTemperatures(); 
    tempC = DS18B20.getTempCByIndex(0);
    dtostrf(tempC, 2, 0, temperatureCString);
    tempF = DS18B20.getTempFByIndex(0);
    dtostrf(tempF, 3, 2, temperatureFString);
    Serial.println(temperatureCString);
    delay(100);
  } while (tempC == 85.0 || tempC == (-127.0));
}

/*double Thermister() {  //Function to perform the fancy math of the Steinhart-Hart equation
 double Temp;
 int val;                //Create an integer variable
 double temp;            //Variable to hold a temperature value
 val=analogRead(2);      //Read the analog port 0 and store the value in val
                         //Runs the fancy math on the raw analog value 
 Temp = log(((10240000/val) - 10000));
 //Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp )
 Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
 Temp = Temp - 273.15;              // Convert Kelvin to Celsius
 //Temp = (Temp * 9.0)/ 5.0 + 32.0; // Celsius to Fahrenheit - comment out this line if you need Celsius
 return Temp;
}
 */

////////////////////////////////////////////

//http://arduino.esp8266.com/stable/package_esp8266com_index.json 

////////////////////////////////////////////
