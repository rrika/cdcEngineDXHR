#include "MultiFileSystem.h"

namespace cdc {

int32_t MultiFileSystem::ExistsInFileSystem(const char *path) {
	for (int i=0; i < m_systems.size(); i++)
		if (m_systems[i]->FileExists(path))
			return i;
	return -1;
}

FileSystem *MultiFileSystem::GetBestFileSystem(const char *path) {
	int index = ExistsInFileSystem(path);
	return m_systems[index >= 0 ? index : 0];
	// TODO
}

FileRequest *MultiFileSystem::createRequest(FileReceiver *receiver, const char *path, uint32_t offset) {
	// TODO
	return GetBestFileSystem(path)->createRequest(receiver, path, offset);
}

File *MultiFileSystem::createFile(const char *path) {
	return GetBestFileSystem(path)->createFile(path);
}

bool MultiFileSystem::FileExists(const char *path) {
	return ExistsInFileSystem(path) >= 0;
}

uint32_t MultiFileSystem::getSize(const char *path) {
	return GetBestFileSystem(path)->getSize(path);
}

void MultiFileSystem::setLanguageMask(uint32_t mask) {
	for (auto system : m_systems)
		system->setLanguageMask(mask);
}

uint32_t MultiFileSystem::getLanguageMask() {
	// TODO
	return m_systems[0]->getLanguageMask();
}

bool MultiFileSystem::hasRequests() {
	for (auto system : m_systems)
		if (system->hasRequests())
			return true;
	return false;
}

void MultiFileSystem::processRequest() {
	for (auto system : m_systems)
		system->processRequest();
	// TODO
}

void MultiFileSystem::processAll() {
	for (auto system : m_systems)
		system->processAll();
}

void MultiFileSystem::processUntil(FileRequest *request) {
	while (request->getCompletionStatus() != 3)
	{
		// instead of calling this->processRequest() like other classes
		// this just does the first loop of processRequest
		for (auto system : m_systems)
			system->processRequest();
	}
}

}
