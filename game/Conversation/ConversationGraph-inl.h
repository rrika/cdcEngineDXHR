#include <cstring>
#include "ConversationGraph.h"
#include "ConversationGraphNodes.h"

#ifdef _WIN32
#define strcasecmp _stricmp
#endif

template <typename Dtp>
ConversationGraphNode<Dtp> *ConversationGraph<Dtp>::createNode(
	const char *nodetype,
	dtp::ConversationGraphNode *dtpNode,
	ConversationBase *conv)
{
	if (strcasecmp(nodetype, "Sound") == 0 || strcasecmp(nodetype, "Player") == 0)
		return new ConversationGraphNode_Player<Dtp>(this, dtpNode, conv);
	else if (strcasecmp(nodetype, "Animation") == 0)
		return new ConversationGraphNode_NPC<Dtp>(this, dtpNode, conv);
	else if (strcasecmp(nodetype, "Choice") == 0)
		return new ConversationGraphNode_Choice<Dtp>(this, dtpNode, conv);
	else if (strcasecmp(nodetype, "Script") == 0)
		return new ConversationGraphNode_Script<Dtp>(this, dtpNode, conv);
	else if (strcasecmp(nodetype, "Shop Override") == 0)
		return new ConversationGraphNode_ShopOverride<Dtp>(this, dtpNode, conv);
	else if (strcasecmp(nodetype, "Shop") == 0)
		return new ConversationGraphNode_Shop<Dtp>(this, dtpNode, conv);
	else if (strcasecmp(nodetype, "SmartScript") == 0)
		return new ConversationGraphNode_SmartScript<Dtp>(this, dtpNode, conv, "SmartScript");
	else if (strcasecmp(nodetype, "Actions") == 0)
		return new ConversationGraphNode_Actions<Dtp>(this, dtpNode, conv, "Actions");
	else if (strcasecmp(nodetype, "Random") == 0)
		return new ConversationGraphNode_Random<Dtp>(this, dtpNode, conv);
	else if (strcasecmp(nodetype, "Camera Track") == 0)
		return new ConversationGraphNode_CameraTrack<Dtp>(this, dtpNode, conv);
	else if (strcasecmp(nodetype, "Query") == 0)
		return new ConversationGraphNode_Query<Dtp>(this, dtpNode, conv);
	else if (strcasecmp(nodetype, "Delay") == 0)
		return new ConversationGraphNode_Delay<Dtp>(this, dtpNode, conv);
	else if (strcasecmp(nodetype, "Social Start") == 0)
		return new ConversationGraphNode_SocialStart<Dtp>(this, dtpNode, conv);
	else if (strcasecmp(nodetype, "Social Round") == 0)
		return new ConversationGraphNode_SocialRound<Dtp>(this, dtpNode, conv);
	else if (strcasecmp(nodetype, "Social Selection") == 0)
		return new ConversationGraphNode_SocialSelection<Dtp>(this, dtpNode, conv);
	else if (strcasecmp(nodetype, "Line") == 0)
		return new ConversationGraphNode_BriefingLine<Dtp>(this, dtpNode, conv);
	else if (strcasecmp(nodetype, "Call Function") == 0)
		return new ConversationGraphNode_ScriptFunctionCall<Dtp>(this, dtpNode, conv);
	else if (strcasecmp(nodetype, "Set Emotion") == 0)
		return new ConversationGraphNode_Emotion<Dtp>(this, dtpNode, conv);
	else if (strcasecmp(nodetype, "Social Query") == 0)
		return new ConversationGraphNode_SocialQuery<Dtp>(this, dtpNode, conv);
	else if (strcasecmp(nodetype, "Social Augment Pheromones") == 0)
		return new ConversationGraphNode_SocialAugmentPheromones<Dtp>(this, dtpNode, conv);
	else if (strcasecmp(nodetype, "Personality") == 0)
		return new ConversationGraphNode_SocialAugmentPersonality<Dtp>(this, dtpNode, conv);
	else if (strcasecmp(nodetype, "Stop") == 0)
		return new ConversationGraphNode_Stop<Dtp>(this, dtpNode, conv);
	else if (strcasecmp(nodetype, "Social End") == 0)
		return new ConversationGraphNode_SocialEnd<Dtp>(this, dtpNode, conv);

	return nullptr;
}
