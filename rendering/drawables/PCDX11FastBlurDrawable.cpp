#include <cmath>
#include "PCDX11FastBlurDrawable.h"
#include "../PCDX11DeviceManager.h"
#include "../PCDX11RenderDevice.h"
#include "../PCDX11StateManager.h"
#include "../buffers/PCDX11UberConstantBuffer.h"
#include "../surfaces/PCDX11BaseTexture.h"
#include "../surfaces/PCDX11RenderTarget.h"

namespace cdc {

void PCDX11FastBlurDrawable::draw(uint32_t funcSetIndex, IRenderDrawable *other) {
	auto stateManager = deviceManager->getStateManager();

	uint32_t texWidth = m_texture->width;
	uint32_t texHeight = m_texture->height;
	uint32_t rtWidth = m_renderTarget->getWidth();
	uint32_t rtHeight = m_renderTarget->getHeight();

	// bind
	stateManager->pushRenderTargets(nullptr, nullptr);
	auto *shader = m_renderDevice->getBlurShader(isHorizontalPass, weighted ? 1 : 0);
	stateManager->setComputeShader(shader);
	stateManager->setComputeShaderTexture(0, m_texture);
	stateManager->setComputeShaderUAV(0, m_renderTarget->getRenderTexture());

	// instruct shader
	//PCDX11DynamicConstantBuffer cb;
	PCDX11UberConstantBuffer cb(1);
	float row[] = {(float)texWidth, (float)texHeight, (float)rtWidth, (float)rtHeight};
	unsigned irow[] = {texWidth, texHeight, rtWidth, rtHeight};
	memcpy(row, irow, 16); // BUG: shader expects ints actually so memcpy those
	cb.assignRow(0, row, 1);
	cb.syncBuffer(m_renderDevice->getD3DDeviceContext());
	stateManager->setComputeShaderCB(0, &cb);

	// dispatch
	float widthDiv = isHorizontalPass ? 128 : 2;
	float heightDiv = isHorizontalPass ? 2 : 128;
	unsigned x = (unsigned) ceil(rtWidth / widthDiv);
	unsigned y = (unsigned) ceil(rtHeight / heightDiv);
	stateManager->dispatch(x, y, 1);

	// clear state
	stateManager->setComputeShaderTexture(0, nullptr);
	stateManager->setComputeShaderUAV(0, nullptr);
	stateManager->setComputeShaderCB(0, nullptr);
	stateManager->popRenderTargets();
}

uint32_t PCDX11FastBlurDrawable::compare(uint32_t funcSetIndex, IRenderDrawable *other) {
	// TODO
	return 1;
}

}
