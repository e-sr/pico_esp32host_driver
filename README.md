
## build slave firmware

This is the frist step to do and is done usnig `esp idf`. Depending on slave mcu and configuration there will be

steps:
1. ```idf.py set-target esp32c6```
2. ```idf.py menuconfig```
   select SPI transport 
   optionally setup SPI, GPIO, ...
3. ```idf.py build```
4. ```idf.py flash```

esp idf generate a `sdkconfig` file and its header file counterpart `sdkconfig.h` under `build/config`.
driver necessitate the headerfile.


##