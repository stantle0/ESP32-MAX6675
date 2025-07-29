Projeto para ESP32 de leitura de temperatura e gravação em cartão micro SD.

- Conexão ao WIFI
- Leitura de temperatura com MAX6675
- Página da Web com atualização do valor em tempo real (Obter IP do ESP32 impresso na saída Serial)
- Gravação em cartão SD, contendo data, hora e temperatura de cada leitura
- LED do GPIO2 ficará ligado o tempo todo, exceto quando o ESP32 estiver ocupado gravando no SD

Pinout do ESP32:

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