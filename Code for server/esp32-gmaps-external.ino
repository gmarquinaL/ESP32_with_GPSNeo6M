#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <WiFi.h>
#include <WiFiClient.h>
//#include <WebServer.h>
//#include "mainpage.h"
//#include "jscript.h"
//#include "style.h"
#include <HTTPClient.h>

//SSID y contraseña del WiFi
const char* ssid = "Gimena";
const char* password = "70361391";

//Dirección del servidor web externo
const char* server = "https://esp32grupo.000webhostapp.com/serverfiles/gps.php";

//WebServer server(80); 

TinyGPS gps;
SoftwareSerial ss;

//Para almacenar datos como cadena
String gpsData = "";
char buff[18];
//Datos GPS
float flat, flon;
unsigned long age;

void setup(void) {
  Serial.begin(115200);
  // Inicializar GPS
  ss.begin(9600, SWSERIAL_8N1, 17, 16, false);
  if(!ss){
    Serial.println("Configuración de pin de serie de software no válida, verifique la configuración"); 
    while (1) { // Configuración inválida
      delay (1000);
    }
  }
 
  //Usar ESP32 como estación WiFi
  WiFi.mode(WIFI_STA);
  //Iniciar conexión WiFi
  WiFi.begin(ssid, password);
  Serial.println("");
  //Esperar conexión
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Conectado a ");
  //Imprime el SSID de tu WiFi
  Serial.println(ssid);
  Serial.print("Dirección IP: ");
  //Imprima su dirección IP local (para navegar por la aplicación)
  Serial.println(WiFi.localIP());
}

void loop(void) {
  //Envía datos de GPS cada segundo
  gps.f_get_position(&flat, &flon, &age);
  gpsData = "lat="+floatToString(flat,TinyGPS::GPS_INVALID_F_ANGLE, 10, 14) + "&lng=";
  gpsData += floatToString(flon,TinyGPS::GPS_INVALID_F_ANGLE, 11, 14);
  if(gpsData.indexOf("v") > 0){
      gpsData = "lat=-8.11175033127294&lng=-79.02834479625534";
  }

  Serial.println(gpsData);
  smartdelay(1000);
  
  HTTPClient client;
  
  client.begin(server);
  client.addHeader("Connection","keep-alive");
  client.addHeader("Content-Length",String(gpsData.length()));
  client.addHeader("Content-Type", "text/plain");
  int httpResponseCode = client.POST(gpsData);

  Serial.println(client.getString());
  if(httpResponseCode > 0){
    Serial.println("¡Datos enviados con éxito al servidor!");      
  }else{
    Serial.print("La carga del servidor falló.");
    Serial.println(client.errorToString(httpResponseCode).c_str());
  }
  client.end();
}

//Función para convertir valores flotantes gps a cadena
String floatToString(float val, float invalid, int len, int prec) {
  String out = "";
  if (val == invalid) {
    while (len-- > 1){
      return "inv" ;
    }
  }
  else{
    for (int i = 0; i < 10; i++) {
       dtostrf(val, len, prec, buff);  
       out += buff;
       return out;
    }
  }
}

static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}
