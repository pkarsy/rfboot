* Arduino and rfboot

Generally speaking a bootloader is code agnostic. You can upload any firmware written in C or assembler or Arduino (Which uses the c++ compiler).

Arduino is a first class citizen. The arduino IDE is not used however. Instead the excelllent arduino-makefile is used, witch gives us the power to use any editor. The examples in this site use the geany editor, but obvioulsy you can use another.

"rftool create ProjectName" creates a new arduino project with unique RF channel and SyncWord (To prevent RF collitions with other modules) and a unique XTEA key. All this customization is saved in the app_settings.h file inside the project's folder.

A "make xtal" burns the bootloader to the target MCU. This is done once.

From now on a "make send" sents the code to the module wirelesly. Most editors can be customized to execute such commands with a click (See Video). Much more details on this topic in The first projectNOT READY