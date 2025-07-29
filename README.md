Prejeto para ESP32 de leitura de temperatura.

- Conexão ao WIFI
- Leitura de temperatura com MAX6675
- Página da Web com atualização do valor em tempo real (AJAX)
- Gravação em cartão SD, contendo data, hora e temperatura de cada leitura

Pinout do ESP32 para o MAX6675 e o módulo SD Card:

- VIN: VCC do MAX6675 e VCC do módulo SD Card
- GND: GND do do MAX6675 e VCC do módulo SD Card
-------------------------------------------------
- GPIO19: MISO do módulo SD Card
- GPIO23: MOSI do módulo SD Card
- GPIO18: SCK do módulo SD Card
- GPIO5: CS do módulo SD Card
-------------------------------------------------
- GPIO21: SCK do MAX6675
- GPIO4: CS do MAX6675
- GPIO22: SO do MAX6675