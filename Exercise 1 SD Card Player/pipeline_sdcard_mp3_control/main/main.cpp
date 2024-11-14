#include "audio.cpp"
#include "wifi.cpp"

extern "C" void app_main(void)
{
    Wifi::init();
    Audio::init();
}
