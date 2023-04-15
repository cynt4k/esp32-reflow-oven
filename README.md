# ESP32 Reflow oven
Build instructions and source code for my self-build reflow oven.
## Description
For soldering multiple PCBs, I needed a reflow oven which is not pricy and, in the best case, a complete OpenSource one.
Different approaches exist for building a Reflow oven, but I need something to match my needs.
This reflow oven is built to be extended with multiple new features like a Webserver or App control.
At this state, you can do the following things:

- [x] Reflow Profile  
- [x] Advanced PID settings  
- [x] Draw reflow curve  
- [x] Status display  
- [x] Manage four Profile profiles  
- [ ] Bluetooth control  

## Instructions
I used a standard cheap 1200W Pizza Oven from [Amazon](https://www.amazon.de/gp/product/B07FK4DBFB). This model is often sold under different names. You can practically use any oven with this project.

### BOM

|Name|Amount|Link|Price|
|---|---|---|---|
|Pizza Oven|1|https://www.amazon.de/gp/product/B07FK4DBFB|42€|
|Wemos D1 ESP32|1|https://www.ebay.de/itm/144343720569|8€|
|MAX6675|1|https://www.ebay.de/itm/255283310828|7€|
|Nextion 3.5" 480x320 HMI|1|https://www.ebay.de/itm/294993756320|44€|
|SSR-25DA Solid State Module|1|https://www.ebay.de/itm/284795452417|7€|
|Main Switch|1|https://www.ebay.de/itm/131812926875|5€|
|Cables|multiple|hardware store|ca. 10€|
|Screws|multiple|hardware store|ca. 2€|

You could expect it to cost about 100€ in total.

### Build PCB
To build the PCB, you need a breadboard to solder everything together.  
|<img src="doc/pics/pcb_front.jpg" width="200">|<img src="doc/pics/pcb_back.jpg" width="200">|<img src="doc/pics/pcb_schematic.png" width="400">
|:---:|:---:|:---:|
|Front|Back|Schematic|

### Software upload
The program is written with the Arduino SDK and PlatformIO for the embedded platform.
You can compile it by checking out this repo and opening it with VSCode or the preferred IDE.
Then press for, e.g., the VSCode Platformio upload or use the CLI.
```bash
platformio run --target upload
```