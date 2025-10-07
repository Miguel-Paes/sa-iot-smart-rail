//ARQUIVO S1 - MIGUEL PAES DA SILVA - SMART RAIL (SR)

#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient client; //Cria objeto para wifi
PubSubClient mqtt(client); //Cria objeto para MQTT usando wifi

const String SSID = "FIESC_IOT_EDU";
const String PASS = "8120gv08";

const String brokerURL = "test.mosquitto.org";
const int brokerPort = 1883;

const String brokerUser = "";
const String brokerPass = "";

void setup() {
  Serial.begin(115200); //Configura a placa para mostrar na tela
  WiFi.begin(SSID, PASS); //Tenta conectar na rede
  Serial.println("Conectando ao WiFI");
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }
    Serial.println("\nConectado com Sucesso!");

    mqtt.setServer(brokerURL.c_str(), brokerPort);
    Serial.println("Conectando no Broker");

    String boardID = "S1-";

    boardID += String(random(0xffff), HEX);
    
    while(!mqtt.connect(boardID.c_str())) {
      Serial.println(".");
      delay(200);
    }
    Serial.println("\nConectado com sucesso ao Broker");
}

void loop() {

}
