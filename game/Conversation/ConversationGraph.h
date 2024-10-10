#pragma once

namespace dtp {
struct ConversationGraphNode;
struct ConversationGraphNodeList;
}

class ConversationBase;
template <typename T> class ConversationGraphNode;

template <typename T>
class ConversationGraph {
public:
	// dtp::ConversationGraphNodeList *dtpNodeList; // 4
	// ConversationGraphNode<T> *node8;
	// ConversationGraphNode<T> *nodeC;
	// ConversationBase *conv; // 10
	// dtp::ConversationGraphNode *dtpNode; // 14

	ConversationGraphNode<T> *createNode(
		const char *nodetype,
		dtp::ConversationGraphNode*,
		ConversationBase*);
};
