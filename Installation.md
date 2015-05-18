## Installation of eHalOS System and Applications for eHalOS ##

This Installation Guide shows how to install eHalOS on the AVR NGW100 embedded board and also how to build and install the Applications. In order to run eHalOS on the NGW100 board the following steps have be done:

  1. Start AVR32 Studio and make sure that the JTAGICE mkII is recognized by AVR32 Studio
  1. Setup the JTAGICE mkII device in AVR32 Studio (first do "scan targets" and setup to the CPU: AP7000 and to the board: NGW100)
  1. Open and Execute (run) the eHalOS Project on the AVR32 Studio (this will burn the eHalOS on the board)
  1. Burn the eHalOS applications to the Flash with the following parameters:
    * IdleProcess -> File: IdleProcess.bin, Start Address: 0x00500000
    * Shell: -> File: Shell.bin, Start Address: 0x00520000
    * SpaceInvaders: -> File: SpaceInvaders.bin, Start Address: 0x00540000
    * ImageShow: -> File: PhotoFrame.bin, Start Address: 0x00560000

**Note:** The Applications have to be build with the eHalOS API´s (see folder: .../Halos\_Development/src/halos\_apis) and have avterwards to be converted from the standard **ELF-Fileformat to a BINARY-Fileformat**. For converting just run on the console the file "convert\_elf\_to\_bin\_fileformat.bat". This does the converting for you. After that you can find the .bin file in the debug folder of the application.

Now the eHalOS System is ready to run and also ready to debug. Just restart the NGW100 by the reset button or power plug and be sure that the NGW100 by its serial port to the serial port of any computer with a running terminal console (E.g. TeraTerm, for serial port settings see below ).