#include <cstring>
#include <unordered_set>
#include "cdcResource/DTPDataSection.h"
#include "InventoryPlayer.h"

using namespace std;

enum FindMode {
	kAny = 0,
	kSmallest = 1,
	kBiggestNonFull = 2
};

static const bool g_disableReorganizeGrid = true;
static const bool *g_pDisableReorganizeGrid = &g_disableReorganizeGrid;

InventoryContainer::InventoryContainer(InventoryContainer const& other) {
	gridWidth = other.gridWidth;
	gridHeight = other.gridHeight;
	nextFreeSlot = other.nextFreeSlot;

	uint32_t numCells = gridWidth * gridHeight;
	m_grid = new uint8_t [numCells];
	memcpy(m_grid, other.m_grid, numCells);

	for (uint32_t i=0; i<std::size(m_slots); i++)
		m_slots[i] = other.m_slots[i];
}


InventorySlot *InventoryContainer::findWithLimit(
	dtp::Pickup *pickup, uint32_t dtpId, uint32_t mode, uint32_t& index)
{
	InventorySlot *result = nullptr;
	for (uint32_t i=index; i < /*256*/ std::size(m_slots); i++) {
		if (m_slots[i].dtpId != dtpId)
			continue;

		if (mode == 0)
			return &m_slots[index = i];

		else if (mode == 1) { // find smallest
			if (result && result->count <= m_slots[i].count)
				continue;

		} else if (mode == 2) { // find biggest non-full
			if (m_slots[i].count >= pickup->stack)
				continue;
			if (result && result->count >= m_slots[i].count)
				continue;

		} else
			continue;

		result = &m_slots[index = i];
	}

	return result;
}

uint32_t InventoryContainer::totalFreeCells() {
	uint32_t total = 0;
	for (uint32_t i = 0; i < gridWidth * gridHeight; i++)
		if (m_grid[i] == 0xff)
			total++;
	return total;
}

void InventoryContainer::assignRectangle(
	uint32_t x, uint32_t y, uint32_t w, uint32_t h,
	bool transpose, uint32_t cellByte, uint8_t *grid)
{
	if (!grid)
		grid = m_grid;

	if (transpose)
		std::swap(w, h);

	for (int32_t iy=y; iy<y+h; iy++)
		for (int32_t ix=x; ix<x+w; ix++)
			grid[ix + iy*gridWidth] = cellByte;
}

bool InventoryContainer::checkRectangle(
	uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t *grid, uint32_t refByte)
{
	if (!grid)
		grid = m_grid;

	if (x + w > gridWidth || y + h > gridHeight)
		return false;

	for (int32_t iy=y; iy<y+h; iy++) {
		for (int32_t ix=x; ix<x+w; ix++) {
			uint8_t cellByte = grid[ix + iy*gridWidth];
			if (cellByte != 0xff && cellByte != refByte)
				return false;
		}
	}
	return true;
}

bool InventoryContainer::findFreeSpot(dtp::Pickup *pickup, InventorySlot& slot) {
	for (int32_t y=0; y<gridHeight; y++) {
		for (int32_t x=0; x<gridWidth; x++) {
			slot.x = x;
			slot.y = y;
			if (checkRectangle(x, y, pickup->width, pickup->height, m_grid, 0xff)) {
				slot.transpose = false;
				return true;
			}
			if (checkRectangle(x, y, pickup->height, pickup->width, m_grid, 0xff)) {
				slot.transpose = true;
				return true;
			}
		}
	}
	return false;
}

void InventoryContainer::assignSlot(dtp::Pickup *pickup, InventorySlot& slotData) {
	InventorySlot& newSlot = m_slots[slotData.slotIndex];
	newSlot = slotData;
	assignRectangle(
		newSlot.x, newSlot.y,
		pickup->width, pickup->height,
		newSlot.transpose, nextFreeSlot, m_grid);
	for (uint32_t i=0; i<std::size(m_slots); i++) {
		if (m_slots[i].dtpId == ~0u) {
			nextFreeSlot = i;
			break;
		}
	}
}

void InventoryContainer::growGrid(uint32_t newWidth, uint32_t newHeight) {
	uint8_t *newGrid = new uint8_t[newWidth * newHeight];
	memset(newGrid, 0xff, newWidth * newHeight);
	if (m_grid) {
		// TODO: transfer contents
		delete[] m_grid;
	}
	gridWidth = newWidth;
	gridHeight = newHeight;
	m_grid = newGrid;
}

void InventoryContainer::removeFromGrid(uint32_t) {
	// TODO
}

bool InventoryContainer::reorganizeGridForOneMore(InventorySlot&, uint32_t) {
	// TODO
	return false;
}

InventoryContainer::InventoryContainer(uint32_t width, uint32_t height) {
	for (auto& slot : m_slots)
		slot = InventorySlot {};
	growGrid(width, height);
}

InventoryContainer::~InventoryContainer() {
	delete[] m_grid;
}

int32_t InventoryContainer::add(uint32_t dtpId, int32_t amount, InventorySlotSub *sub, std::vector<InventorySlot*> *affectedSlots) {
	if (amount <= 0)
		return amount;

	auto *pickup = (dtp::Pickup*)DTPDataSection::getPointer(dtpId);
	if (pickup->notAddable)
		return amount;

	int32_t leftToAdd = amount;
	InventorySlot *lastAffected = nullptr;

	uint32_t index = 0;

	while (leftToAdd > 0) {

		auto *slot = findWithLimit(pickup, dtpId, kBiggestNonFull, index);
		if (!slot) {
			slot = findWithLimit(pickup, dtpId, kAny, index);

			if (pickup->category == 3 && !slot) {
				uint32_t otherDtpId = pickup->dtpId48; // might be to add ammo through a gun pickup
				if (otherDtpId != 0 && otherDtpId != ~0u) {
					slot = findWithLimit(pickup, otherDtpId, kBiggestNonFull, index);
					if (!slot)
						slot = findWithLimit(pickup, otherDtpId, kAny, index);
				}
			}
		}

		if (pickup->category == 10) {
			// TODO: email
			leftToAdd--;
			break;
		}
		if (pickup->category == 11) {
			// TODO: developer commentary
			leftToAdd--;
			break;
		}

		if (slot) { // add to an existing stack
			if (pickup->flags2C & 1)
				break; // no stacking allowed

			if (slot->count >= pickup->stack) {
				index++;
				continue;
			}

			// update count
			auto spaceLeftInStack = int32_t(pickup->stack - slot->count);
			if (leftToAdd > spaceLeftInStack) {
				slot->count = pickup->stack;
				leftToAdd -= spaceLeftInStack;

			} else {
				slot->count += leftToAdd;
				leftToAdd = 0;
			}
			
			// record changes
			if (affectedSlots && lastAffected != slot) {
				lastAffected = slot;
				affectedSlots->push_back(lastAffected);
			}


		} else { // create a new slot and place 1 quantity there
			InventorySlot newSlot;

			if (sub)
				newSlot.sub = *sub;
			newSlot.dtpId = dtpId;
			newSlot.count = 1;
			newSlot.slotIndex = nextFreeSlot;

			if (pickup->width * pickup->height > totalFreeCells())
				break; // can't possibly fit

			if (findFreeSpot(pickup, newSlot)) {
				assignSlot(pickup, newSlot);
				leftToAdd--;

			} else if (!*g_pDisableReorganizeGrid && reorganizeGridForOneMore(newSlot, 1)) {
				leftToAdd--;
			 	
			} else
				break; // failed

			if (affectedSlots) {
				lastAffected = &m_slots[newSlot.slotIndex];
				affectedSlots->push_back(lastAffected);
			}
		}
	}

	return amount - leftToAdd;
}

uint32_t InventoryContainer::removeByType(uint32_t dtpId, uint32_t amount) {
	auto *pickup = (dtp::Pickup*)DTPDataSection::getPointer(dtpId);

	InventorySlot *smallest = nullptr;
	for (auto& candidate : m_slots) {
		if (candidate.dtpId != dtpId)
			continue;
		if (!smallest || candidate.count < smallest->count)
			smallest = &candidate;
	}

	if (!smallest)
		return 0;

	if (pickup->category == 8) {
		// TODO
	}
	return removeFromSlot(smallest - &*m_slots.begin(), amount);
}

uint32_t InventoryContainer::removeFromSlot(uint32_t slotIndex, uint32_t amount) {
	auto& slot = m_slots[slotIndex];
	uint32_t dtpId = slot.dtpId;
	uint32_t have = slot.count;
	if (have > amount) {
		// less than one stack
		slot.count -= amount;
		return amount;
	}

	// one stack
	removeFromGrid(slotIndex);

	// more than one stack
	if (have < amount)
		return have + removeByType(dtpId, amount - have);
	else
		return have;
}

uint32_t InventoryContainer::removeWholeStack(uint32_t slotIndex) {
	return removeFromSlot(slotIndex, m_slots[slotIndex].count);
}

void InventoryContainer::clear() {
	for (auto& slot : m_slots)
		slot.dtpId = ~0u;
	memset(m_grid, 0xff, gridWidth*gridHeight);
}

uint32_t InventoryContainer::totalOfType(uint32_t dtpId) {
	uint32_t total = 0;
	for (auto& slot : m_slots)
		if (slot.dtpId == dtpId)
			total += slot.count;
	return total;
}

vector<InventorySlot*> InventoryContainer::collectAllSlots() {
	return collectSlotsOfCategory((dtp::Pickup::Category)-1, false);
}

vector<InventorySlot*> InventoryContainer::collectSlotsOfCategory(dtp::Pickup::Category cat, bool noDuplicates) {
	vector<InventorySlot*> result;
	unordered_set<uint32_t> seenDtpIds;
	for (auto& slot : m_slots)
		if (slot.dtpId != ~0u) {
			auto *pickup = (dtp::Pickup*)DTPDataSection::getPointer(slot.dtpId);
			if (cat == ~0u || cat == pickup->category) {
				if (!noDuplicates || cat == pickup->category && seenDtpIds.insert(slot.dtpId).second)
					result.push_back(&slot);
			}
		}
	return result;
}

vector<InventorySlot*> InventoryContainer::collectSlotsOfType(uint32_t dtpId) {
	vector<InventorySlot*> result;
	for (auto& slot : m_slots)
		if (slot.dtpId == dtpId)
			result.push_back(&slot);
	return result;
}

uint32_t InventoryContainer::countSlotsInCategory(dtp::Pickup::Category cat, bool noDuplicates) {
	uint32_t result = 0;
	unordered_set<uint32_t> seenDtpIds;
	for (auto& slot : m_slots)
		if (slot.dtpId == ~0u) {
			auto *pickup = (dtp::Pickup*)DTPDataSection::getPointer(slot.dtpId);
			if (cat == ~0u || cat == pickup->category) {
				if (!noDuplicates || cat == pickup->category && seenDtpIds.insert(slot.dtpId).second)
					result++;
			}
		}
	return result;
}

InventorySlot *InventoryContainer::getSlot(uint32_t slotIndex) {
	return &m_slots[slotIndex];
}

InventorySlot *InventoryContainer::firstSlotOfType(uint32_t dtpId) {
	for (auto& slot : m_slots)
		if (slot.dtpId == dtpId)
			return &slot;

	return nullptr;
}

bool InventoryContainer::checkFit(
	vector<uint32_t>& dtpIndices,
	vector<uint32_t>& amounts
) {
	InventoryContainer copy = *this;
	for (uint32_t i=0; i<dtpIndices.size(); i++) {
		uint32_t added = copy.add(dtpIndices[i], amounts[i], 0, 0);
		if (added < amounts[i])
			return false;
	}
	return true;
}

// bool InventoryContainer::serialize(BinaryWriter&) {
// 	// TODO
// }

// bool InventoryContainer::deserialize(BinaryReader&) {
// 	// TODO
// }
