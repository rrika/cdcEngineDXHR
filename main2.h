#pragma once
#include "windows.h"
namespace cdc { class FileSystem; }
class ArchiveFileSystem;
extern void *yellowCursor;
extern ArchiveFileSystem *archiveFileSystem_default;
extern HWND hwnd1;
cdc::FileSystem *getDefaultFileSystem();
char *readFileBlocking(const char *path);
void setupPrefixes(const char *prefix);
void buildDRMPath(char *buffer, const char *name);
int WinMain2(HINSTANCE hInstance, LPSTR lpCmdLine);
