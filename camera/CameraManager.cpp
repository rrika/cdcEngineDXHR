#include "CameraManager.h"

void CameraManager::update() {
	matrix = *activeCamera->getMatrix();
}

cdc::Matrix *CameraManager::getActiveCameraMatrix() {
	return activeCamera->getMatrix();
}

cdc::Matrix *CameraManager::getMatrix() {
	return &matrix;
}
