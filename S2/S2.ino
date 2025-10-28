#include <WiFi.h>
#include <PubSubClient.h>
#include "env.h"
#include <WiFiClientSecure.h>

WiFiClient client; //cria objeto p/ wifi
PubSubClient mqtt(client);

const String mensagem_ligar[6] = {"Acender", "Ligar", "On", "1", "True", "Xazam"};
const String mensagem_desligar[5] = {"Apagar", "Desligar", "Off", "0", "False"};

// int nome[ = {}]
void callback(char* topico, byte* mensagem, unsigned int length){
  Serial.println("Recebido");
  Serial.println(topico);
}

void setup() {
  wifiClient.setInsecure():
  Serial.begin (115200); //configura a placa para mostrar na tela
  WiFi.begin(SSID, PASS); //tenta conectar na rede
  Serial.println("Conectando no WiFi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(200);
  }
  Serial.println("conectado com sucesso");

  mqtt.setServer(BROKER_URL, BROKER_PORT);
  Serial.println("Conectando no Broker");

  String boardID = "S2-"; // cria um nome que conecta com S''

  boardID += String(random(0xffff),HEX); //nta o S'' com um número aleatório hexadecimal

  while(!mqtt.connected()) {
      mqtt.connect(userId.c_str(), BROKER_USR_NAME, BROKER_USR_PASS); 
      Serial.println(".");
    delay(200);
  }
  mqtt.setCallback(callback);
    mqtt.subscribe(TOPIC2);

  Serial.println("\nConectado cpm sucesso ao broker!");
}



void loop() {
  // put your main code here, to run repeatedly:

  String mensagem = "";
  if(Serial.available() > 0) {
    mensagem = Serial.readStringUntil('\n');
    Serial.print("Mensagem digitada: ");
    Serial.println(mensagem);
    mensagem = "Miguel: " + mensagem;
    mqtt.publish(TOPIC1, mensagem.c_str());
  }


}
