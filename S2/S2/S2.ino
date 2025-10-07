#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient client; //cria objeto p/ wifi
PubSubClient mqtt(client);

const String SSID = "FIESC_IOT_EDU";
const String PASS = "8120gv08";

const String brokerURL = "test.mosquitto.org";
const int brokerPort = 1883; 

const String brokerUser = ""; //variável para uso do broker
const String brokerPass = ""; //variavel para a senha do broker

void setup() {
  Serial.begin (115200); //configura a placa para mostrar na tela
  WiFi.begin(SSID, PASS); //tenta conectar na rede
  Serial.println("Conectando no WiFi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(200);
  }
  Serial.println("conectado com sucesso");

  mqtt.setServer(brokerURL.c_str(),brokerPort);
  Serial.println("Conectando no Broker");

  String boardID = "S2-"; // cria um nome que conecta com S''

  boardID += String(random(0xffff),HEX); //nta o S'' com um número aleatório hexadecimal

  while(!mqtt.connect(boardID.c_str())){
    Serial.print(".");
    delay(200);
  }
  Serial.println("\nConectado cpm sucesso ao broker!");
}



void loop() {
  // put your main code here, to run repeatedly:

}
