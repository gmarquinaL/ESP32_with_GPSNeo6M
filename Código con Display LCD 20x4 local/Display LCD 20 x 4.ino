/* Conexion pines LCD
     ESP32           I2C         
        D21          SDA
        D22          SCL
  */

  #include <SoftwareSerial.h>  //incluimos SoftwareSerial
  #include <TinyGPS.h>         //incluimos TinyGPS  
  #include <LiquidCrystal_I2C.h>

  //Crear el objeto lcd  dirección  0x3F y 16 columnas x 2 filas
   LiquidCrystal_I2C lcd(0x27,20,4);  // A4 (SDA); A5(SCL)
  
  TinyGPS gps;  //Declaramos el objeto GPS
  SoftwareSerial serialgps(17,16);      //Declaramos el pin 17 (Tx del GPS) y 16 (Rx del GPS)
   
  //Declaramos la variables para la obtención de datos
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned long chars;
  unsigned short sentences, failed_checksum;

  void setup()
  {

  lcd.init();
  lcd.backlight();
  lcd.clear();

   Serial.begin(115200);  //Iniciamos el puerto serie
   serialgps.begin(9600); //Iniciamos el puerto serie del gps




   //Imprimimos en el monitor serial:
   Serial.println("");
   Serial.println("GPS GY-GPS6MV2 Leantec");
   Serial.println(" ---Buscando senal--- ");
   
   lcd.print("   Buscando senal   "); 
   

 
  }
  
 
  void loop()
  {
    
   while(serialgps.available()) 
   {
    int c = serialgps.read(); 
    if(gps.encode(c)) 
    
    {
     
     //lcd.clear();
     float latitude, longitude;
     gps.f_get_position(&latitude, &longitude);
     Serial.print("Latitud/Longitud: ");
     lcd.setCursor(0,0);
     lcd.print("Latitud: ");
     lcd.print(latitude,4); 
     Serial.print(latitude,4); 

     lcd.setCursor(0,1); 
     lcd.print("Longitud: ");
      
     
     Serial.print(","); 
      
     Serial.println(longitude,4);
     lcd.print(longitude,4);

     lcd.setCursor(0,2);
     gps.crack_datetime(&year,&month,&day,&hour,&minute,&second,&hundredths);
     Serial.print("Fecha: "); Serial.print(day, DEC); Serial.print("-"); 
     lcd.print("Fecha: "); lcd.print(day, DEC); lcd.print("-");
     Serial.print(month, DEC); Serial.print("-"); Serial.print(year);
     lcd.print(month, DEC); lcd.print("-"); lcd.print(year);




     lcd.setCursor(0,3);
     gps.crack_datetime(&year,&month,&day,&hour,&minute,&second,&hundredths);
     Serial.print("Hora: "); Serial.print(hour, DEC); Serial.print(":"); 
     lcd.print("Hora: "); lcd.print(hour, DEC); lcd.print(":");  
     Serial.print(minute, DEC); Serial.print(":"); Serial.print(second, DEC); 
     lcd.print(minute, DEC); lcd.print(":"); lcd.print(second, DEC); 
     Serial.print("."); Serial.println(hundredths, DEC);
     lcd.print("."); lcd.print(hundredths, DEC);


     Serial.print("Altitud (metros): "); Serial.println(gps.f_altitude()); 


     Serial.print("Rumbo (grados): "); Serial.println(gps.f_course()); 


     Serial.print("Velocidad(kmph): "); Serial.println(gps.f_speed_kmph());
     
    // lcd.setCursor(0,3);
     Serial.print("Satelites: "); Serial.println(gps.satellites());
     //lcd.print("Satelites: "); lcd.println(gps.satellites());
     
     Serial.println();


     gps.stats(&chars, &sentences, &failed_checksum);
    }
   }
  }