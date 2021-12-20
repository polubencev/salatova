#include <Stepper.h>

#define LASER_PIN 7
#define STEP_A 9
#define STEP_B 10
#define STEP_C 11
#define STEP_D 12
#define DETECTOR_PIN 5

const int step_to_turn = 2048;                    //Число шагов двигателя на 1 оборот
const double a = 10;                              //Расстояние между лазером и датчиком
const double h = 8.660254038;                     //Высота равностороннего треугольника
const double H = 500.0;                           //Высота трубы к примеру 500мм
bool finder = true;
int steps = 0;
double Hv = 0;
char bufferS[15];
Stepper stepper = Stepper(4096, STEP_A,STEP_B,STEP_C,STEP_D);

void setup() {
   Serial.begin(9600);
   pinMode(DETECTOR_PIN, INPUT);
   pinMode(LASER_PIN, OUTPUT);
   attachInterrupt(0, calculate, CHANGE);           //Установка прерывания
   stepper.setSpeed(1);

}

void loop() {
  if(finder){                                //проверяем финдер, если тру то
    digitalWrite(LASER_PIN, HIGH);           //включаем лазер
    stepper.step(1);                         //поворачиваем ШД на 1 шаг
    steps++;                                 //считаем шаги
  }else{                                     //если датчик сработал, то..
    Hv = (H - (steps * 0.001953) - h);       //считаем по формуле уровень
    digitalWrite(LASER_PIN, LOW);            //тушим лазер
    dtostrf(Hv, 4,4, bufferS);               // Печатаем все на экран
    Serial.println("\nУровень найден!");
    Serial.print(Hv);
    Serial.print(" mm");
    delay(5000);                             //ждем 5 секунд и снова ищем уровень
    finder = true;                            // ставим финдер в тру, чтобы снова начать поиск
  }
}
//Обработчик прерывания
void calculate(){
  finder = false;
}
