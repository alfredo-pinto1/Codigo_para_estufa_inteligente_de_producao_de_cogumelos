[README.md](https://github.com/user-attachments/files/25349642/README.md)
# Códigos de Controlo e Monitorização de Humidade e Temperatura para uma Estufa Inteligente

## Códigos do repositório
- [`codigo_estufa_inteligente.ino`](codigo_estufa_inteligente.ino) - Código para o Arduino
- [`ler_arduino.py`](ler_arduino.py) - Script Python para a leitura dos dados do Arduino, pelo PC (Linux)

## 🔧 Hardware Principal Utilizado
- Ventoinha
- Arduino
- Sensor DHT22
- Relé
- Humidificador

## Resumo do Funcionamento do Circuito
- **Pino 2** → Está ligado a um relé que permite abrir e fechar um circuito externo que alimenta uma ventoinha, permitindo trocas de ar periódicas
- **Pino 3** → Está ligado a um transístor que permite amplificar o sinal do Arduino através de um circuito externo, de forma a alimentar a placa do humidificador
- **Pino 4** → Está ligado ao sensor DHT22, permite receber os valores de humidade e temperatura
- **Pino 8** → Está ligado a um transístor soldado ao botão da placa do humidificador, permite acionar o funcionamento da placa remotamente

## 🔌 Informações Adicionais
Para informações detalhadas sobre o circuito, materiais e funcionamento da estufa, consulta o [Instructables](https://www.instructables.com/Estufa-Automática-Para-Produzir-Cogumelos/).

## 📋 Funcionalidades

### Código do Arduino
- Leitura de temperatura e humidade a cada 2 segundos
- Controlo automático do humidificador baseado na humidade desejada
- Alternância do relé a cada 60 segundos (liga e desliga a ventoinha)

### Código de Leitura
- Registo de dados em ficheiro de texto com timestamp
- Sincronização automática de hora via NTP

## 📊 Formato dos Dados
Os dados são guardados em `dados_arduino.txt` com formato tab-separated:
```
DataHora            Temperatura(C)  Humidade(%)
2026-02-15 14:30:00    23.5           88.2
```

## 📦 Dependências

### Arduino
**Instalação no Arduino IDE:**
1. Sketch → Include Library → Manage Libraries
2. Procura "DHT sensor library". Instala a versão da Adafruit

### Python
```bash
pip install -r requirements.txt
```

## 📝 Licença
Este projeto está sob a licença [MIT](LICENSE)
