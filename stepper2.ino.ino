#include <Stepper.h>

#define LASER_PIN 7                               //Тут наверное понятно, просто даем псевдонимы номерам пинов Arduino
#define STEP_A 9
#define STEP_B 10
#define STEP_C 11
#define STEP_D 12
#define DETECTOR_PIN 2
#define ZERO_PIN     3

const int step_to_turn = 2048;                    //Число шагов двигателя на 1 оборот
const double a = 10;                              //Расстояние между лазером и датчиком
const double h = 8.660254038;                     //Высота равностороннего треугольника
const double H = 500.0;                           //Высота трубы к примеру 500мм
bool finder = true;                               //Логическая переменная, для запуска и остановки движения лазера
bool onZero = false;                              //Переменная для отслеживания концевика
int steps = 0;
double Hv = 0;                                    // Переменная, куда мы сохраняем найденную высоту
char bufferS[15];                                 //Это массив из 15 однобайтовых символов, для корректного отображения нашей высоты в терминале
Stepper stepper = Stepper(4096, STEP_A,STEP_B,STEP_C,STEP_D);

void setup() {
   Serial.begin(9600);                            //Это настройка последовательного порта(это просто для вывода на экран нашей информации)
   pinMode(DETECTOR_PIN, INPUT);                  //Настраиваем пин,куда фотодатчик подключен как ВХОД
   pinMode(ZERO_PIN, INPUT);                      //Концевик наш
   pinMode(LASER_PIN, OUTPUT);                    //Ну, а где лазер - на ВЫХОД
   attachInterrupt(0, calculate, CHANGE);         //Установка прерывания(наверно понятно)
   attachInterrupt(1, setZero, CHANGE);           //Устанавливаем прерывание на концевик!
   stepper.setSpeed(1);                           //Скорость шаговика

}

void loop() {
  if(finder && !onZero){                          //если лазер не на нуле и финдер тру,то ищем уровень
    digitalWrite(LASER_PIN, HIGH);                //включаем лазер
    stepper.step(1);                              //поворачиваем ШД на 1 шаг
    steps++;                                      //считаем шаги
  }else{                                          //если датчик сработал, то..
    Hv = (H - (steps * 0.001953) - h);            //считаем по формуле уровень
    digitalWrite(LASER_PIN, LOW);                 //тушим лазер
    dtostrf(Hv, 4,4, bufferS);                    // Это короче функция языка Си, для перевода double в строку, корорую мы помещаем в buffeS. Чтобы напечатать
    Serial.println("\nУровень найден!");
    Serial.print(bufferS);                        //Ну тут печатаем нашу высоту. С 4мя цифрами после запятой.
    Serial.print(" mm");
    while(!onZero){                               //пока не замкнется концевик едем назад.
      goToZero();
    }
    delay(5000);                                  //ждем 5 секунд и снова ищем уровень
    finder = true;                                // ставим финдер в тру, чтобы снова начать поиск
  }
}
//Обработчик прерывания
void calculate(){
  finder = false;                                 //Когда фото ловит лазер, то срабатывает эта функция и лазер останавливается, т.к. finder становится FALSE
}
//Обрабатываем прерывание от концевика
void setZero(){
   onZero = true;
}
//функция движения назад до концевика
void goToZero(){
  stepper.step(-1);
}
//--------------END-----------------//
