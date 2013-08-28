/*
Помогите с прерываниями по таймерам Arduino MEGA
Программинг микроконтроллеров*, Arduino
День добрый. Голова пухнет кучей других вопросов и никак не хочет осилить работу с таймерами((((

Что надо:
— вызывать два прерывания на разных таймерах
— иметь возможность быстрого изменения частоты работы таймеров

Вот есть такой код (простите, но тег code как то тут все в кашу смешал)
*/
volatile unsigned int tcnt2;
volatile unsigned int flag; 
void setup() 
{ 
    flag=true; TIMSK2 &= ~(1<<TOIE2); //разрешения прерывания по переполнению таймера/счетчика 
    Т2 TCCR2A &= ~((1<<WGM21) | (1<<WGM20)); // Режим работы таймера/счетчика 
    TCCR2B &= ~(1<<WGM22);// Режим работы таймера/счетчика 
    ASSR &= ~(1<<AS2); //Выбор источника синхронизации таймера если AS2=0 от системного генератора 
    tcnt2 = 1; // 16000000/64/f=tcnt2 
    TIMSK2 |= (1<<TOIE2);//Разрешение прерывания по переполнению Т2. 
}
void loop() 
{

} 

void MyInterupt() 
{ 
    if (flag) 
        { flag=false; } 
    else
        { flag=true; } //обработчик вашего прерывания 
} 
        //****************обработчик прерывания******************** 
ISR(TIMER2_OVF_vect) 
{
    TCNT2 = tcnt2; 
    MyInterupt(); 
}

/*
Он как бы работает и делает то, что надо. Почти.
Хочу попросить исправить его так, что бы:
— можно было выбирать таймер. Т.е. какие значения надо писать для каких таймеров? Если правильно понимаю — то в этой строчке выбирается таймер и режим его работы
TIMSK2 &= ~(1<<TOIE2); //разрешения прерывания по переполнению таймера/счетчика Т2
Но нам для изменения (плавного) частоты его работы надо задать не переполнение, а, видимо, совпадение с определенным значением и менять это значение?

И не совсем понятно как привязывается обработчик прерывания к конкретному счетчику
****************обработчик прерывания******************** 
29 ISR(TIMER2_OVF_vect) 30 { 31 TCNT2 = tcnt2; 32 MyInterupt(); 33 }

В общем мне нужен скетч с подробным комментариями вида:
— вот тут мы выбираем таймер — значения для таймера 1, 2, 3 надо ввести такие-то
— вот тут мы задаем скорость работы — значения от и до…
— вот тут мы включаем прерывание (вкл такое-то значение, выкл такое-то)
— вот тут мы цепляем обработчик на такое-то прерывания. Значения такие-то будут для таких-то таймеров.
*/

void setup()
{
//------ Timer0 ----------
TCCR0A = (1<<WGM01);   // Режим CTC (сброс по совпадению)
TCCR0B = (1<<CS00);    // Тактирование от CLK.
                       // Если нужен предделитель :
// TCCR0B = (1<<CS01);           // CLK/8
// TCCR0B = (1<<CS00)|(1<<CS01); // CLK/64
// TCCR0B = (1<<CS02);           // CLK/256
// TCCR0B = (1<<CS00)|(1<<CS02); // CLK/1024

OCR0A = 123;            // Верхняя граница счета. Диапазон от 0 до 255.
                        // Частота прерываний будет = Fclk/(N*(1+OCR0A)) 
                        // где N - коэф. предделителя (1, 8, 64, 256 или 1024)
TIMSK0 = (1<<OCIE0A);   // Разрешить прерывание по совпадению

//------ Timer1 ----------
TCCR1B = (1<<WGM12);    // Режим CTC (сброс по совпадению)
TCCR1B |= (1<<CS10);    // Тактирование от CLK.
                        // Если нужен предделитель :
// TCCR1B |= (1<<CS11);           // CLK/8
// TCCR1B |= (1<<CS10)|(1<<CS11); // CLK/64
// TCCR1B |= (1<<CS12);           // CLK/256
// TCCR1B |= (1<<CS10)|(1<<CS12); // CLK/1024

OCR1A = 2678;           // Верхняя граница счета. Диапазон от 0 до 65535.
                        // Частота прерываний будет = Fclk/(N*(1+OCR1A)) 
                        // где N - коэф. предделителя (1, 8, 64, 256 или 1024)
TIMSK1 = (1<<OCIE1A);   // Разрешить прерывание по совпадению

//------ Timer2 ----------
TCCR2A = (1<<WGM21);    // Режим CTC (сброс по совпадению)
TCCR2B = (1<<CS20);     // Тактирование от CLK.
                        // Если нужен предделитель :
// TCCR2B = (1<<CS21);                     // CLK/8
// TCCR2B = (1<<CS20)|(1<<CS21);           // CLK/32
// TCCR2B = (1<<CS22);                     // CLK/64
// TCCR2B = (1<<CS20)|(1<<CS22);           // CLK/128
// TCCR2B = (1<<CS21)|(1<<CS22);           // CLK/256
// TCCR2B = (1<<CS20)|(1<<CS21)|(1<<CS22); // CLK/1024

OCR2A = 234;            // Верхняя граница счета. Диапазон от 0 до 255.
                        // Частота прерываний будет = Fclk/(N*(1+OCR2A)) 
                        // где N - коэф. предделителя (1, 8, 32, 64, 128, 256 или 1024)
TIMSK2 = (1<<OCIE2A);   // Разрешить прерывание по совпадению

sei ();                 // Глобально разрешить прерывания
}

ISR (TIMER0_COMPA_vect)
{
    // Обработчик прерывания таймера 0
}

ISR (TIMER1_COMPA_vect)
{
    // Обработчик прерывания таймера 1
}

ISR (TIMER2_COMPA_vect)
{
    // Обработчик прерывания таймера 2
}

