
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

// --------------------------------- Задание переменных значений


// Состояние реле.
int relayState = HIGH;

// Состояние реле digitalRead(relayPin) при котором включается реле.
int relayOnDigitalRead = LOW;


// --------------------------------- Настройка
void setup() {
  pinMode(buttonStartPin, INPUT);
  pinMode(buttonStopPin, INPUT);

  pinMode(relayPin, OUTPUT);


  Serial.begin(9600);
}


// --------------------------------- Главный цикл
void loop() {
   

    int buttonStartState = digitalRead(buttonStartPin);
    Serial.println("buttonStartState " + String(buttonStartState));
    if(buttonStartState  == 0){
        relayState = HIGH;
        digitalWrite(relayPin, relayState);
    }

    int buttonStopState = digitalRead(buttonStopPin);
    Serial.println("buttonStopState " + String(buttonStopState));
    if(buttonStopState == 0){
      relayState = LOW;
        digitalWrite(relayPin, relayState);
    }
}

// firstLine - первая строка; secondLine - вторая строка
void printMessage(String firstLine, String secondLine){

}
