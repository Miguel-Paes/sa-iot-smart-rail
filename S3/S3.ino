//ARQUIVO S3 - VINICIUS RUBENS DOS SANTOS - SMART RAIL (SR)

#include <Servo.h>

Servo meuServo1; // cria um objeto do tipo servo
Servo meuServo2; 

// declaração de variaveis
const byte ldr = A0;
const byte pinoTrig = 13;
const byte pinoEcho = 12;
const byte pinoServo1 = 2;
const byte pinoServo2 = 3;
int valorLdr = 0;
float duracao, distancia;

// declaração de funções

void sensorLuminosidade(int valorLumin)
{
	valorLumin = analogRead(ldr);
  	Serial.println("Valor lido pelo LDR = ");
  	Serial.println(valorLumin);
}

void ligarPinoSensorPresenca()
{
  // Liga o pino de saída, espera dois microssegundos e desliga, depois espera dez microsegundos e liga denovo.
	digitalWrite(pinoTrig, LOW);
  	delayMicroseconds(2);
  	digitalWrite(pinoTrig, HIGH);
	delayMicroseconds(10);
    digitalWrite(pinoTrig, LOW);
}


void calcularDistanciaDuracao(float distance, float duration)
{
	duration = pulseIn(pinoEcho, HIGH);
  	distance = (duration*0.343)/2;
  	Serial.println("Distancia: ");
  	Serial.println(distance);
	delay(100);
}

void atuadorServo1() 
{
	
  	// mover o servo motor para 0 graus
  	meuServo1.write(0);
  	delay(1000);
  	// mover o servo para 180 graus
  	meuServo1.write(180);
  	delay(1000);
  	// retorna para a posição inicial
  	meuServo1.write(90);
  	delay(1000);
}

void atuadorServo2()
{
  	// mover o servo motor para 0 graus
  	meuServo2.write(0);
  	delay(1000);
  	// mover o servo para 180 graus
  	meuServo2.write(180);
  	delay(1000);
  	// retorna para a posição inicial
  	meuServo2.write(90);
  	delay(1000);
}

//codigo principal

void setup()
{
  	meuServo1.attach(pinoServo1); // associa o objeto Servo ao pino
  	meuServo2.attach(pinoServo2);
  	pinMode(pinoTrig, OUTPUT);
    pinMode(pinoEcho, INPUT);
	pinMode(ldr, INPUT);
  	Serial.begin(9600);
}

void loop()
{
	sensorLuminosidade(valorLdr);
	ligarPinoSensorPresenca();
  calcularDistanciaDuracao(distancia, duracao);
  atuadorServo1();
  atuadorServo2();
}
