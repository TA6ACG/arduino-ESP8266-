#include <ESP8266WiFi.h>
#include <OneWire.h>
 #include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <dht11.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
int DHT11_pin=16; // DHT11_pin olarak Dijital D2'yi belirliyoruz.
dht11 DHT11_sensor; // DHT11_sensor adında bir DHT11 nesnesi oluşturduk.
const char* ssid = "XXXXXXXX";//type your ssid - wifi ağ adınız
const char* password = "XXXXXXXXXXXXx";//type your password - şifreniz
 
int DS18S20_1_Pin = 2;//D9
 
int DS18S20_2_Pin = 0;//D8ic
// Sıcaklık Sensörleri Giriş-Çıkışları
OneWire ds1(DS18S20_1_Pin); // 1. sensör 2. Dijital pinde.
OneWire ds2(DS18S20_2_Pin); // 2. sensör 3. Dijital pinde
WiFiServer server(80);
 String ipi="no Wifi";
void setup() {

  delay(10);
 
   lcd.begin();
  Serial.begin(9600); // Seri iletişimi başlatıyoruz.
  Serial.println("*********************");

 
  lcd.backlight();
 
   
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
   WiFi.softAP("Termometre", "passwordSifre"); //remote control Access Point - wifi ağı yoksa burdan baglanirsiniz
  WiFi.begin(ssid, password);
   
  while (WiFi.status() != WL_CONNECTED) {

    lcd.setCursor(0, 0);
  // print from 0 to 9:

   lcd.print("SICAKLIK: ");
   lcd.print(getTemp(ds2));
   lcd.print("C");
  
  // set the cursor to (16,1):
  lcd.setCursor(0,1);
  lcd.print("DISARI:");
  lcd.print(getTemp(ds1));
  lcd.print("'C");
 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
   
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  ipi=WiFi.localIP().toString();
  Serial.println("/");
  
}
 
void loop() {
  float temperature1 = getTemp(ds1); // 1. sensörden gelen sıcaklık değeri
  float temperature2 = getTemp(ds2); // 2. sensörden gelen sıcaklık değeri ic
  int chk = DHT11_sensor.read(DHT11_pin);
    float h = DHT11_sensor.humidity;
    float t = DHT11_sensor.temperature;

    Serial.print("Nem Orani (%): ");
    Serial.println((float)DHT11_sensor.humidity, 2);
    Serial.print("Sicaklik (Celcius): ");
    Serial.println((float)DHT11_sensor.temperature, 2);
    Serial.print("Sicaklik (Kelvin): ");
    Serial.println(DHT11_sensor.kelvin(), 2);
    Serial.print("Sicaklik (Fahrenheit): ");
    Serial.println(DHT11_sensor.fahrenheit(), 2);
    Serial.print("Cig Olusma Noktasi: ");
    Serial.println(DHT11_sensor.dewPoint(), 2);
    Serial.println("------------------");
     lcd.clear();
      lcd.setCursor(0, 0);
  // print from 0 to 9:

   lcd.print("IC:  ");
   lcd.print(temperature2);
   lcd.print("C");
  
  // set the cursor to (16,1):
  lcd.setCursor(0,1);
  lcd.print("NEM: ");
  lcd.print(h);
  lcd.print("%");
 
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DIS: ");
   lcd.print(temperature1);
   lcd.print("C");
   
    lcd.setCursor(0,1);
  lcd.print("IP : ");
  

  lcd.print(ipi.substring(8)); //ekranda fazla yer kaplamasın diye 192.168. kısmını siliyorum tamamı için 8 i 0 yapın
                                //Please change 8 to 0 for all ip address in LCD display
 
  delay(2000);
    // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
   
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
   
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
   
     
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
   client.println("<meta charset=\"utf-8\">"); 
    client.println("<meta http-equiv=\"refresh\" content=\"10;url=index.html\">"); //10sn de bir refresh
  client.println("<font size=\"50\" color=\"black\">");
  client.println("<br><br>");
  client.println("<center>");
  client.print("www.cagriseyfeli.com.tr");
  client.println("<br><br>");
 if(temperature2>=30){client.println("<font size=\"50\" color=\"red\">");}
 if(temperature2<=4){client.println("<font size=\"50\" color=\"blue\">");}
  client.println("Sıcaklık iç : ");
  client.println("<font size=\"50\" color=\"black\">");
  client.print(temperature2);client.print("'C");
   if(temperature1>=30){client.println("<font size=\"50\" color=\"red\">");}
 if(temperature1<=5){client.println("<font size=\"50\" color=\"blue\">");}
  
  client.println("<br><br>");
 
  client.println("Sıcaklık dış: ");
  client.print(temperature1);client.print("'C");
  client.println("<br><br>");
  client.println("<font size=\"50\" color=\"black\">");
  client.println("Nem: ");
  client.println(h);client.print("%");
  client.println("<br><br>");
   client.println("Çiğ Oluşma Noktası: ");
  client.println(DHT11_sensor.dewPoint(), 2);
  client.println("<br><br>");
  client.println("<font size=\"5\" color=\"white\">");
  client.println("dht11 sıcaklık:");client.print(t);
  client.println(" ip: ");client.print(ipi);
 
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
 
             
}
float getTemp(OneWire sensor){ //sensorlerden gelen veriyi ceviriyoruz degistirmeyin
//returns the temperature from one DS18S20 in DEG Celsius

byte data[12];
byte addr[8];

if ( !sensor.search(addr)) {
   //no more sensors on chain, reset search
   sensor.reset_search();
   return -1000;
}

if ( OneWire::crc8( addr, 7) != addr[7]) {
   Serial.println("CRC is not valid!");
   return -1000;
}

if ( addr[0] != 0x10 && addr[0] != 0x28) {
   Serial.print("Device is not recognized");
   return -1000;
}

sensor.reset();
sensor.select(addr);
sensor.write(0x44,1); // start conversion, with parasite power on at the end

byte present = sensor.reset();
sensor.select(addr);  
sensor.write(0xBE); // Read Scratchpad

for (int i = 0; i < 9; i++) { // we need 9 bytes
  data[i] = sensor.read();
}

sensor.reset_search();

byte MSB = data[1];
byte LSB = data[0];

float tempRead = ((MSB << 8) | LSB); //using two's compliment
float TemperatureSum = tempRead / 16;

return TemperatureSum;

}
