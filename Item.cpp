#include "Item.h"
#include "Patron.h"

using namespace std;

int Item::nextId = 1;

Item::Item(const string& title, const string& author, const string& format)
    : id(nextId++), title(title), author(author), format(format), availabilityStatus(true) {}

Item::~Item() {}

int Item::getQueuePosition(const Patron* p) const {
    for (size_t i = 0; i < holdQueue.size(); ++i) {
        if (holdQueue[i] == p) {
            return static_cast<int>(i) + 1; // First position should be 1
        }
    }
    return -1;
}

bool Item::addHold(Patron* p) {
    for (Patron* patron : holdQueue) {
        if (patron == p) {
            return false;
        }
    }
    holdQueue.push_back(p);
    return true;
}

bool Item::removeHold(Patron* p) {
    for (size_t i = 0; i < holdQueue.size(); ++i) {
        if (holdQueue[i] == p) {
            for (size_t j = i; j < holdQueue.size() - 1; ++j) { // Shift elements left
                holdQueue[j] = holdQueue[j + 1];
            }
            holdQueue.pop_back();
            return true;
        }
    }
    return false;
}
