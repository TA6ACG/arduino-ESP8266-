#include <ESP8266WiFi.h>
#include "DHT.h"
#define DHTTYPE DHT22 //eğer DHT11 i kullanıyorsanız burayı ve 8inci satırda ki 11 i 22 yapmanız yeterli 
                      //fakat DHT11 çok hatalı ölçüm yapıyor tavsiye etmiyorum
#define DHTPIN  2
const char* ssid = "XXXXXXXXXXX";//type your ssid Kablosuz Ağ Adı
const char* password = "XXXXXXXXXXXXXXX";//type your password Şifresi
 
DHT dht(DHTPIN, DHTTYPE, 22);
WiFiServer server(80);
 float t=-1.0;float h=-1.0 ;float hic=-1.0;
void setup() {

  delay(10);
 
  
  Serial.begin(9600); // Seri iletişimi başlatıyoruz.
  dht.begin();
  Serial.println("*********************");  
  

 
   
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
   WiFi.softAP("AG_ADI", "Sifresi1234"); //modüle kablosuz ağ olmadan bağlanmak için ağ tanımlıyoruz

  WiFi.begin(ssid, password); //bağlanacağımız ağ bilgilerini alıyor 
    
  while (WiFi.status() != WL_CONNECTED) {
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
  Serial.println("/");
    
}
 void(* resetFunc) (void) = 0;//declare reset function at address 0 //kilitlendiği zaman resetlemek için çağıracağımız fonksiyon
void loop() {
  
  h= dht.readHumidity();
  
  t= dht.readTemperature();
  hic = dht.computeHeatIndex(t, h, false); //ısı endexi
  
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

      if (isnan(h) || isnan(t)) { client.println("<font size=\"50\" color=\"red\">"); client.println("!!! SENSÖR OKUNAMIYOR !!!");   client.println("<a href=\"/reboot\">RESTART</a> <br> <br>");} //sensör hatası 
 client.println("<font size=\"50\" color=\"black\">");
 if(t>=30){client.println("<font size=\"50\" color=\"red\">");}
 if(t<=4){client.println("<font size=\"50\" color=\"blue\">");}
 client.println("<center>");
 client.println("<br><br>");client.println("<br><br>");
  client.print("www.cagriseyfeli.com.tr");
   

  client.println("<br><br>");
  client.println("Sıcaklık : ");
  client.print(t);client.print("'C");
  client.println("<br><br>");
  client.println("Nem: ");
  client.println(h);client.print("%");
  client.println("<br><br>");
  // client.println("Çiğ Oluşma Noktası: ");
//  client.println(DHT11_sensor.dewPoint(), 2);
  client.println("<br><br>");
 // client.println("Sensör Durumu");client.print(chk);
 client.println("<font size=\"3\" color=\"white\">");
 client.print("İp : "); client.println(WiFi.localIP());
 client.print("Isı indexi : "); client.println(hic);
  client.println("</html>");
    if (request.indexOf("/reboot") != -1) {Serial.println("Reboot NOW");resetFunc();} //call reset  daha sonra gelen sayfayı kapatın yoksa sürekli reseti çağırır
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
 
             
}
