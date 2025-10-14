//ARQUIVO S1 - MIGUEL PAES DA SILVA - SMART RAIL (SR)

#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient client; //Cria objeto para wifi
PubSubClient mqtt(client); //Cria objeto para MQTT usando wifi

const String SSID = "FIESC_IOT_EDU";
const String PASS = "8120gv08";

const String brokerURL = "test.mosquitto.org";
const int brokerPort = 1883;
const String topic = "miguel123";

const String brokerUser = "";
const String brokerPass = "";

const String mensagem_ligar[6] = {"Acender", "Ligar", "On", "1", "True", "Xazam"};
const String mensagem_desligar[5] = {"Apagar", "Desligar", "Off", "0", "False"};


// int nome[ = {}]
void callback(char* topico, byte* mensagem, unsigned int length){
  Serial.println("Recebido");
  Serial.println(topico);
}


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
    mqtt.setCallback(callback);
    mqtt.subscribe(topic.c_str());

    Serial.println("\nConectado com sucesso ao Broker");
}

void loop() {

  String mensagem = "";
  if(Serial.available() > 0) {
    mensagem = Serial.readStringUntil('\n');
    Serial.print("Mensagem digitada: ");
    Serial.println(mensagem);
    mensagem = "Miguel: " + mensagem;
    mqtt.publish(topic.c_str(), mensagem.c_str());
  }

}



