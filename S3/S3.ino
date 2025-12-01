//ARQUIVO S3 - VINICIUS RUBENS DOS SANTOS - SMART RAIL (SR)

#include <Servo.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include "env.h"

// Objeto wifi

WiFiClientSecure wifiClient;
WiFiClient client;         
PubSubClient mqtt(client); 

// Topicos

const String topico1 = "SmartRail/S3/Presenca4"; 
const String topico2 = "SmartRail/S3/Led_s3";
const String topico3 = "SmartRail/S3/Servo2";
const String topico4 = "SmartRail/S3/Servo3";
const String topico5 = "SmartRail/S3/Led_rgb_s3";

// Servos

Servo meuServo1;  
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
void controladorLedsPresenca(float distance) {
  if (distance > 1000) {
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledVermelho, LOW);
  } else if (distance < 1000) {
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledVermelho, HIGH);
  } else {
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledVermelho, LOW);
  }
}

void callback(char* topic, byte* payload, unsigned long length) {
  String mensagemRecebida = "";
  for (int i = 0; i < length; i++) {
    mensagemRecebida += (char)payload[i];
  }
  byte int velocidade = mensagemRecebida.toInt();
  Serial.println(mensagemRecebida);
  // controle dos leds
  controladorLeds(distance);
}

void calcularDistanciaDuracao(float distance, float duration) {
  duration = pulseIn(pinoEcho, HIGH);
  distance = (duration * 0.343) / 2;
  Serial.println("Distancia: ");
  Serial.println(distance);
  delay(100);
}

void atuadorServo1() {
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

void atuadorServo2() {
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

// funcao para conectar wifi
void conectarWifi() {
  Serial.println("Conectando no WiFi");
 // Loop para conectar no wifi
  while (WiFi.status() != WL_CONNECTED) {
    ledcWrite(ledVermelho, 150);
    ledcWrite(ledVerde, 150);
    ledcWrite(ledAzul, 0);
    delay(1000);
  }
  Serial.println("\n Conectado com sucesso!");
}

// funcao configurar servidor do broker/porta
String configurarServidorBroker() {
  // configura o servidor do broker/porta
  mqtt.setServer(BROKER_URL, BROKER_PORT);
  Serial.println("Conectando no Broker");

  // cria um nome que começa com s3-
  String userID = "S3-RUBENS";

  // junta o "s3-" com um numero aleatorio hexadecimal
  userID += String(random(0xffff), HEX);
  return userID;
}

// funcao para conectar broker
void conectarBroker() {
  String userID = configurarServidorBroker();
  // Loop para conectar no broker
  while (!mqtt.connected()) {
    if (mqtt.connect(userID.c_str(), BROKER_USR_NAME, BROKER_USR_PASS)) {
      Serial.print('.');
      ledcWrite(ledVermelho, 150);
      ledcWrite(ledAzul, 150);
      ledcWrite(ledVerde, 0);
      delay(2000);
    } else {  // caso dê erro de conexao com broker
      ledcWrite(ledVermelho, 255);
      ledcWrite(ledAzul, 0);
      ledcWrite(ledVerde, 0);
      delay(2000);
    }
  }
}

//codigo principal

void setup() {
  // define os pinos dos leds
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAzul, OUTPUT);

  meuServo1.attach(pinoServo1);  
  meuServo2.attach(pinoServo2);
  pinMode(pinoTrig, OUTPUT);
  pinMode(pinoEcho, INPUT);

  // define os leds como entrada analogico
  ledcAttach(ledVermelho, 5000, 8);
  ledcAttach(ledVerde, 5000, 8);
  ledcAttach(ledAzul, 5000, 8);

  //configuração do wifi
  wifiClient.setInscure();
  Serial.begin(115200);    // configura a placa para mostrar na tela
  WiFi.begin(SSID, PASS);  // tenta conectar na rede

  //  conectando no wifi
  conectarWifi();

  // conectando no broker
  conectarBroker();

  mqtt.subscribe(topico.c_str());  
  mqtt.subscribe("miguel123");
  mqtt.setCallback(callback);
  Serial.println("Conectado com sucesso!");
}

void loop() {
  calcularDistanciaDuracao(distancia, duracao);
  controladorLedsPresenca(distancia);
  atuadorServo1();
  atuadorServo2();
}
