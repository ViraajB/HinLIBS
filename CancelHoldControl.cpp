#include "CancelHoldControl.h"
#include "LibrarySystem.h"
#include "Item.h"
#include "Patron.h"
#include <vector>

using namespace std;

CancelHoldControl::CancelHoldControl(LibrarySystem* libSys, QObject* parent)
    : QObject(parent), librarySystem(libSys) {}

void CancelHoldControl::cancelHoldRequested(int itemId) {
    if (!librarySystem) {
        emit cancelHoldFailed("Library system not initialized.");
        return;
    }

    Patron* currentPatron = dynamic_cast<Patron*>(librarySystem->getCurrentUser());
    if (!currentPatron) {
        emit cancelHoldFailed("No patron is currently logged in.");
        return;
    }

    vector<Item*> catalogue = librarySystem->getCatalogue();
    Item* requestedItem = nullptr;
    for (Item* it : catalogue) {
        if (it && it->getId() == itemId) {
            requestedItem = it;
            break;
        }
    }

    if (!requestedItem) {
        emit cancelHoldFailed("Selected item not found in catalogue.");
        return;
    }

    vector<Item*> activeHolds = currentPatron->getActiveHolds();
    bool holdFound = false;
    for (Item* holdItem : activeHolds) {
        if (holdItem && holdItem->getId() == itemId) {
            holdFound = true;
            break;
        }
    }

    if (!holdFound) {
        emit cancelHoldFailed("You do not have a hold on this item.");
        return;
    }

    requestedItem->removeHold(currentPatron);
    currentPatron->removeItemFromHoldQueue(requestedItem);

    emit cancelHoldSuccessful("Hold cancelled successfully.");
}
