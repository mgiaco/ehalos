## eHalOS Operating System ##
**eHalOS** was created 2008/09 on a ATMLE AVR32 AP7000 NGW100 embedded board (with self build-on TFT). It comes with 3 demo applications (shell, space invaders, digital picture frame show). Its architecture allowes to port eHalOS onto any other embedded system like AVR32 UC3 or ARM based embedded boards. Through its LGPL license it is also possible to use it a wide range of projects.

[![](http://ehalos.googlecode.com/svn/trunk/www_stuff/pictures/atmel_ap7000_ngw100.png)](http://www.atmel.com/dyn/products/tools_card.asp?tool_id=4102)

[![](http://ehalos.googlecode.com/svn/trunk/www_stuff/pictures/demo_halos_2_thumb.jpg)](http://www.atmel.com/dyn/products/tools_card.asp?tool_id=4102)

## eHalOS Architecture ##
eHalOS was designed as a monolitic kernel (95% written in language C) for quite fast and smart kernel execution and has been devided in several modules like
  * HAL (hardware abstraction layer)
  * Virtual Memory Manager (MMU memory management)
  * Scheduler (task scheduling, round robing algorithm)
  * Ressource Manager (handels IO devices and there access from the usermode/applications)
  * USART Device Driver
  * GDI support (for small LCD/TFT displays)
  * Kernel API (halos\_devide + gdi + system\_api for Applications which want access the OS kernel)

The applications can be started over the FLASH device via the Loader-Module. eHalOS comes with a Idle process, a shell process with basic commands, the old good Space-Invaders game and a small digital picture frame application for demo and testing purpose. As you can see in the architecture overview below, some parts are full coloured and some are gray. The gray modules of eHalOS have not been implemented yet.

![![](http://ehalos.googlecode.com/svn/trunk/www_stuff/pictures/halos_architecture_april2009_thumb.png)](http://ehalos.googlecode.com/svn/trunk/www_stuff/pictures/halos_architecture_april2009_big.png)


## eHalOS Team ##
This project was initiated by six developers at the University of Applied Sciences Vorarlberg, Austria (www.fhv.at) and the goal will be to continue on eHalOS and also to share our hard work with all people who are interested in eHalOS. The Team:
  * Brändle Christian {Christian.Braendle AATT gmx.at}
  * Giacomuzzi Mathias {ecam AATT aon.at}
  * Jung Andreas {andreas.jung AATT students.fhv.at}
  * Mayr Andreas {andreas.mayr AATT cable.vol.at}
  * Speckle Markus {maex.sp AATT gmail.com}
  * Zerlauth Karl Sebastian {karl.zerlauth AATT students.fhv.at}

Master IT-Students (ITM07), FH Vorarlberg, University of Applied Sciences, Dornbirn, Austria (www.fhv.at), Coach FH Vorarlberg: Simma Armin {armin.simma AATT fhv.at}

Big thanks to www.mikrocontroller.net und www.avrfreaks.com for some code and help.

Feel free to join our project, for other supporters --> we search sponsors for ATMEL AVRMKII Jtag (development tool for On-chip Debugging).

Team eHalos, 2009


Build complete for project eHalOS

Time consumed: 28344  ms

:)