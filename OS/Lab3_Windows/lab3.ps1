function Build
{
	# Static library
	g++ -c -o Number\Number.o Number\Number.cpp
	ar rvs Number\libNumber.lib Number\Number.o
	
	# Dynamic library
	g++ -c -DVECTOR_EXPORTS -o Vector\Vector.o -INumber\ Vector\Vector.cpp
	& g++ -shared -o Vector\libVector.dll '-Wl,--subsystem,windows,--out-implib,Vector\libVector.a' Vector\Vector.o Number\libNumber.lib
	
	# Build project
    Copy-Item "Vector\libVector.dll" -Destination ""
	g++ -o lab3 -INumber\ -IVector\ main.cpp -L.\Number\ -L.\Vector\ -lNumber -lVector
}

function Run
{
	.\lab3.exe
}

Build
Run
