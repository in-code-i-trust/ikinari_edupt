/*
Copyright (c) 2014 hole
This software is released under the MIT License (http://kagamin.net/hole/license.txt).
A part of this software is based on smallpt (http://www.kevinbeason.com/smallpt/) and
released under the MIT License (http://kagamin.net/hole/smallpt-license.txt).
*/
// #include <iostream>

#include "render.h"

extern "C" int _fltused = 1;

extern "C" void* allocate(unsigned long long sz)
{
	return (void*)GlobalAlloc(0x40, sz);
}

// int main(int argc, char **argv) 
void entryPoint()
{
	OutputDebugStringW(L"Path tracing renderer\r\n");
	// std::cout << "Path tracing renderer: edupt" << std::endl << std::endl;

	// 640x480の画像、(2x2) * 4 sample / pixel
	edupt::render(640, 480, 4, 2);
	ExitProcess(0);
}
