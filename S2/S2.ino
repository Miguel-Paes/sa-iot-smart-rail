#include <WiFi.h>
#include <PubSubClient.h>
#include "env.h"
#include <WiFiClientSecure.h>
#include <Servo.h>

WiFiClient client; //cria objeto p/ wifi
PubSubClient mqtt(client);

//Conexão
const char *mqtt_broker = "1acd7ac2686b4748913cce80ad4161df.s1.eu.hivemq.cloud";
const int mqtt_port = 8883; // Porta padrão
const char *mqtt_user = "user_placa_2";
const char *mqtt_password = "Userplaca2";

//Tópicos - Publish
const String topicoPres1 = "SmartRail/S2/Presenca1"; 
const String topicoPres2 = "SmartRail/S2/Presenca2"; 

//Tópicos - Subscribe
const String topicoSubIlum = "SmartRail/S1/Lumn";

//Servo
Servo meuServo1;

// declaração de variaveis
const byte ldr = A0;
int valorLdr = 0;
const byte pinoTrig = 13;
const byte pinoEcho = 12;
const byte pinoEcho2 = 11;
const byte pinoTrig2 = 10;
const byte pinoServo1 = 2;

float duracao, distancia;

void controladorLedsPresenca(float distance){
	if(distance > 1000){
		digitalWrite(ledVerde, HIGH);
    digitalWrite(ledVermelho, LOW);
    delay(1000);
	} else if(distance < 1000){
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledVermelho, HIGH);
    delay(1000);
  } else {
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledVermelho, LOW);
    delay(1000);
  }
}

void atuadorServo() 
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

void calcularDistanciaDuracaoPresenca1(float distance, float duration)
{
	duration = pulseIn(pinoEcho, HIGH);
  	distance = (duration*0.343)/2;
  	Serial.println("Distancia do sensor de presenca1: ");
  	Serial.println(distance);
	delay(1000);
}

void calcularDistanciaDuracaoPresenca2(double distance2, double duration2)
{
	duration2 = pulseIn(pinoEcho2, HIGH);
  	distance2 = (duration2*0.343)/2;
    Serial.println("Distancia do sensor de presenca 2: ");
    Serial.println(distance2);
    delay(1000);            
}

void sensorLuminosidade(int valor){
	valor = analogRead(ldr);
	Serial.println("Valor lido pelo LDR: ");
  	Serial.println(valor);
}

void callback(char* topic, byte* payload, unsigned long length){
  String mensagemRecebida1 = "";
  String mensagemRecebida2 = "";
  for(int i = 0; i < length; i++){
    mensagemRecebida1 += (char) payload[i];
	mensagemRecebida2 += (char) payload[i];
  }
  // conversao para inteiro
  byte int distance1 = mensagemRecebida1.toInt();
  byte int distance2 = mensagemRecebida2.toInt();
  Serial.println(mensagemRecebida1);
  Serial.println(mensagemRecebida2);

  // Verificacao em qual topico a mensagem chegou
  String topicoStr = String(topic);

  if(topicoStr == "SmartRail/S2/Presenca1") {
     controladorLedsPresenca(distance1);
  }

  if(topicoStr == "SmartRail/S2/Presenca2"){
    controladorLedsPresenca(distance2);
  }
}

void conectarWifi(){
  Serial.println("Conectando no WiFi");
  // enquanto não estiver conectado no wifi mostra "." e liga o led amarelo
  while(WiFi.status() != WL_CONNECTED){
    ledcWrite(ledVermelho, 150);
    ledcWrite(ledVerde, 150);
    ledcWrite(ledAzul, 0);
    delay(1000);
  }
  Serial.println("\n Conectado com sucesso!");
}

// funcao configurar servidor do broker/porta
String configurarServidorBroker(){
  // configura o servidor do broker/porta
  mqtt.setServer(*mqtt_broker, mqtt_port);
  Serial.println("Conectando no Broker");
  
  // cria um userID
  String userID = "S4-NATHAN"; 

  // junta o "s4-" com um numero aleatorio hexadecimal
  userID += String(random(0xffff), HEX);
  return userID;
}

// funcao para conectar broker
void conectarBroker(){
  String userID = configurarServidorBroker();
  // Loop para conectar no broker
  while(!mqtt.connected()){
    if(mqtt.connect(userID.c_str(), *mqtt_user, *mqtt_password)){
      Serial.print('.');
      ledcWrite(ledVermelho, 150);
      ledcWrite(ledAzul, 150);
      ledcWrite(ledVerde, 0);
      delay(2000);
    } else { // caso dê erro de conexao com broker
      ledcWrite(ledVermelho, 255);
      ledcWrite(ledAzul, 0);
      ledcWrite(ledVerde, 0);
      delay(2000);
    }
  }
}

void setup()
{
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAzul, OUTPUT);

  // define os leds como entrada analogico
  ledcAttach(ledVermelho, 5000, 8);
  ledcAttach(ledVerde, 5000, 8);
  ledcAttach(ledAzul, 5000, 8);
  
  // configuração do wifi
  wifiClient.setInscure();
  Serial.begin(115200); //Configura a placa para mostrar na tela
  WiFi.begin(SSID, PASS); //Tenta conectar na rede
  
  // conectando no wifi
  conectarWifi();
  //conectando no broker
  conectarBroker();

  meuServo1.attach(pinoServo1);
  	pinMode(pinoTrig, OUTPUT);
  	pinMode(pinoEcho, INPUT);
  	pinMode(pinoTrig2, OUTPUT);
  	pinMode(pinoEcho2, INPUT);
	pinMode(ldr, INPUT);
  	Serial.begin(9600);
}

void loop()
{
  calcularDistanciaDuracaoPresenca1(duracao, distancia);
  atuadorServo();
  calcularDistanciaDuracaoPresenca2(duracao, distancia);
}
