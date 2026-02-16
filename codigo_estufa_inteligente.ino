#include <DHT.h>

// Definições do sensor DHT
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Pinos de controlo
int rele = 2;
int humificador = 3;
const int pinClick = 8;

// Valores desejados
float humidade_desejada = 90.0;

// Estados de controlo
bool rele_ligado = false;
bool controlo_humidade_ativa = true;
// bool primeiroCiclo = true;

// Temporizadores
unsigned long tempoAnteriorLeitura = 0;
const unsigned long INTERVALO_LEITURA = 2000; // 2 segundos

unsigned long tempoAnteriorRele = 0;
const unsigned long INTERVALO_RELE = 60000; // 60 segundos

unsigned long ultimoCliqueHumificador = 0;
unsigned long INTERVALO_CLIQUE = 14520000; // começa com 4h e 1min

const int CLICK_DURATION = 300; // ms

void setup() {
  Serial.begin(9600);
  
  pinMode(rele, OUTPUT);
  pinMode(humificador, OUTPUT);
  pinMode(pinClick, OUTPUT);

  digitalWrite(rele, LOW); // Relé ligado inicialmente
  analogWrite(humificador, 255); // Humificador ligado no máximo

  // Clique inicial para ligar humificador
  digitalWrite(pinClick, HIGH);
  delay(CLICK_DURATION);
  digitalWrite(pinClick, LOW);
  
  ultimoCliqueHumificador = millis(); // começa a contar a partir daqui

  dht.begin();
}

void loop() {
  unsigned long agora = millis();

  // Leitura dos sensores a cada 2 segundos
  if (agora - tempoAnteriorLeitura >= INTERVALO_LEITURA) {
    tempoAnteriorLeitura = agora;
    
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Falha ao ler do DHT!");
      return;
    }

    // Envia dados para o monitor serial
    String dados = "Temperatura: " + String(temperature, 1) + "°C | Humidade: " + String(humidity, 1) + "%";
    Serial.println(dados);

    // Controlo da humidade
    if (humidity > humidade_desejada && controlo_humidade_ativa) {
      analogWrite(humificador, 1); // Humificador no mínimo
      controlo_humidade_ativa = false;
      Serial.println("Humidade alta - reduzindo humificador");
    } else if (humidity < humidade_desejada && !controlo_humidade_ativa) {
      analogWrite(humificador, 255); // Humificador no máximo
      controlo_humidade_ativa = true;
      Serial.println("Humidade baixa - ligando humificador");
    }
  }

  // Controlo do relé a cada 60 segundos
  if (agora - tempoAnteriorRele >= INTERVALO_RELE) {
    tempoAnteriorRele = agora;
    
    rele_ligado = !rele_ligado;
    digitalWrite(rele, rele_ligado ? HIGH : LOW);
    
    Serial.println(rele_ligado ? "Relé DESLIGADO" : "Relé LIGADO");
  }

  
  if (agora - ultimoCliqueHumificador >= INTERVALO_CLIQUE) {
    // Faz clique
    digitalWrite(pinClick, HIGH);
    delay(CLICK_DURATION);
    digitalWrite(pinClick, LOW);
    ultimoCliqueHumificador = agora;
    
    Serial.println("Clique no humificador executado");
  }
}