Goomba Save Manager
===================

This repo contains two programs designed to edit the SRAM data of the Goomba and Goomba Color emulators. It can extract and replace the compressed Game Boy / Game Boy Color SRAM data. It can't edit savestate data or add new SRAM (yet).

.exe files are located in the "Release Binaries" folder. Linux / Cygwin users can build the command-line version of the application by using the Makefile or by running:

  gcc -Wall -o goombasav main.c goombasav.c minilzo-2.06/minilzo.c

Make sure you back up your save data before using the application, just in case.

NEW in version 2014-04-30:
* The GTK application was added.
* The "Save As" in the C++/CLI version now works properly.
* The command-line app, when run with one filename argument, now shows a lot more information about the save data.
* All three versions now display a three-byte hash of the compressed data, so you can see at a glance when it gets replaced.

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

Goomba Save Manager (goombasav_gtk)
-----------------------------------

This version of Goomba Save Manager wraps the goombasav.c code in a GTK (2.0) interface, whose design largely mirrors the C++/CLI version.

You can compile it on GNU/Linux (etc.) using the Makefile in the goombasav_gtk folder - note that the files in the directory above it (goombasav.c, minilzo-2.06, and so on) ARE used, and so the best way is to check out the whole directory structure from git, cd to goombasav_gtk, and run make from there.

To run this version in Visual Studio, install the 32-bit all-in-one package (http://www.gtk.org/download/win32.php) to C:\gtk, and set the GTKDIR system environment variable to C:\gtk (in Windows 8, go to This PC -> Properties -> Advanced system settings -> Advanced -> Environment variables, and add a new system variable.)
