Liquid Impulse
==============

Liquid Impulse is an open source SDL game. It was created for an Academic project and is made under the Mozilla Public License 2.0 (MPL-2.0).  
Game runs on Linux, Windows (XP at least) and PSP system.


### What is Liquid Impulse ?
The game is a Beat Them All, with references to oldies that defined the genre. Our goal is to make a game with the good old spirit but using the modern computing power. What would you say if you could beat 40 guys with the same punch in Streets of Rage, sending them in the air then jumping and keeping hitting them before the final knockdown-in-air that made explode the floor and send up the 20 others guys that were waiting you so you can keep hitting the 60 bastards  ?



### Heyyy ok I want to play !
Me too. But right now the game is still in the first stage of his life. But you can help us and download any of the concept/alpha builds. You can download the last build here: http://liquidimpulse.codeplex.com/releases/view/101199 (Linux, Windows and PSP). Put please note that the game is not finished yet and there is bugs.

### I want to build it from the sources !
##### On Linux:
>mkdir build  
>cd build  
>cmake ../.  
>make  

Then copy the binary compiled into the parent folder and run it ! If you launch it from there it wont find the Resource folder and crash.

##### Using Visual Studio:  
Open the .sln, then right click on the project and "build".  
Then go to the bin/Debug folder and copy all the .dll on the sources/DLL folder into it and the Resources folder too. You can now run the game running the binary.

##### Using MinGW:  
No one did it yet. It should be as easy as the Linux build if you are using CMake. If you use MinGW you probably know how to do it !

##### Building for the PSP:  
>mv Makefile.psp Makefile  
>make  

### What tools you used to make this game ?
Langage used are C++ and C, and the libraries are SDL, SDL_ttf, SDL_mixer, SDL_image and minIni.  
To develop, we use Visual Studio 2010/2012, Qt Creator or a text editor with a regular Linux build system. We use the psp toolchain and make to compile the game on the PSP handheld.

   

### Credits
Music is from Danjyon Kimura: http://www.jamendo.com/fr/artist/368781/danjyon-kimura  
Sprites are from http://spritedatabase.net/game/849 and is a copy of KOF2003. (Copyright SNK)  
The ingame Backgrounds are made by CardCaptor Schuleter on http://www.bghq.com/bl.php?contrib=ccs  
Fonts are from Ænigma on dafont.com  
The background of the main menu is from the Streets of Rage game. (Copyright SEGA)  
Thanks to G.M Spectre for his sprites that helped us a lot.

See credits.txt file for complete credits list.
