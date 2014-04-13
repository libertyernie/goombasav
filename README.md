Goomba Save Manager
===================

This repo contains two programs designed to edit the SRAM data of the Goomba and Goomba Color emulators. It can extract and replace the compressed Game Boy / Game Boy Color SRAM data. It can't edit savestate data (yet).

Make sure you back up your save data first, just in case.

goombasav
---------

goombasav is a command-line program that can extract and replace save data, as well as "clean" Goomba / Goomba Color save files. (Sometimes these emulators will store uncompressed save data in the range 0xE000-0xFFFF and not compress it to the normal location; in this case, the "clean" function will compress the data, update the marker in the configdata structure, and clear 0xE000.)

goombasav is written in C and compiles in gcc and Visual Studio (although for the latter you have to compile it as C++ code.)

Usage:

	goombasav c PokemonYellow.sav *(reads PokemonYellow.sav, cleans it, and writes to the same file)*

	goombasav c PokemonYellow.sav cleaned.sav *(reads PokemonYellow.sav, cleans it, and writes to cleaned.sav)*

	goombasav x PokemonYellow.sav yellow_gbc.sav *(extract save file)*
	  0. SRAM: POKEMON YELLOW (2312 b / 32768 uncomp)
	  1. Configuration: CFG (48 b / 4196096 uncomp)
	  Extract:
    *(enter 0 and press Enter)*
	  size: 2312
	  type: SRAM (1)
	  uncompressed_size: 32768
	  framecount: 0
	  checksum: BE706244
	  title: POKEMON YELLOW
	  Actual uncompressed size: 32768

	goombasav r PokemonYellow.sav yellow_gbc.sav *(replace save file)*
	  0. SRAM: POKEMON YELLOW (2312 b / 32768 uncomp)
	  1. Configuration: CFG (48 b / 4196096 uncomp)
	  Replace:
    *(enter 0 and press Enter)*
	  size: 2312
	  type: SRAM (1)
	  uncompressed_size: 32768
	  framecount: 0
	  checksum: BE706244
	  title: POKEMON YELLOW
	  Compressed 32768 bytes (compressed size: 2271)

Goomba Save Manager (goombasav_clr)
-----------------------------------

Goomba Save Manager (goombasav_clr.exe) wraps the backend code (goombasav.c) in a Windows Forms interface with C++/CLI. It runs on Windows XP and newer, uses .NET Framework 4 and requires the Visual C++ 2013 redistributable packages.

Use the Open and Save/Save As options from the File menu to open Goomba / Goomba Color save files, and use the Extract and Replace buttons to edit the SRAM. If the file needs to be "cleaned" (see above), the program will let you know when you open it. (If you clean the file, the changes won't be saved until you choose Save or Save As.)
