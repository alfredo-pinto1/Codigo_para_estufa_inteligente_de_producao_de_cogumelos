import serial
import re
import time
import subprocess
from datetime import datetime

# === CONFIGURAÇÕES ===
PORTA = '/dev/ttyUSB0'
BAUD = 9600
ARQUIVO = 'dados_arduino.txt'
SENHA_SUDO = "**********" 

# === Função para sincronizar o relógio ===
def sincronizar_tempo():
    print("[INFO] A sincronizar hora do sistema...")
    try:
        subprocess.run(
            ["sudo", "-S", "ntpdate", "time.nist.gov"],
            input=(SENHA_SUDO + "\n").encode(),
            check=True
        )
        print("[INFO] Hora sincronizada com sucesso.")
    except subprocess.CalledProcessError as e:
        print(f"[ERRO] Falha ao sincronizar a hora: {e}")

# === Regex para extrair os dados do Arduino ===
regex = r"Temperatura:\s*(-?\d+\.\d+).*Humidade:\s*(\d+\.\d+)"

# === Início ===
sincronizar_tempo()

try:
    ser = serial.Serial(PORTA, BAUD, timeout=2)
    time.sleep(2)
    print(f"[INFO] Conectado à porta {PORTA} @ {BAUD} baud.\n")
except serial.SerialException as e:
    print(f"[ERRO] Não foi possível abrir a porta serial: {e}")
    exit(1)

with open(ARQUIVO, 'w') as f:
    f.write("DataHora\tTemperatura(C)\tHumidade(%)\n")
    print(f"[INFO] Salvando dados em '{ARQUIVO}' (tab separado)\n")
    print("DataHora\t\t\tTemperatura(C)\tHumidade(%)")

    while True:
        try:
            linha = ser.readline().decode('utf-8').strip()

            match = re.search(regex, linha)
            if match:
                temperatura = match.group(1)
                humidade = match.group(2)
                agora = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
                linha_formatada = f"{agora}\t{temperatura}\t{humidade}"

                print(linha_formatada)
                f.write(linha_formatada + '\n')
            else:
                print(f"[IGNORADO] {linha}")

        except KeyboardInterrupt:
            print("\n[INFO] Leitura interrompida pelo usuário.")
            break
        except Exception as e:
            print(f"[ERRO] {e}")
