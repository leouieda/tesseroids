TESSEROID GRAVITY FIELDS

The tess* and divmod aplications were written in C++ and compiled with g++ on a WindowsXP.
Run the Makefile to compile all the programs and put the .exes in the bin folder.

Description:
	The programs are all made to be executed via command line.
	The command sintax is similar to that of GMT.
	To get instructions run the program with no commands.
	All the output is sent to stdout! To save to a file redirect stdout using >
	
Programs:
	All the tess*.exe programs calculate the respective gravity field component of the tesseroid model provided.
	
	Example:
		To calculate the vertical component of gravity of ModelExample.txt over a region W=-10 E=10 S=-20 N=0, 
		using a 100x100 grid, at the altitude of 250000m, GLQ order in the: latitude direction=8, longitude dir=8 
		and radial dir=2 and using the 3D GLQ instead of analytical integration in the r direction and 2D GLQ (option -A2D).
		The output grid will be saved to output.gz.txt
		
		tessgz.exe ModelExample.txt -R-10/10/-20/0 -B100/100 -Z250000 -O8/8/2 -A3D > output.gz.txt
		
		
	The DIVMOD program divides the tesseroids in a model into smaller ones.
	This should be done to stabilise the GLQ when it is unstable.
	For calculations at 250km altitude (GOCE orbit) the recomended maximum size of the tesseoids is 1ºx1º and GLQ order 8/8/2.
	The radial dimension has little influence on the stability of the GLQ.
	
	Example:
		To divide all the tesseroids in ModelExample.txt by 2 in the longitudial direction, 3 in the latitudinal dir,
		and 1 (not divide) in the radial direction use:
		The divided model will be saved to ModelExample_div.txt
		
		divmod.exe ModelExample.txt -O2/3/1 > ModelExample_div.txt