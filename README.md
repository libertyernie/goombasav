Goomba Save Manager
===================

This repo contains three programs designed to edit the SRAM data of the Goomba and Goomba Color emulators. It can extract and replace the compressed Game Boy / Game Boy Color SRAM data. It can't edit savestate data or add new SRAM (yet).

Windows .exe files for the .NET and command-line apps are located in the "Release Binaries" folder. Linux / Cygwin users can build the command-line version of the application by using the Makefile or by running:

  gcc -Wall -o goombasav main.c goombasav.c minilzo-2.06/minilzo.c

Also, the directory "goombasav_gtk" has a GTK+ application you can compile and run, which works just like the .NET app and comes with its own Makefile.

Make sure you back up your save data before using the application, just in case.

NEW in version 2014-05-13:
* The C++/CLI application was replaced with a C# app and a separate C++/CLI library (dll file). This library can be re-used in any C# or VB.NET app.
* The command-line version of goombasav is now usable on big-endian processors. I was hoping to embed it in a Game Boy emulator for the Wii, but I couldn't get the emulator to work correctly after recompiling. I guess I'll stick with the Game Boy Player on my GCN.
  * The GTK+ app does NOT work on big-endian (although it could be altered to do so.)

goombasav
---------

goombasav is a command-line program that can extract and replace save data, as well as "clean" Goomba / Goomba Color save files. (Sometimes these emulators will store uncompressed save data in the range 0xE000-0xFFFF and not compress it to the normal location; in this case, the "clean" function will compress the data, update the marker in the configdata structure, and clear 0xE000.)

goombasav is written in C and is also valid C++. In Visual Studio, use the /TP switch to compile .c files as C++ code.

See "example_usage.txt" for an example of how to use goombasav.

Goomba Save Manager (goombasav_cs)
-----------------------------------

Goomba Save Manager (goombasav_cs.exe, goombasav_net.dll) wraps the backend code in a class-oriented C++/CLI library, and makes that available through a GUI written in C#. It runs on Windows XP and newer, uses .NET Framework 4 and requires the Visual C++ 2013 redistributable packages.

Use the Open and Save/Save As options from the File menu to open Goomba / Goomba Color save files, and use the Extract and Replace buttons to edit the SRAM. If the file needs to be "cleaned" (see above), the program will let you know when you open it. (If you clean the file, the changes won't be saved until you choose Save or Save As.)

You can also drag-and-drop .sav files onto the window to open a GBA save file or replace a GB/GBC save file (the program will figure out what you want to do based on the file.)

Goomba Save Manager (goombasav_gtk)
-----------------------------------

This version of Goomba Save Manager wraps the goombasav.c code in a GTK (2.0) interface, whose design largely mirrors the C# version.

You can compile it on GNU/Linux (etc.) using the Makefile in the goombasav_gtk folder - note that the files in the directory above it (goombasav.c, minilzo-2.06, and so on) ARE used, and so the best way is to check out the whole directory structure from git, cd to goombasav_gtk, and run make from there.

To run this version in Visual Studio on Windows, install the 32-bit all-in-one package (http://www.gtk.org/download/win32.php) to C:\gtk, and set the GTKDIR system environment variable to C:\gtk (in Windows 8, go to This PC -> Properties -> Advanced system settings -> Advanced -> Environment variables, and add a new system variable.)
