#if defined _WIN32 || defined _WIN64
#include <Windows.h>

#define EXAMPLELIBRARY_EXPORT __declspec(dllexport)
#else
#include <stdio.h>
#endif

#ifndef EXAMPLELIBRARY_EXPORT
#define EXAMPLELIBRARY_EXPORT
#endif

EXAMPLELIBRARY_EXPORT void ExampleLibraryFunction()
{
#if defined _WIN32 || defined _WIN64
    MessageBox(NULL, TEXT("Loaded vertices.dll from Third Party Plugin sample."), TEXT("Unreal Algorand Plugin"), MB_OK);
#else
    printf("Loaded ExampleLibrary from Third Party Plugin sample");
#endif
}