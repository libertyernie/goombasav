Goomba Save Manager
===================

This library is designed to edit the SRAM data of the Goomba and Goomba Color emulators. It can extract and replace the compressed Game Boy / Game Boy Color SRAM data. It can't edit savestate data or add new SRAM.

Windows .exe files for the .NET and command-line apps are located in the "Release Binaries" folder. Linux / Cygwin users can build the command-line version of the application by using the Makefile or by running:

  gcc -Wall -o goombasav main.c goombasav.c minilzo/minilzo.c

Also, the directory "goombasav_gtk" has a GTK+ application you can compile and run, which works just like the .NET app and comes with its own Makefile.

Make sure you back up your save data before using the application, just in case.

Work on adding PocketNES and maybe SMSAdvance support is in progress.

goombasav
---------

goombasav is a command-line program that can extract and replace save data, as well as "clean" Goomba / Goomba Color save files. (Sometimes these emulators will store uncompressed save data in the range 0xE000-0xFFFF and not compress it to the normal location; in this case, the "clean" function will compress the data, update the marker in the configdata structure, and clear 0xE000.)

goombasav is written in C and is also valid C++. In Visual Studio, use the /TP switch to compile .c files as C++ code.

See "example_usage.txt" for an example of how to use goombasav.

Goomba Save Manager (goombasav_cs)
----------------------------------

Goomba Save Manager (goombasav_cs.exe, goombasav_net.dll) wraps the backend code in a class-oriented C++/CLI library, and makes that available through a GUI written in C#. The binary is built with Visual Studio 2015; it hasn't been tested on Windows XP/Vista recently but might work.

Use the Open and Save/Save As options from the File menu to open Goomba / Goomba Color save files, and use the Extract and Replace buttons to edit the SRAM. If the file needs to be "cleaned" (see above), the program will let you know when you open it. (If you clean the file, the changes won't be saved until you choose Save or Save As.)

You can also drag-and-drop .sav files onto the window to open a GBA save file or replace a GB/GBC save file (the program will figure out what you want to do based on the file.)

Goomba Save Manager (goombasav_gtk)
-----------------------------------

This version of Goomba Save Manager wraps the backend code in a GTK interface, whose design largely mirrors the C# version. It uses GTK 2 but will also work with GTK 3 if you change the Makefile.

You can compile it on GNU/Linux (etc.) using the Makefile in the goombasav_gtk folder - note that the files in the directory above it (goombasav.c, minilzo, and so on) ARE used, and so the best way is to check out the whole directory structure from git, cd to goombasav_gtk, and run make from there.

To run this version in Visual Studio on Windows, you can get the GTK stack from https://github.com/hexchat/gtk-win32 (click the "32-bit" link under GTK+ Bundle), extract it to C:\gtk, and then rename libintl.lib to intl.lib. After building, you'll need to copy the .dll files in bin to the "Debug" folder that the built .exe ends up in.
