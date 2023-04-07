#include "BuiltinResources.h"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include "cdcSys/Assert.h"

// is it okay to cast pointer to vector::data() to a struct with uint32_t? (generally, yes)
static_assert(__STDCPP_DEFAULT_NEW_ALIGNMENT__ % 4 == 0);

static std::vector<char> loadWholeFile(const char *path) {
	FILE *f = fopen(path, "rb");
	if (!f) return {};
	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);
	fseek(f, 0, SEEK_SET);
	std::vector<char> buffer(size);
	fread(buffer.data(), size, 1, f);
	fclose(f);
	return buffer;
}

static std::vector<char> shaderBlob;

namespace shad {

char *error_vs = nullptr;
char *error_ps = nullptr;
char *bilateralBlur_cs = nullptr;
char *fastBlur1_cs = nullptr;
char *fastBlur0_cs = nullptr;
char *shader_5_ps = nullptr;
char *shader_6_vs = nullptr;
char *shader_7_vs = nullptr;
char *shader_8_ps = nullptr;
char *shader_9_ps = nullptr;
char *mlaa_10_ps = nullptr;
char *mlaa_11_ps = nullptr;
char *fxaa_12_ps = nullptr;
char *mlaa_13_ps = nullptr;
char *normalPass_14_vs = nullptr;
char *normalPass_15_vs = nullptr;
char *normalPass_16_ps = nullptr;
char *copy_17_ps = nullptr;
char *copy_18_vs = nullptr;
char *aa_19_vs = nullptr;
char *shader_20_vs = nullptr;
char *shader_21_vs = nullptr;
char *shader_22_ps = nullptr;

char *errorColor_ps = nullptr;
char *fogColor_ps = nullptr;
char *white_25_ps = nullptr;
char *passthrough_ps = nullptr;
char *white_27_ps = nullptr;
char *shader_28_vs = nullptr;
char *shader_29_vs = nullptr;
char *shader_30_vs = nullptr;

char *shader_32 = nullptr;
char *shader_33 = nullptr;
char *shader_34 = nullptr;
char *shader_35 = nullptr;
char *shader_36 = nullptr;
char *shader_37 = nullptr;
char *shader_38 = nullptr;
char *shader_39 = nullptr;
char *shader_40 = nullptr;

char *shader_41 = nullptr;
char *shader_42 = nullptr;
char *shader_43 = nullptr;
char *shader_44 = nullptr;
char *shader_45 = nullptr;
char *shader_46 = nullptr;
char *shader_47 = nullptr;
char *shader_48 = nullptr;
char *shader_49 = nullptr;

}

using namespace shad;

void loadBuiltinResources() {
	if (!shaderBlob.empty())
		return;

	shaderBlob = loadWholeFile("DXHRDC.shad");
	if (shaderBlob.empty())
		shaderBlob = loadWholeFile("../DXHRDC.shad");

	if (shaderBlob.empty()) {
		cdc::FatalError("Couldn't open DXHRDC.shad");
		exit(1);
		return;
	}

	auto probeShader = [&](uint32_t offset) {
		auto *data = (uint32_t*)(shaderBlob.data() + offset);
		// printf("%08x %08x %08x %08x %08x\n",
		// 	offset, data[0], data[1], data[2], data[3]);

		return (char*)data;
	};

	// dx11 shaders
	error_vs         = probeShader(0x00000);
	error_ps         = probeShader(0x00c30);
	bilateralBlur_cs = probeShader(0x00e60);
	fastBlur1_cs     = probeShader(0x0d420);
	fastBlur0_cs     = probeShader(0x14910);
	shader_5_ps      = probeShader(0x1bcc0);
	shader_6_vs      = probeShader(0x1c3b0);
	shader_7_vs      = probeShader(0x1cb60);
	shader_8_ps      = probeShader(0x1daf0);
	shader_9_ps      = probeShader(0x1e8c0);
	mlaa_10_ps       = probeShader(0x1fbc0);
	mlaa_11_ps       = probeShader(0x21dd0);
	fxaa_12_ps       = probeShader(0x22870);
	mlaa_13_ps       = probeShader(0x28bf0);
	normalPass_14_vs = probeShader(0x29280);
	normalPass_15_vs = probeShader(0x29640);
	normalPass_16_ps = probeShader(0x29870);
	copy_17_ps       = probeShader(0x2ca90);
	copy_18_vs       = probeShader(0x2e110);
	aa_19_vs         = probeShader(0x2e360);
	shader_20_vs     = probeShader(0x2e6a0);
	shader_21_vs     = probeShader(0x2ed40);
	shader_22_ps     = probeShader(0x2fc90);

	errorColor_ps    = probeShader(0x302d0);
	fogColor_ps      = probeShader(0x30590);
	white_25_ps      = probeShader(0x30900);
	passthrough_ps   = probeShader(0x31630);
	white_27_ps      = probeShader(0x31860);
	shader_28_vs     = probeShader(0x325a0); // {vert0 * world * viewproj}
	shader_29_vs     = probeShader(0x33200); // {vert1 * screenmatrix, vert0 * instanceparams}
	shader_30_vs     = probeShader(0x343b0); // {vert0 * world * viewproj, {1, 1, 1, 1}, {0, 0, 0, 0}}

	// dx9 shaders
	shader_32 = probeShader(0x36f00);
	shader_33 = probeShader(0x37350);
	shader_34 = probeShader(0x37410);
	shader_35 = probeShader(0x39f20);
	shader_36 = probeShader(0x3a170);
	shader_37 = probeShader(0x3a430);
	shader_38 = probeShader(0x3a670);
	shader_39 = probeShader(0x3a8a0);
	shader_40 = probeShader(0x3ab50);

	// made a mistake while counting, there is no shader_41
	shader_42 = probeShader(0x3ad80);
	shader_43 = probeShader(0x3ae40);
	shader_44 = probeShader(0x3af00);
	shader_45 = probeShader(0x3b0b0);
	shader_46 = probeShader(0x3b2d0);
	shader_47 = probeShader(0x3b760);
	shader_48 = probeShader(0x3de48);
	shader_49 = probeShader(0x3e010);
}
