#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
//
//обьявление всех пинов для удобства 
#define LED1              2
#define LED2              5
#define LED3              12
#define LED4              13
#define LED5              14
#define LED6              15
#define LED7              16
#define LED8              17
#define LED9              18
#define LED10             19
#define LED11             23 
#define LED12             25
#define LED13             26
#define LED14             27
#define LED15             32
#define LED16             33
#define LEDS              LED1,LED2,LED3,LED4,LED5,LED6,LED7,LED8,LED9,LED10,LED11,LED12,LED13,LED14,LED15,LED16
//массив со всем пинами подключенных к светодиодам 
//byte=uint8_t 1 байт	0… 255	Целые числа
uint8_t leds[] = { LEDS };
//таблица включения светодиодов 
uint8_t l2[]={LED7};
uint8_t l3[]={LED13,LED8};
uint8_t l4[]={LED10};
uint8_t l5[]={LED14,LED15};
//определение начальной скорости 
int speed = 4;
int w;
//массивы для random leds
byte myInts[sizeof(leds)];
byte replay[15];//массив для проверки повторов
//обьявление lcd 
LiquidCrystal_I2C lcd(0x27,20,4);
 
// обьявление имени и пароля wifi
const char* ssid     = "ESP32";
const char* password = "12345642";
 
// Установливаем номер порта веб-сервера равным 80(Создает сервер который слушает входящие подключения на указанном порту)
WiFiServer server(80);
 
// Переменная для хранения HTTP-запроса
String header;
 
// начальное название кнопок 
String output2State = "off";
String output3State = "off";
String output4State = "off";
String output5State = "off";
String speedlvl = "one";

void setup() {
  Serial.begin(115200);
  for (uint8_t i = 0; i <  sizeof(leds); i++){
  pinMode(leds[i], OUTPUT);}
  //Вывод информации о wifi на жкд и монитор порта
  lcd.init();//Инициируем работу с LCD дисплеем
  lcd.backlight(); 
  lcd.setCursor(0, 0);
  lcd.print("WIFI Name:");
  lcd.setCursor(10, 0);
  lcd.print(ssid);
  lcd.setCursor(0, 1);
  lcd.print("Password:");
  lcd.setCursor(9, 1);
  lcd.print(password);
  Serial.print("Setting AP (Access Point)…");
  WiFi.softAP(ssid, password);//задаем сеть
  IPAddress IP = WiFi.softAPIP();//обьявление ip
  lcd.setCursor(0, 2);
  lcd.print("IP:");
  lcd.setCursor(3, 2);
  lcd.print(IP);
  Serial.print("AP IP address: ");
  Serial.println(IP);
  lcd.setCursor(11, 3);
  lcd.print("SKN Prod.");
  server.begin();//Сообщает серверу, что нужно начать слушать входящие подключения
}
void loop(){

  WiFiClient client = server.available();   // ждем приходящий клиентов
    //следующие 4 строки отвечате за откытие новой страница(.....4.1/37/two)
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();
  if (client) {                             // если клиент есть 
    Serial.println("New Client.");          // выводим сообщение
    String currentLine = "";                // создаем строку для хранения входящих данных от клиента
    while (client.connected()) {            // пока клиент подключен 
      if (client.available()) {             // если есть байты для чтения с клиента,
        char c = client.read();             // читаем 
        Serial.write(c);                    // выводим сообщение(двоичные данные)
        header += c;
        if (c == '\n') {                    // если байт является символом новой строки
          if (currentLine.length() == 0) {//если текущая строка =0 
            /////////////////////////////////////////////////////////////////////////running light 2.1
            if (header.indexOf("GET /26/on") >= 0) {
              lcd.clear();//очищаем дисплей
              Serial.println("GPIO 26 on");
              lcd.backlight(); 
              lcd.setCursor(0, 0);
              lcd.print("works: running light");//описание режима работы на жкд
              
              for (uint8_t i = 0; client.connected(); i++){
                digitalWrite(leds[i], HIGH);
                delay(50*speed);
                digitalWrite(leds[i], LOW);
                if(i==15){i=-1;}
              }
              lcd.clear();
            } 
            //////////////////////////////////////////////////////////////////////////Blink 3
              else if (header.indexOf("GET /27/on") >= 0) {
              lcd.clear();//очищаем дисплей
              Serial.println("GPIO 27 on");
              lcd.backlight(); 
              lcd.setCursor(0, 0);
              lcd.print("works: blink");//описание режима работы на жкд   
              while (client.connected()){           
              for(int q=0;q<50*speed;q++)//время горения светодиодов чем больше q тем дольше
                {
                for (uint8_t i = 0; i <  sizeof(leds); i++)
                  {
                  digitalWrite(leds[i], HIGH);
                  delayMicroseconds(100); 
                  digitalWrite(leds[i], LOW);
                  delayMicroseconds(1000 - 100);
                  }
                }
                delay(500*speed);
              }  
              lcd.clear();
            } 
            ///////////////////////////////////////////////////////////////////////////Random 4
            else if (header.indexOf("GET /28/on") >= 0){
              lcd.clear();//очищаем дисплей
              Serial.println("GPIO 28 on");
              lcd.backlight(); 
              lcd.setCursor(0, 0);
              lcd.print("works: PWM random");//описание режима работы на жкд 
              int n1=-1;
              while (client.connected()){ 
            for (uint8_t j1 = 0; j1 < sizeof(leds); j1++) 
            {
              if(!client.connected()){break;}
              uint8_t i; 
              n1=n1+1;
              
              i = random(0, sizeof(leds));//генератор случайных чисел без повторов//чтобы бесконечно -1 убираем 
              for (uint8_t z = 0; z < 16; z++)
              {
                if(z<15 && replay[z]==i)
                {
                  i = random(0, sizeof(leds));//нужно снова в начало цикла на проверку
                  z = -1;
                }
              }
              replay[j1]=i;
              //отладка
              Serial.print(j1);
              Serial.print(")");
              Serial.print("i:");
              Serial.print(i);
              Serial.print("   ");
              Serial.print(n1);
              Serial.println();
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("works: PWM random");
              for( w= 0 ; w < sizeof(leds); w++)//цикл заполнения второго массива
              {
                  if(myInts[w]!=0){continue;}   

                  myInts[w]=leds[i];
                  break;
              }
              
                for( int q = 0; q< 25*speed; q++)//цикл времени работы ШИМ
                {
                  for( w = 0; w < sizeof(leds); w++ )//ШИМ
                  {
                    digitalWrite(myInts[w], HIGH);//!back
                    delayMicroseconds(500); 
                    digitalWrite(myInts[w], LOW);//back
                    delayMicroseconds(1000 - 100);

                  }
                }   
              if(n1%14==0&&n1!=0){//28 чтобы все погасло 
              n1=-1;
              break;}
            }
            if(!client.connected()){goto exit;}
              for(int q=0;q<50*speed;q++)//время горения светодиодов чем больше q тем дольше
                {
                for (uint8_t i = 0; i <  sizeof(leds); i++)
                  {
                  digitalWrite(leds[i], HIGH);
                  delayMicroseconds(100);
                  digitalWrite(leds[i], LOW);
                  delayMicroseconds(1000 - 100);
                  }
                }
            exit:
            //очистка массива для следующего цикла
            for(w=0;w < sizeof(leds);w++)//Выключение всех светодиодов
            {

              digitalWrite(myInts[w], LOW);
              
            }
            for( int i = 0; i < sizeof(myInts);  ++i )//очистка массива
            {
              myInts[i] = (char)0;
              replay[i] = (char)0;
            }
            }
            lcd.clear();
            }
            /////////////////////////////////////////////////////////////////////////////////Glow 1
            else if (header.indexOf("GET /25/on") >= 0){
              lcd.clear();//очищаем дисплей
              lcd.backlight(); 
              lcd.setCursor(0, 0);
              lcd.print("works: glow");//описание режима работы на жкд 
              while (client.connected()){            
                for (uint8_t i = 0; i <  sizeof(leds); i++)
                  {
                  digitalWrite(leds[i], HIGH);
                  delayMicroseconds(100); 
                  digitalWrite(leds[i], LOW);
                  delayMicroseconds(1000 - 100);
                  }
              }
            }
            ///////////////////////////////////////////////////////////////////////////////////////Running light 2.2
            else if (header.indexOf("GET /36/on") >= 0){
              lcd.clear();//очищаем дисплей
              lcd.noBacklight(); 
              lcd.setCursor(0, 0);
              lcd.print("works: running light 3X");//описание режима работы на жкд 
                 
                for (uint8_t i = 0; client.connected(); i++){
                if(i==0){digitalWrite(leds[0], HIGH);}
                if(i==1){
                  digitalWrite(leds[0], HIGH);
                  digitalWrite(leds[1], HIGH);}
                if(i==14){
                  digitalWrite(leds[0], HIGH);
                  digitalWrite(leds[14], HIGH);
                  digitalWrite(leds[15], HIGH);}
                if(i==15){
                  digitalWrite(leds[0], HIGH);
                  digitalWrite(leds[1], HIGH);
                  digitalWrite(leds[15], HIGH);}                
                if(i!=0&&i!=1&&i!=14&&i!=15){
                  digitalWrite(leds[i], HIGH);
                  digitalWrite(leds[i+1], HIGH);
                  digitalWrite(leds[i+2], HIGH);
                }
                delay(50*speed);
                for(w=0;w<16;w++)
                {digitalWrite(leds[w], LOW);}
                
                if(i==15){i=-1;}
              }
              lcd.clear();//очищаем дисплей
                 
            }  ///////////////////////////////////////////////////////////////////////////////////////Multiple lights 6
            else if (header.indexOf("GET /34/on") >= 0){
              lcd.clear();//очищаем дисплей
              lcd.noBacklight(); 
              lcd.setCursor(0, 0);
              lcd.print("works:Multiple lights");//описание режима работы на жкд 
              
              for (uint8_t i = 0; client.connected(); i=i+4){
                if(i==16){
                  delay(20*speed);
                  i=1;}
                if(i==17){
                  delay(20*speed);
                  i=2;}
                if(i==18){
                  delay(20*speed);
                  i=3;}
                if(i==19){
                  delay(20*speed);
                  i=0;}
                digitalWrite(leds[i], HIGH);
                delayMicroseconds(100);
                digitalWrite(leds[i], LOW);
                delayMicroseconds(1000 - 100);
              }
              lcd.clear();//очищаем дисплей
              }
              /////////////////////////////////////////////////////////////////////////////////сходящийся и расходящийся 7
              else if (header.indexOf("GET /35/on") >= 0){
              lcd.clear();//очищаем дисплей
              lcd.noBacklight(); 
              lcd.setCursor(0, 0);
              lcd.print("works:converge and diverge");//описание режима работы на жкд 
              
              for (uint8_t i = 0; client.connected(); i++){
                digitalWrite(leds[i], HIGH);
                digitalWrite(leds[15-i], HIGH);
                delay(50*speed);
                for(w=0;w<16;w++)
                {digitalWrite(leds[w], LOW);}
                if(i==15){i=-1;}
              }
              }
              //////////////////////////////////////////////////////////////////////////////////// кнопка 1
            else if (header.indexOf("GET /30/on") >= 0){
              output2State = "on";
              lcd.clear();//очищаем дисплей
              lcd.backlight(); 
              lcd.setCursor(0, 0);
              lcd.print("l2");
              for(int i=0;i<sizeof(l2);i++)
              {
                digitalWrite(l2[i], HIGH);
              }
            } else if (header.indexOf("GET /30/off") >= 0) {
              output2State = "off";
              lcd.clear();
              for(int i=0;i<sizeof(l2);i++)
              {
                digitalWrite(l2[i], LOW);
              }
            }
            ////////////////////////////////////////////////////////////////////////////////////// кнопка 2
            else if (header.indexOf("GET /31/on") >= 0){
              output3State = "on";
              lcd.clear();//очищаем дисплей
              lcd.backlight(); 
              lcd.setCursor(0, 0);
              lcd.print("l3");
              for(int i=0;i<sizeof(l3);i++)
              {
                digitalWrite(l3[i], HIGH);
              }
            } else if (header.indexOf("GET /31/off") >= 0) {
              output3State = "off";
              lcd.clear();
              for(int i=0;i<sizeof(l3);i++)
              {
                digitalWrite(l3[i], LOW);
              }
            }
            ///////////////////////////////////////////////////////////////////////////////////////// кнопка 3
            else if (header.indexOf("GET /32/on") >= 0){
              output4State = "on";
              lcd.clear();//очищаем дисплей
              lcd.backlight(); 
              lcd.setCursor(0, 0);
              lcd.print("l4");
              for(int i=0;i<sizeof(l4);i++)
              {
                digitalWrite(l4[i], HIGH);
              }
            } else if (header.indexOf("GET /32/off") >= 0) {
              output4State = "off";
              lcd.clear();
              for(int i=0;i<sizeof(l4);i++)
              {
                digitalWrite(l4[i], LOW);
              }
            }
            ///////////////////////////////////////////////////////////////////////////////////////кнопка 4
            else if (header.indexOf("GET /33/on") >= 0){
              output5State = "on";
              lcd.clear();//очищаем дисплей
              lcd.backlight(); 
              lcd.setCursor(0, 0);
              lcd.print("l5");
              for(int i=0;i<sizeof(l5);i++)
              {
                digitalWrite(l5[i], HIGH);
              }
            } else if (header.indexOf("GET /33/off") >= 0) {
              output5State = "off";
              lcd.clear();
              for(int i=0;i<sizeof(l5);i++)
              {
                digitalWrite(l5[i], LOW);
              }
            }
            ///////////////////////////////////////////////////////////////////////////        кнопка скорости
            else if (header.indexOf("GET /37/one") >= 0){
              speed = 2;
              speedlvl = "two";
              lcd.clear();
              lcd.backlight(); 
              lcd.setCursor(0, 0);
              lcd.print("speed:");
              lcd.setCursor(6, 0);
              lcd.print(speed);
            }
            else if (header.indexOf("GET /37/two") >= 0){
              speed = 1;
              speedlvl = "three";
              lcd.clear();
              lcd.backlight(); 
              lcd.setCursor(0, 0);
              lcd.print("speed:");
              lcd.setCursor(6, 0);
              lcd.print(speed);
            }
            else if (header.indexOf("GET /37/three") >= 0){
              speed = 4;
              speedlvl = "one";
              lcd.clear();
              lcd.backlight(); 
              lcd.setCursor(0, 0);
              lcd.print("speed:");
              lcd.setCursor(6, 0);
              lcd.print(speed);
            }
            //////////////////////////////////////////////////////////////////////////

            //HTML
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            
            client.println(".button { background-color: #3498db; border: none; color: white; padding: 16px 40px;");//кнопка в момент on
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}");
            //управление скоростью 
            client.println(".button3 {background-color: #008000;}");
            client.println(".button4 {background-color: #ffff00;}");
            client.println(".button5 {background-color: #ff0000;}");

            client.println("</style></head>");
            // Web Page Heading 
            client.println("<body><h1>ESP32 SKN Production</h1>");
            //оформление кнопок
            client.println("<p>Speed - touch to choose lvl</p>");
            if (speedlvl=="one") {
              client.println("<p>Now slow</p>");
              client.println("<p><a href=\"/37/one\"><button class=\"button button4\">switch to average</button></a></p>");
            } else if(speedlvl=="two") {
              client.println("<p>Now average</p>");
              client.println("<p><a href=\"/37/two\"><button class=\"button button5\">switch to fast</button></a></p>");
            } else if(speedlvl=="three"){
              client.println("<p>Now fast</p>");
              client.println("<p><a href=\"/37/three\"><button class=\"button button3\">switch to slow</button></a></p>");
            }

            client.println("<p>Glow - touch to on</p>");
            client.println("<p><a href=\"/25/on\"><button class=\"button\">ON</button></a></p>");//используется   

            client.println("<p>Running light - touch to on</p>");
            client.println("<p><a href=\"/26/on\"><button class=\"button button\">ON</button></a></p>");//используется 

            client.println("<p><a href=\"/36/on\"><button class=\"button button\">ON</button></a></p>");//используется 

            client.println("<p>BLINK - touch to on</p>");
            client.println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>");//используется 

            client.println("<p>Random - touch to on</p>");
            client.println("<p><a href=\"/28/on\"><button class=\"button\">ON</button></a></p>");//используется 

            client.println("<p>Multiple lights - touch to on</p>");
            client.println("<p><a href=\"/34/on\"><button class=\"button\">ON</button></a></p>"); //используется 

            client.println("<p>converge and diverge - touch to on</p>");
            client.println("<p><a href=\"/35/on\"><button class=\"button\">ON</button></a></p>");

            client.println("<p>ON/OFF LEDS - touch to on</p>");
            if (output2State=="off") {
              client.println("<p><a href=\"/30/on\"><button class=\"button\">ON(7)</button></a></p>");//30 31 32 33
            } else {
              client.println("<p><a href=\"/30/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
            
            if (output3State=="off") {
              client.println("<p><a href=\"/31/on\"><button class=\"button\">ON(13,8)</button></a></p>");//30 31 32 33
            } else {
              client.println("<p><a href=\"/31/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
            
            if (output4State=="off") {
              client.println("<p><a href=\"/32/on\"><button class=\"button\">ON(10)</button></a></p>");//30 31 32 33
            } else {
              client.println("<p><a href=\"/32/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
            
            if (output5State=="off") {
              client.println("<p><a href=\"/33/on\"><button class=\"button\">ON(14,15)</button></a></p>");//30 31 32 33
            } else {
              client.println("<p><a href=\"/33/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
            client.println("</body></html>");
            
            // HTTP-ответ заканчивается еще одной пустой строкой
            client.println();

            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}