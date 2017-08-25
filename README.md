Goomba Save Manager
===================

This library is designed to edit the SRAM data of the PocketNES, Goomba, and Goomba Color emulators. It can extract and replace the compressed NES / Game Boy / Game Boy Color SRAM data. It can't edit savestate data or add new SRAM.

Windows .exe files for the GUI and command-line apps (which support Goomba and Goomba Color only, not PocketNES) are located in the Releases section on GitHub. Unix / Linux / Cygwin users can build the command-line or GTK+ version of the application by using the Makefile. See below for information about these apps.

Make sure you back up your save data before using the application, just in case.

goombasav
---------

goombasav is a command-line program that can extract and replace save data, as well as "clean" Goomba / Goomba Color save files. (Sometimes these emulators will store uncompressed save data in the range 0xE000-0xFFFF without compressing it to the normal location; in this case, the "clean" function will compress the data, update the marker in the configdata structure, and clear 0xE000.)

goombasav is written in C and is also valid C++. In Visual Studio, use the /TP switch to compile .c files as C++ code.

See "example_usage.txt" for an example of how to use goombasav.

gbaromextract
-------------

gbaromextract is a command-line program that can extract NES ROMs from compiled PocketNES ROMs and Game Boy ROMs from compiled Goomba / Goomba Color ROMs. It works similarily to goombasav, but cannot replace any data.

gbaromextract can also extract Game Boy ROMs from any uncompressed source (e.g. .tar files); however, this is not true of NES ROMs.

gbaromextract is written in C and is also valid C++. In Visual Studio, use the /TP switch to compile .c files as C++ code.

Goomba Save Manager (goombasav_cs)
----------------------------------

Goomba Save Manager (goombasav_cs.exe, goombasav_net.dll) wraps the backend code in a class-oriented C++/CLI library, and makes that available through a GUI written in C#. The binary is built with Visual Studio 2015; it hasn't been tested on Windows XP/Vista recently but might work.

Use the Open and Save/Save As options from the File menu to open Goomba / Goomba Color save files, and use the Extract and Replace buttons to edit the SRAM. If the file needs to be "cleaned" (see above), the program will let you know when you open it. (If you clean the file, the changes won't be saved until you choose Save or Save As.)

You can also drag-and-drop .sav files onto the window to open a GBA save file or replace a GB/GBC save file (the program will figure out what you want to do based on the file.)

Goomba Save Manager (goombasav_gtk)
-----------------------------------

This version of Goomba Save Manager wraps the backend code in a GTK interface, whose design largely mirrors the C# version. It uses GTK 2 but will also work with GTK 3 if you change the Makefile.

You can compile it on GNU/Linux (etc.) using the Makefile in the goombasav_gtk folder - note that the files in the directory above it (goombasav.c, minilzo, and so on) ARE used, and so the best way is to check out the whole directory structure from git, cd to goombasav_gtk, and run make from there.

Other Applications
------------------

Besides the command-line and GUI applications listed above, the following apps also make use of the Goomba Save Manager libraries, allowing you to transparently use Goomba compressed save data.

* [TGB Dual L](https://github.com/libertyernie/tgbdual_L) (Game Boy emulator for Windows)
  * Link cable support and other features
* [Visual Boy Advance GX](https://github.com/dborth/vbagx) (Game Boy emulator for GameCube/Wii)
* [Unofficial NRage plugin fork](https://github.com/libertyernie/nrage-input) (N64 emulator plugin)
  * Allows you to use Goomba save data for Transfer Pak emulation (Pokémon Stadium, etc.)
  * Also requires original GB/GBC ROM
