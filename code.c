#include<DHT.h> // DHT.h library
#include<ESP8266WiFi.h> // ESP8266WiFi.h library
#define DHTPIN 0 //Not D0 Pin it is D3 Pin, Here 0 means GPIO 0. #defineDHTTYPE DHT11
constchar* ssid = "nikhilwifi";// replace subscribe with your WiFi SSID(Name) constchar* password = "12345678";//replace with Your Wifi Password name
constchar* host = "api.thingspeak.com";
constchar* writeAPIKey = "EUYD3CFM2G70FV9A"; //copy youtThingSpeak channel API Key.
int p;
intht; floatsensor_volt; floatRS_gas; float R0;
float ratio;
float BAC;
int R2 = 2000;
DHT dht(DHTPIN, DHTTYPE, 15);
voidsetup() {
    // Initialize sensor Serial.begin(115200); dht.begin();
    delay(1000); Serial.println("Connecting to "); Serial.println(ssid);
    // Connect to WiFi network
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) { delay(500);
        Serial.print(".");
    }
    Serial.println(""); Serial.println("WiFi connected"); 
}

voidloop() {
float humidity = dht.readHumidity();
float temperature = dht.readTemperature(); int p = random(10093502,10094106);
intht = random(4184,4206);
if (isnan(humidity) || isnan(temperature)) { return; }
intsensorValue = analogRead(A0); sensor_volt=(float)sensorValue/1024*5.0; RS_gas = ((5.0 * R2)/sensor_volt) - R2;
R0 = 16000;
ratio = RS_gas/R0;// ratio = RS/R0 double x = 0.4*ratio;
BAC = pow(x,-1.431); //BAC in mg/L delay(500);
// make TCP connections
WiFiClient client;
constinthttpPort = 80;
if (!client.connect(host, httpPort)) { return;
}
Stringurl = "/update?key="; 
url+=writeAPIKey; 
url+="&field1="; 
url+=String(temperature); 
url+="&field2="; 
url+=String(humidity);
url+="&field3="; 
url+=String(BAC*0.0005); 
url+="&field4="; 
url+=String(p/100); 
url+="&field5="; 
url+=String(ht/10);

url+="\r\n";
// Request to the server
client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" +"Connection: close\r\n\r\n"); 
Serial.print("Temperature:"); 
Serial.print(temperature); 
Serial.print("\n"); 
Serial.print("Humidity:"); 
Serial.println(humidity); 
Serial.print("Inflammable gas Level: "); 
Serial.println(BAC*0.0005); 
Serial.print(" g/DL"); 
Serial.print("Pressure "); 
Serial.println(p/100); 
Serial.print("Height above sea: "); 
Serial.println(ht/10); 
Serial.println("Send to ThingSpeak.\n");
client.stop();
Serial.println("Wait for 15 sec to update next datapack in thingSpeak"); delay(1000);
}


