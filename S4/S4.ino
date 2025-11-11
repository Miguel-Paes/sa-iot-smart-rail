#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include "env.h"

WiFiClientSecure wifiClient;
WiFiClient client;         // cria o objeto para o wifi
PubSubClient mqtt(client);// cria o objeto para o mqtt usando wifi

const String topico = "SmartRail/S4/Trem/vel"; // nome do topico

// declaração de variaveis
const int ledVermelho = 7;
const int ledVerde = 6;

// ----------------------- abaixo do loop/funções principais do sensor-----------------------

// funcao para controlar leds
void controladorLeds(byte velocidade){
  if(velocidade > 0) {
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledVermelho, LOW);
  } else if(velocidade == 100){
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledVermelho, HIGH);
  } else {
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledVermelho, LOW);
  }
}

void callback(char* topic, byte* payload, unsigned long length){
  String mensagemRecebida = "";
  for(int i = 0; i < length; i++){
    mensagemRecebida += (char) payload[i];
  }
  byte int velocidade = mensagemRecebida.toInt();
  Serial.println(mensagemRecebida);
  // fazer o controle de leds aqui
  controladorLeds(velocidade);
}

void setup() {
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  wifiClient.setInscure();
  Serial.begin(115200); // configura a placa para mostrar na tela
  WiFi.begin(SSID, PASS); // tenta conectar na rede
  Serial.println("Conectando no WiFi");

  // enquanto não estiver conectado mostra "."
  while(WiFi.status() != WL_CONNECTED){
    Serial.println(".");
    delay(200);
  }
  Serial.println("\n Conectado com sucesso!");

  // configura o servidor do broker/porta
  mqtt.setServer(BROKER_URL, BROKER_PORT);
  Serial.println("Conectando no Broker");
  
  // cria um nome que começa com s4-
  String userID = "S4-NATHAN"; 

  // junta o "s4-" com um numero aleatorio hexadecimal
  userID += String(random(0xffff), HEX);

  // tenta conectar ao Broker
  // enquanto não estiver conectado mostra "."
  while(!mqtt.connected()){
    mqtt.connect(userID.c_str(), BROKER_USR_NAME, BROKER_USR_PASS);
    Serial.print('.');
    delay(2000);
  }

  mqtt.subscribe(topico.c_str()); // inscrever em um topico
  mqtt.subscribe("miguel123");
  mqtt.setCallback(callback);
  Serial.println("Conectado com sucesso!");
}

void loop() {
  String mensagem = "";
  if(Serial.available() > 0){
    mensagem = Serial.readStringUntil('\n');
    Serial.print("Mensagem digitada: ");
    Serial.println(mensagem);
    mensagem = "Nathan: " + mensagem;
    mqtt.publish("miguel123", mensagem.c_str()); //envia msg
  }  
  mqtt.loop();
  callback(byte velocidade);
}