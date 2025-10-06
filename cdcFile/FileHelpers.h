#pragma once

class ArchiveFileSystem;

namespace cdc {

class FileSystem;
extern ArchiveFileSystem *archiveFileSystem_default;
extern FileSystem *upperFileSystem;

void createLowerFileSystems();
void createHigherFileSystems();
void destroyFileSystems();
cdc::FileSystem *getDefaultFileSystem();
char *FSHelper_ReadFile(const char *path);

}
