/*********
  Руи Сантос
  Более подробно о проекте на: http://randomnerdtutorials.com  
*********/

#include <WiFi.h>
#include <Servo.h>

Servo myservo;  // создаем экземпляр класса «Servo»,
                // чтобы с его помощью управлять сервоприводом;
                // большинство плат позволяют
                // создать 12 объектов класса «Servo»

// GPIO-контакт, к которому подключен сервопривод:
static const int servoPin = 13;

// вставьте здесь учетные данные своей сети:
const char* ssid     = "";
const char* password = "";

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
                             // подключенный к контакту «servoPin»,
                             // к объекту «myservo»

  // подключаемся к WiFi при помощи заданных выше SSID и пароля: 
  Serial.print("Connecting to ");  //  "Подключаемся к "
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // печатаем локальный IP-адрес и запускаем веб-сервер:
  Serial.println("");
  Serial.println("WiFi connected.");  //  "WiFi подключен."
  Serial.println("IP address: ");     //  "IP-адрес: "
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  // начинаем прослушивать входящих клиентов:
  WiFiClient client = server.available();

  if (client) {                     // если подключился новый клиент,     
    Serial.println("New Client.");  // печатаем сообщение
                                    // «Новый клиент.»
                                    // в мониторе порта;
    String currentLine = "";        // создаем строку для хранения
                                    // входящих данных от клиента;
    while (client.connected()) {    // цикл while() будет работать
                                    // все то время, пока клиент
                                    // будет подключен к серверу;
      if (client.available()) {     // если у клиента есть данные,
                                    // которые можно прочесть, 
        char c = client.read();     // считываем байт, а затем    
        Serial.write(c);            // печатаем его в мониторе порта
        header += c;
        if (c == '\n') {            // если этим байтом является
                                    // символ новой строки
          // если получили два символа новой строки подряд,
          // то это значит, что текущая строчка пуста;
          // это конец HTTP-запроса клиента,
          // а значит – пора отправлять ответ:
          if (currentLine.length() == 0) {
            // HTTP-заголовки всегда начинаются
            // с кода ответа (например, «HTTP/1.1 200 OK»)
            // и информации о типе контента
            // (чтобы клиент понимал, что получает);
            // в конце пишем пустую строчку:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
                       //  "Соединение: отключено"
            client.println();

            // показываем веб-страницу:
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial; margin-left:auto; margin-right:auto;}");
            client.println(".slider { width: 300px; }</style>");
            client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>");
                     
            // веб-страница:
            client.println("</head><body><h1>ESP32 with Servo</h1>");
                                        //  "Управление сервомотором
                                        //   с помощью платы ESP32"
            client.println("<p>Position: <span id=\"servoPos\"></span></p>");          
            client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider\" id=\"servoSlider\" onchange=\"servo(this.value)\" value=\""+valueString+"\"/>");
            
            client.println("<script>var slider = document.getElementById(\"servoSlider\");");
            client.println("var servoP = document.getElementById(\"servoPos\"); servoP.innerHTML = slider.value;");
            client.println("slider.oninput = function() { slider.value = this.value; servoP.innerHTML = this.value; }");
            client.println("$.ajaxSetup({timeout:1000}); function servo(pos) { ");
            client.println("$.get(\"/?value=\" + pos + \"&\"); {Connection: close};}</script>");
           
            client.println("</body></html>");     
            
            //GET /?value=180& HTTP/1.1
            if(header.indexOf("GET /?value=")>=0) {
              pos1 = header.indexOf('=');
              pos2 = header.indexOf('&');
              valueString = header.substring(pos1+1, pos2);
              
              // вращаем ось сервомотора:
              myservo.write(valueString.toInt());
              Serial.println(valueString); 
            }         
            // конец HTTP-ответа задается 
            // с помощью дополнительной пустой строки:
            client.println();
            // выходим из цикла while(): 
            break;
          } else { // если получили символ новой строки,
                   // очищаем текущую строку «currentLine»:
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
