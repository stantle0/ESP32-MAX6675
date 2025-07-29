//IMPORTANTE: ANTES DE EXECUTAR, DEFINIR O WIFI E SENHA EM:
//#define MEU_SSID "SSID_WIFI"
//#define MINHA_SENHA "SENHA_WIFI"

#include <max6675.h>
#include <WiFi.h>
#include <WebServer.h>
#include <SD.h>
#include <time.h>
#include "PaginaInicial.h"
#include "SenhasEKeys.h"


//****************************************
//CONTROLE ESP32
#define LED_INTERNO_GRAVACAO 2 //LED apagado indica ESP32 ocupado gravando no SD e não deve ser desligado

//****************************************
//Dados para MAX6675
#define MAX_SCK 21
#define MAX_CS 4
#define MAX_SO 22
MAX6675 thermopar(MAX_SCK,MAX_CS,MAX_SO);
float temperaturaAtual;

//****************************************
//Dados para MODULO SD
#define MICRO_SD_CS 5 //Outras portas utilizdas: 18,19,23
#define DELAY_PARA_ESCRITA_SD 30000
File arquivoSD;
String nomeArquivo = "/temperaturas.csv";
unsigned long tempoUltimaGravacao;

//****************************************
//Dados para conexão com a INTERNET
WebServer server(80);

//****************************************
//Dados para obternção de horário via NTP Server
const char* ntpServer = "pool.ntp.org";
const long fusoGmt_Segundos = -4 * 3600; // fuso horário de -4 em segundos
const int horarioVerao_Segundos = 0; // não utilizar horário de verão
time_t horarioBase; //timestamp atual desde 1970 como base
unsigned long milissegundosBase; //marca o tempo base desde que o ESP32 sincronizou



void setup() {
  //criando LED para controle de codigo
  pinMode(LED_INTERNO_GRAVACAO,OUTPUT);
  digitalWrite(LED_INTERNO_GRAVACAO,HIGH);
  
  Serial.begin(115200);
  delay(500);

  //Conexão WiFi
  Serial.println("Conectando");
  WiFi.begin(MEU_SSID,MINHA_SENHA);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
  }
  Serial.print("Conectado no IP ");
  Serial.println(WiFi.localIP());
  
  //Estabelecendo as chamadas e as funções para lidar com elas
  server.on("/", handlePaginaInicial);
  server.on("/atualiza_temperatura", handleAtualizaTemperatura);

  //Inicia o servidor
  server.begin();

  //Inicializando o SD
  Serial.println("Inicializando o SD");
  if (!SD.begin(MICRO_SD_CS)) {
    Serial.println("Falha ao inicializar o SD");
    while(1);
  }
  Serial.println("SD inicializado!");
  
  //Inicializando temporizador para gravar em SD
  tempoUltimaGravacao = millis();

  //Sincronização com o servidor NTP
  configTime(fusoGmt_Segundos, horarioVerao_Segundos, ntpServer);
  Serial.println("Inicio da sincronização de horario");
  struct tm tempoNTP;
  //sincronização pode demorar, então esse while se certifica que sincronizou
  while (!getLocalTime(&tempoNTP)) {
    Serial.println("Aguardando NTP server");
    delay(200);
  }
  horarioBase = time(NULL); //segundos desde 1970, conforme a sincronizacao com sucesso
  milissegundosBase = millis(); //tempo de início da sync do ESP32 em milissegundos
  Serial.println("Horario obtido com sucesso");
}

void loop() {
  //************************************
  //ESCRITA EM ARQUIVO
  if (millis() - tempoUltimaGravacao > DELAY_PARA_ESCRITA_SD) {
    Serial.println("Gravando no arquivo");
    tempoUltimaGravacao = millis();

    //preparando string com data e hora
    unsigned long segundosAposSyncInicial = (millis() - milissegundosBase) / 1000;
    time_t tempoParaGravacaoEmSegundos = horarioBase + segundosAposSyncInicial;
    struct tm *horaAtualParaGravacao = localtime(&tempoParaGravacaoEmSegundos); //converte os segundos desde 1970 para struct tm
    char stringDataHora[30];
    strftime(stringDataHora, sizeof(stringDataHora), "%d/%m/%Y %H:%M:%S", horaAtualParaGravacao);
    //abrindo aquivo
    Serial.println("Abrindo arquivo");
    digitalWrite(LED_INTERNO_GRAVACAO,LOW); //indica que o ESP32 está ocupado e nao deve ser desligado
    arquivoSD = SD.open(nomeArquivo, FILE_WRITE);
    if (arquivoSD) {
      arquivoSD.seek(arquivoSD.size());
      arquivoSD.println(String(stringDataHora) + ", " + String(thermopar.readCelsius()));
      Serial.println(String(stringDataHora) + ", " + String(thermopar.readCelsius()));
      arquivoSD.close();
      digitalWrite(LED_INTERNO_GRAVACAO,HIGH);
      Serial.println("Gravação concluída");
    }
    else {
      Serial.println("Falha ao abrir o arquivo.");
      while (1);
    }
  }

  //**************************************************
  //RESPONDER CLIENTES DO SERVIDOR
  server.handleClient();
}

//****************************************************
//HANDLERS DOS PEDIDOS DOS CLIENTES
void handlePaginaInicial() {
  server.send(200,"text/html", PAGINA_INICIAL);
}

void handleAtualizaTemperatura() {
  temperaturaAtual = thermopar.readCelsius();
  Serial.println("Enviando temperatura atualizada: " + String(temperaturaAtual));
  //Serial.println(temperaturaAtual);
  server.send(200,"text/plain",String(temperaturaAtual));
}