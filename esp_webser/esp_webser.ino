/*********
  Руи Сантос
  Более подробно о проекте на: http://randomnerdtutorials.com  
*********/

#include <WiFi.h>
#include <Servo.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;

Servo myservo;  // создаем экземпляр класса «Servo»,

// GPIO-контакт, к которому подключен сервопривод:
static const int servoPin = 13;

// вставьте здесь учетные данные своей сети:
const char* ssid     = "RT-GPON-0098";
const char* password = "4P6ETT45";

// создаем веб-сервер на порте «80»:
WiFiServer server(80);

// переменная для хранения HTTP-запроса:
String header;

// несколько переменных для расшифровки значения в HTTP-запросе GET:
String valueString = String(5);
int pos1 = 0;
int pos2 = 0;

void setup() {
  Serial.begin(115200);
  myservo.attach(servoPin);  // привязываем сервопривод,
  Serial.print("Connecting to ");  //  "Подключаемся к "
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
  server.begin();
  timeClient.begin();
  timeClient.setTimeOffset(25200);
  
  
}

void loop(){
  // начинаем прослушивать входящих клиентов:
  WiFiClient client = server.available();

  timeClient.update();
  formattedDate = timeClient.getFormattedTime();
  int splitT = formattedDate.indexOf("T");
  timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
  for (int i=10;i<=24;i++){
        if (timeStamp==(String(i)+":00:0")||timeStamp==("09:00:0") ||timeStamp==("00:12:0")||timeStamp==("00:13:0")||timeStamp==("00:14:0")||timeStamp==("00:15:0")||timeStamp==("00:16:0")||timeStamp==("00:17:0"))
          { 
            Serial.println(timeStamp);
            myservo.write(180);
            delay(2000);  
            myservo.write(0); 
          } 
          i+=2; 
    }
  if (client) {
    String currentLine = "";        // создаем строку для хранения
    while (client.connected()) {    // цикл while() будет работать
      if (client.available()) {     // если у клиента есть данные,
        char c = client.read();     // считываем байт, а затем    
        Serial.write(c);            // печатаем его в мониторе порта
        header += c;
        if (c == '\n') {            // если этим байтом является
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
                       //  "Соединение: отключено"
            client.println();
             // Extract date
              int splitT = formattedDate.indexOf("T");
              timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
              client.println("HOUR: ");
              client.println(timeStamp);
            
            //GET /?value=180& HTTP/1.1
            if(header.indexOf("GET /?value=")>=0) {
              pos1 = header.indexOf('=');
              pos2 = header.indexOf('&');
              valueString = header.substring(pos1+1, pos2);
              if (valueString=="test")
              {
                  for (int i=10;i<=24;i++)
                  {
                    client.println(timeStamp);
                    client.println(String(i)+":00:0");
                    client.println(String( timeStamp==(String(i)+":00:0") ));
                    
                    myservo.write(180);
                    delay(2000);  
                    myservo.write(0); 
                    delay(2000);  
                    myservo.write(180);
                    delay(2000);  
                    myservo.write(0); 
                    delay(2000);  
                    myservo.write(180);
                    delay(2000);  
                    myservo.write(0);
                    i+=2; 
                }
               
              }
              else {
              // вращаем ось сервомотора:
              myservo.write(valueString.toInt());
              }
            }         
            // конец HTTP-ответа задается 
            // с помощью дополнительной пустой строки:
            client.println();
            // выходим из цикла while(): 
            break;
          } 
          else
          {
            currentLine = "";
          }
        } else if (c != '\r') {  // если получили любые данные,
                                 // кроме символа возврата каретки,
          currentLine += c;      // добавляем эти данные 
                                 // в конец строки «currentLine»
        }
      }
    }
    // очищаем переменную «header»:
    header = "";
    // отключаем соединение:
    client.stop();
    Serial.println("Client disconnected.");
               //  "Клиент отключился."
    Serial.println("");
  }
}
