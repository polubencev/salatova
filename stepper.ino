#include <Stepper.h>

#define LASER_PIN 7
#define STEP_A 9
#define STEP_B 10
#define STEP_C 11
#define STEP_D 12
#define DETECTOR_PIN 5

const double step_motor = 0.18;                   //поворот двигателя на 0,18 градусов за 1 шаг
const double transmission = 0.25;
const double transmission_step = 4.0;                  //один поворт ШПВ 4 мм
const int step_to_turn = 4096;                    //Число шагов двигателя на 1 оборот
const double a = 10;                              //Расстояние между лазером и датчиком
const double h = 8.660254038;                     //Высота равностороннего треугольника
const double H = 500.0;                             //Высота трубы к примеру 500мм

Stepper stepper = Stepper(4096, STEP_A,STEP_B,STEP_C,STEP_D);

double stepTomm(double step_motor, double transmission_step);
void init_stepper(int speed);
void stepper_on(bool state);
void laser_init();
void init_detector();
void laser_on(bool state);
void showMessage(int msg);                       //Перегрузка метода для показа сообщений
void showMessage(String msg);                    //Перегрузка метода для показа сообщений
void calculate();
int detector_state();
int temp = 0;                                    //Вспомогательная переменная                                         
int steps_motor = 0;
double Hv = 0;                                      //Высота воды
bool finder = true;
static char bufferS[15];

// Метод настройки и прочего
void setup(){
  Serial.begin(9600);
  init_stepper(1);
  init_detector();
  laser_init();
  showMessage("********************************************");
  showMessage("Старт программы!");
  showMessage("********************************************");
  attachInterrupt(0, calculate, CHANGE);           //Установка прерывания
}

void loop(){
  if(finder){
    stepper_on(true);
    laser_on(true);
    steps_motor++;
  }else{
    stepper_on(false);
    laser_on(false);
    Hv = H -((stepTomm(steps_motor, transmission)/ 2000.0) + h);
    dtostrf(Hv, 4,4, bufferS);                           //Это короче для правильного отображения числа с плавающей точкой,лайфхак из Си
    showMessage("Уровень жидкости найден!");
    showMessage(bufferS);
    showMessage("mm");
    delay(5000);
    finder = true;
  }
}
//Перевод одного шага двигателя в миллиметры
double stepTomm(double step_motor, double transmission){
  
  return ((step_motor * transmission_step) / 2000.0);
}
//Инициализация и настройка шагового двигателя
void init_stepper(int speed){ 
 stepper.setSpeed(speed); 
}
//Инициализация и настройка лазера
void laser_init(){
  pinMode(LASER_PIN, OUTPUT);
}
//Инициализация детектора
void init_detector(){
  pinMode(DETECTOR_PIN, INPUT);
}
//Включение лазера (испульсно)
void laser_on(bool state){
  if(state){
    digitalWrite(LASER_PIN, HIGH);
    delay(5);
    digitalWrite(LASER_PIN, LOW);
    delay(5);
  }else{
    digitalWrite(LASER_PIN, LOW);
  }    
}
//Читаем данные с фотодатчика
int detector_state(){
  return(digitalRead(DETECTOR_PIN));
}
//Показать сообщение в консоль "строка"
void showMessage(String msg){
  Serial.println(msg);
}
//Показать сообщение в консоль "значение"
void showMessage(int msg){
  Serial.println(msg);
}
//Включаем шаговый
void stepper_on(bool state){
  if(state){
    stepper.step(1);
  }
}
//Обработчик прерывания
void calculate(){
  showMessage("Interrupt!");
  finder = false;
}
