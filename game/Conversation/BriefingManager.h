#pragma once

class ConversationBriefing;
class NsBrieferMovieController;

class BriefingManager {
	ConversationBriefing *m_briefing = nullptr; // 1C
	bool m_pendingCommentary = false; // 36
	NsBrieferMovieController *m_movieController = nullptr; // BC
public:
	bool hasBriefing() const { return m_briefing != nullptr; }
	// bool callBriefingMethod20() const { return m_briefing && m_briefing->method_20(); }
	void setCommentaryFlag(bool flag) { m_pendingCommentary = flag; }
	void openOrUpdateBriefer(bool zero);
	bool detectDev() const;

	static void message1(void*, void*);
	static void message447(void*, void*);

	// void serialize(BinaryWriter&);
	// void deserialize(BinaryReader&);
};
