// --------------------------------- Импорт сторонних библиотек
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// --------------------------------- Задание константных значений
// Пин кнопки ПУСК
const int buttonStartPin = 5;

// Пин кнопки СТОП
const int buttonStopPin = 4;

// Пин реле
const int relayPin = 3;

// Сообщение: "Машина Атвуда"
const String welcomeMessage = " Atwood machine ";

// Сообщение: "Нажмите СТОП"
const String pressStopMessage = "   Press STOP   ";

// Сообщение: "Результат"
const String resultMessage = "     Result     ";

// Время ожидания после нажатия кнопки, чтобы убрать дребезг контактов
const long waitTimeAfterPushButton = 300;

// --------------------------------- Задание переменных значений

// Изначальное состояние реле
bool defaultRelayState = true;

// Состояние реле.
int relayState = defaultRelayState;

// Состояние реле digitalRead(relayPin) при котором включается реле.
int relayOnDigitalRead = LOW;


// --------------------------------- Настройка
void setup() {
  pinMode(buttonStartPin, INPUT);
  pinMode(buttonStopPin, INPUT);

  pinMode(relayPin, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(1,0);
  // Печатает Вступительно сообщение в одну строку
  lcd.print(welcomeMessage);

  Serial.begin(9600);
  Serial.println(welcomeMessage);
}


// --------------------------------- Главный цикл
void loop() {
   
  // Зажим магнита автоматически после включения
  setRelayState(defaultRelayState);

  Serial.println("before while");

  while(true){
    // Проверяем зажата ли кнопка ПУСК. Зажатие == земля.
    if(digitalRead(buttonStartPin)  == LOW){
      Serial.println("buttonStartPin НАЖАТА");
      stopwatch();

      // Печатает Вступительно сообщение в одну строку
      printMessageInOneLine(welcomeMessage);
    }
  }
}

// --------------------------------- Метод секундомера
void stopwatch(){
  Serial.println("stopwatch start method");
  // Время начала работы секундомера
  long initTime = millis();

  // Отпустить магнита
  setRelayState(false);

  // Нажмите СТОП
  printMessageInOneLine(pressStopMessage);

  // Дребез контактов 
  delay(waitTimeAfterPushButton);

  Serial.println("stopwatch - start while");
  while(true){

    // Проверяем состояние кнопки СТОП, в случае её окончания - выходим из счета секундомера
    if(digitalRead(buttonStopPin)  == LOW){
      break;
    }
    delay(10);
  }
  Serial.println("stopwatch - stop while");
  
  // Время окончания работы секундомера
  long resultTime = millis();

  // Зажим магнита
  setRelayState(true);

  // Время работы секундомера
  long result = resultTime - initTime;

  // Вывод результата
  printMessageInTwoLine(resultMessage, convertMillistToString(result));

  // Дребез контактов 
  delay(waitTimeAfterPushButton);

  while(true){

    // Проверяем состояние кнопки СТОП, в случае её окончания - выходим из метода
    if(digitalRead(buttonStopPin)  == LOW){
      break;
    }
  }

  delay(waitTimeAfterPushButton);
  Serial.println("stopwatch end method");
}

// --------------------------------- Печать сообщение в две строки
// firstLine - первая строка; secondLine - вторая строка
void printMessageInTwoLine(String firstLine, String secondLine){
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print(firstLine);
  lcd.setCursor(1,1);
  lcd.print(secondLine);

  Serial.println("led print; ");
  Serial.print("firstLine:" + firstLine + "; ");
  Serial.print("secondLine:" + secondLine);
  Serial.println("");
}

// --------------------------------- Печать сообщение в одну строку
void printMessageInOneLine(String firstLine){
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print(firstLine);

  Serial.println("firstLine:" + firstLine + ";");
}

// --------------------------------- Управление реле
void setRelayState(bool state){
    Serial.println("setRelayState " + String(state));
    if (state){
        digitalWrite(relayPin, LOW);
    } else {
        digitalWrite(relayPin, HIGH);
    }
}

// --------------------------------- Конвертация времени в строку
String convertMillistToString(long time){
  long second = time / 1000;
  long millisecond = time - second * 1000; 

  return String(second) + "." + String(millisecond) + "s";
}