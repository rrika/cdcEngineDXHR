#pragma once

namespace cdc {

class GameShell {
public:
	static void LOAD_ObjectFileName(char *name, const char *object);
	static void LOAD_UnitFileName(char *name, const char *unit);
	// static void LOAD_SoftAccessoryFileName(char *name, const char *sa);

	static void LOAD_StreamGroupFileName(char *name, const char *streamgroup);
	static void LOAD_PadShockLibFileName(char *name, const char *libName);
	static void LOAD_MusicFileName(char *name, const char *music);

	static void LOAD_MiscFileName(char *name, const char *filename);
	static void LOAD_IMFFileName(char *name, const char *filename);
};

}
