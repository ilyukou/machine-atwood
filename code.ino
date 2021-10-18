// --------------------------------- Импорт сторонних библиотек
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);
// Порты для экранчика
// SDA	A4
// SCL	A5

// --------------------------------- Задание константных значений
// Пин кнопки ПУСК
const int buttonStartPin = 5;

// Пин кнопки СТОП
const int buttonStopPin = 4;

// Пин реле
const int relayPin = 3;

// Пин кнопки ДУ ПУСК - Включает РЕЛЕ
const int buttonControlStartPin = 6;

// Пин кнопки ДУ СТОП - Выключает РЕЛЕ
const int buttonControlStopPin = 7;

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

// --------------------------------- Настройка
void setup() {
  pinMode(buttonStartPin, INPUT);
  pinMode(buttonStopPin, INPUT);

  pinMode(buttonControlStartPin, INPUT);
  pinMode(buttonControlStopPin, INPUT);

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
    // Проверяем зажата ли кнопка ПУСК. Зажатие == земля.
    if(digitalRead(buttonStartPin)  == LOW){
      Serial.println("buttonStartPin НАЖАТА");
      stopwatch();

      // Печатает Вступительно сообщение в одну строку, после окончания работы секундомера
      printMessageInOneLine(welcomeMessage);
    }
    // Проверяет кнопки ДУ и изменяет состояние реле
    controlButtonManagement();
    delay(10);
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

  // Убераем дребез контактов 
  delay(waitTimeAfterPushButton);

  Serial.println("stopwatch - start while");
  while(true){

    // Проверяем состояние кнопки СТОП, в случае её окончания - выходим из счета секундомера
    if(digitalRead(buttonStopPin)  == LOW){
      break;
    }
    // Проверяет кнопки ДУ и изменяет состояние реле
    controlButtonManagement();
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

  // Убераем дребез контактов 
  delay(waitTimeAfterPushButton);

  while(true){

    // Проверяем состояние кнопки СТОП, в случае её окончания - выходим из метода
    if(digitalRead(buttonStopPin)  == LOW){
      break;
    }
    // Проверяет кнопки ДУ и изменяет состояние реле
    controlButtonManagement();
    delay(10);
  }

  // Убераем дребез контактов 
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

// --------------------------------- Проверка кнопок ДУ и изменение состояние РЕЛЕ
void controlButtonManagement(){
  if(digitalRead(buttonControlStartPin)  == LOW){
    Serial.println("buttonControlStartPin НАЖАТА");

    // Включаем РЕЛЕ
    setRelayState(true);
    // Убираем дребезг контактов
    delay(waitTimeAfterPushButton);
  }

  if(buttonControlStopPin == LOW){
    Serial.println("buttonControlStopPin НАЖАТА");

    // Выключаем РЕЛЕ
    setRelayState(false);
    // Убираем дребезг контактов
    delay(waitTimeAfterPushButton);
  }
}