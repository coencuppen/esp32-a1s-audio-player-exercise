# esp32-a1s-audio-player-exercise

## Environment set up 
1. install ESP-IDF v5.3

https://docs.espressif.com/projects/esp-adf/en/latest/get-started/index.html#get-started-setup-esp-idf

2. install ESP-ADF v2.6
   
```bash 
git clone --recursive https://github.com/espressif/esp-adf.git
```
   
### Adding the custom ESP-32-A1S audio board to adf

1. clone ```https://github.com/trombik/esp-adf-component-ai-thinker-esp32-a1s```
2. put the ```esp-adf-component-ai-thinker-esp32-a1s``` folder inside ```esp\esp-adf\components\audio_board```
