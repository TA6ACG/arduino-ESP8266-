#include <Wire.h>       //I2C library
#include <RtcDS3231.h>  //RTC library
 
//RtcDS3231 rtcObject;              //Uncomment for version 1.0.1 of the rtc library
RtcDS3231<TwoWire> rtcObject(Wire); //Uncomment for version 2.0.0 of the rtc library
 
void setup() {
 Serial.println("BASLADI");
  Serial.begin(115200);  //Starts serial connection
  rtcObject.Begin();     //Starts I2C
 
 //// RtcDateTime currentTime = RtcDateTime(19, 11, 22, 00, 16, 0); //tarihi yil ay gün saat dakika saniye olarak set etme
 //// rtcObject.SetDateTime(currentTime); //anlik saat nesnesini set et
 
}
 
void loop() {
 
  RtcDateTime currentTime = rtcObject.GetDateTime();    // RTC nesnesinden zaman bilgisini alıyoruz
 
  char str[20];   //karakter dizisini tanımlıyoruz
 
  sprintf(str, "%d/%d/%d %d:%d:%d",     //%d karakter dizilimini ve istediğimiz ayraçları tanımlıyoruz
          currentTime.Year(),   //yıl verisini alıyoruz
          currentTime.Month(),  //ay verisini alıyoruz
          currentTime.Day(),    //gün verisini alıyoruz
          currentTime.Hour(),   //saat verisini alıyoruz
          currentTime.Minute(), //dakika verisini alıyoruz
          currentTime.Second()  //saniye verisini alıyoruz
         );
 
  Serial.println(str); //seri port ekranına yazdırıyoruz
 
  delay(20000); //hangi sıklıkla yenilenmesini istediğimizi belirliyoruz (20sn)
 
}
