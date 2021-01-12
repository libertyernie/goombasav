Goomba Save Manager
===================

This library is designed to edit the SRAM data of the Goomba, Goomba Color, PocketNES, and SMSAdvance emulators, all of which run on the Game Boy Advance. The library can extract and replace the compressed SRAM or savestate data of the game being emulated. It can't add new SRAM.

Windows .exe files for the GUI and command-line apps are located in the Releases section on GitHub. Unix / Linux / Cygwin / WSL users can also build the command-line apps by using the Makefile. See below for information about these apps.

Make sure you back up your save data before using the application, just in case.

goombasav
---------

goombasav is a command-line program that can extract and replace save data, as well as "clean" Goomba / Goomba Color save files. (Sometimes these emulators will store uncompressed save data in the range 0xE000-0xFFFF without compressing it to the normal location; in this case, the "clean" function will compress the data, update the marker in the configdata structure, and clear 0xE000.)

goombasav is written in C. In Visual Studio, use the /TP switch to compile as C++ code.

See "example_usage.txt" for an example of how to use goombasav.

gbaromextract
-------------

gbaromextract is a command-line program that can:

* extract uncompressed Game Boy ROMs from compiled Goomba / Goomba Color ROMs, or any other uncompressed archive file (Game Boy ROMs have a standard header format that makes this possible)
* extract uncompressed NES ROMs from compiled PocketNES ROMs
* extract uncompressed Master System / Game Gear ROMs from compiled SMSAdvance ROMs

It cannot replace any data in the input file.

gbaromextract is written in C. In Visual Studio, use the /TP switch to compile as C++ code.

Goomba Save Manager (goombasav_cs)
----------------------------------

Goomba Save Manager (goombasav_cs.exe, goombasav_net.dll) wraps the backend code in a class-oriented C++/CLI library, and makes that available through a GUI written in C#.

Use the Open and Save/Save As options from the File menu to open Goomba / Goomba Color save files, and use the Extract and Replace buttons to edit the SRAM. If the file needs to be "cleaned" (see above), the program will let you know when you open it. (If you clean the file, the changes won't be saved until you choose Save or Save As.)

You can also drag-and-drop .sav files onto the window to open a GBA save file or replace a GB/GBC save file (the program will figure out what you want to do based on the file.)

Other Applications
------------------

A GTK+ version used to exist, but is no longer supported. Revert commit c3adff7b in the Git repository if you want to bring it back.

Besides the command-line and GUI applications listed above, the following apps also make use of the Goomba Save Manager libraries, allowing you to transparently use Goomba compressed save data.

* [TGB Dual L](https://github.com/libertyernie/tgbdual_L) (Game Boy emulator for Windows)
  * Link cable support and other features
* [Visual Boy Advance GX](https://github.com/dborth/vbagx) (Game Boy emulator for GameCube/Wii)
* [Unofficial NRage plugin fork](https://github.com/libertyernie/nrage-input) (N64 emulator plugin)
  * Allows you to use Goomba save data for Transfer Pak emulation (Pokémon Stadium, etc.)
  * Also requires original GB/GBC ROM
