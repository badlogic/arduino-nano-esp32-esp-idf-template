#include "Arduino.h"

extern "C" void app_main(void) {
    initArduino();

    printf("Internal Total heap %d, internal Free Heap %d\n", (int)ESP.getHeapSize(), (int)ESP.getFreeHeap());
    printf("SPIRam Total heap %d, SPIRam Free Heap %d\n", (int)ESP.getPsramSize(), (int)ESP.getFreePsram());
    printf("ChipRevision %d, Cpu Freq %d, SDK Version %s\n", (int)ESP.getChipRevision(), (int)ESP.getCpuFreqMHz(), ESP.getSdkVersion());
    printf("Flash Size %d, Flash Speed %d\n", (int)ESP.getFlashChipSize(), (int)ESP.getFlashChipSpeed());

    int j = 0;
    while (true) {
        delay(100);
        for (int i = 0; i < 5; i++) {
            j += i;
        }
        printf("Hello world! %i\n", j);
        delay(100);
    }
}
