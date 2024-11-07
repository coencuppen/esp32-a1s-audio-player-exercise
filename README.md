# esp32-a1s-audio-player-exercise

## Environment set up 
### Install ESP-IDF v5.3

https://docs.espressif.com/projects/esp-adf/en/latest/get-started/index.html#get-started-setup-esp-idf

1. inside Step 1 click ont the ```ESP32``` link
2. scroll down and click on the ```Windows Installer```
3. click on ```Windows Installer Download```
4. select the ```Uiversal Online Installer```
5. run the installer
6. select ```Download ESP-IDF```
7. select ```v5.3.1```
8. leave the default install folder as is.
9. select ```full installation```
10. now hope that the internet in the office is fast enough (it's a 50/50 change)

### install ESP-ADF

Now do step 2 of the guide
https://docs.espressif.com/projects/esp-adf/en/latest/get-started/index.html#step-2-get-esp-adf

if you don't have ```git``` installed and can't run:
```bash 
git clone --recursive https://github.com/espressif/esp-adf.git
```
1. download a .zip from https://github.com/espressif/esp-adf.git by clicking on ```< > code```
2. go to your homepath ```C:\Users\yourname\```
3. create folder ```esp```
4. place the unzipped ```esp-adf-master``` folder inside ```esp```
5. rename ```esp-adf-master``` to ```esp-adf```

Follow step 3 of the guide
https://docs.espressif.com/projects/esp-adf/en/latest/get-started/index.html#step-3-set-up-the-environment

(run the ```export.bat``` within the esp-idf terminal)
 
### Adding the custom ESP-32-A1S audio board to adf

1. clone or download the .zip https://github.com/trombik/esp-adf-component-ai-thinker-esp32-a1s
2. put the ```esp-adf-component-ai-thinker-esp32-a1s``` folder inside ```esp\esp-adf\components\audio_board```
3. next step will be inside the exercise instructions. good luck!
