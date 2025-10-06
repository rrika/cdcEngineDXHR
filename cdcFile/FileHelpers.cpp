#include <cstdlib> // for getenv
#include <cstring>
#include <string>
#ifdef _WIN32
#include <windows.h>
#endif
#include "FileHelpers.h"
#include "ArchiveFileSystem.h"
#include "FileUserBufferReceiver.h"
#include "HackFileSystem.h"
#include "MultiFileSystem.h"
#include "cdcSys/Assert.h"

namespace cdc {

FileSystem *lowerFileSystem;
FileSystem *threadedLowerFileSystem;

void createLowerFileSystems() {
	// TODO
	lowerFileSystem = new HackFileSystem();
	threadedLowerFileSystem = lowerFileSystem;
}

ArchiveFileSystem *archiveFileSystem_default;
ArchiveFileSystem *archiveFileSystem_lang;
FileSystem *upperFileSystem;

std::string findBigfile() {
	if (const char *bigfileEnv = getenv("BIGFILE"))
		return bigfileEnv;
#ifdef _WIN32
	char buffer[512] = {0};
	DWORD bufferSize = sizeof(buffer);
	LSTATUS r;
	r = RegGetValueA(HKEY_LOCAL_MACHINE,
		"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Steam App 238010",
		"InstallLocation",
		RRF_RT_REG_SZ | RRF_SUBKEY_WOW6464KEY,
		nullptr,
		buffer,
		&bufferSize);
	if (r == ERROR_SUCCESS) {
		printf("found installation at %s\n", buffer);
		return std::string(buffer) + "\\BIGFILE.000";
	}
	// There's also the key HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall\Steam App 238010
	// but it's not clear to me when that is written, and which access flag are needed to read it,
	// so it will remain unimplemented until a need arises.
#endif
	return "";
}

void createHigherFileSystems() {
	std::string bigfilePath = findBigfile();
	if (bigfilePath.empty()) {
		printf("\nspecify path to BIGFILE.000 through BIGFILE environment variable\n\n");
		exit(1);
		return;
	}
	archiveFileSystem_default = new ArchiveFileSystem(lowerFileSystem);
	bool indexOk = archiveFileSystem_default->readIndex(bigfilePath.c_str(), 0);

	if (!indexOk) {
		FatalError("Unable to open bigfile BIGFILE.DAT!");
		exit(1);
	}

	std::string languagePath = bigfilePath;
	if (char const *period = strrchr(languagePath.c_str(), '.')) {
		languagePath.resize(period-languagePath.c_str());
		languagePath += "_English.000";
	}
	archiveFileSystem_lang = new ArchiveFileSystem(lowerFileSystem);
	indexOk = archiveFileSystem_lang->readIndex(languagePath.c_str(), 0);

	if (indexOk) {
		auto *multi = new MultiFileSystem();
		multi->m_systems.push_back(archiveFileSystem_default);
		multi->m_systems.push_back(archiveFileSystem_lang);
		upperFileSystem = multi;
	} else {
		upperFileSystem = archiveFileSystem_default;
	}
}

void destroyFileSystems() {
	// TODO
}

FileSystem *getDefaultFileSystem() {
	// TODO
	return upperFileSystem;
}

char *FSHelper_ReadFile(const char *path) {
	FileSystem *fs;
	if (strcmp(path, "pc-dev\\dtpdata.ids") == 0) {
		// the copy that comes with the PC version is incorrect.
		// use the list that comes with this repo instead.
	 	fs = lowerFileSystem;
	 	path = "dtpdata.ids";
	} else {
	 	fs = upperFileSystem; // HACK
	}
	uint32_t size = fs->getSize(path);

	char *buffer = new char[size+1];
	FileReceiver *rec = FileUserBufferReceiver::create(buffer);
	FileRequest *req = fs->createRequest(rec, path, 0);
	req->submit(1);
	fs->processUntil(req);
	req->decrRefCount();
	buffer[size] = '\0';
	// req is owned by fs which takes care of it in processAll()

	return buffer;
}

}
