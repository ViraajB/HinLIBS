#include "PlaceHoldControl.h"
#include "LibrarySystem.h"
#include "Item.h"
#include "Patron.h"
#include <vector>

using namespace std;

PlaceHoldControl::PlaceHoldControl(LibrarySystem* libSys, QObject* parent)
    : QObject(parent), librarySystem(libSys) {}

void PlaceHoldControl::placeHoldRequested(int itemId) {
    if (!librarySystem) {
        emit placeHoldFailed("Library system not initialized.");
        return;
    }

    Patron* currentPatron = dynamic_cast<Patron*>(librarySystem->getCurrentUser());
    if (!currentPatron) {
        emit placeHoldFailed("No patron is currently logged in.");
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
        emit placeHoldFailed("Selected item not found in catalogue.");
        return;
    }

    // If it's available, no need to hold – user should borrow instead
    if (requestedItem->getAvailabilityStatus()) {
        emit placeHoldFailed("Item is currently available; please borrow it instead.");
        return;
    }

    // Don't allow duplicate holds by same patron
    vector<Item*> activeHolds = currentPatron->getActiveHolds();
    for (Item* holdItem : activeHolds) {
        if (holdItem && holdItem->getId() == itemId) {
            emit placeHoldFailed("You have already placed a hold on this item.");
            return;
        }
    }

    requestedItem->addHold(currentPatron);
    currentPatron->addItemToHoldQueue(requestedItem);

    emit placeHoldSuccessful("Hold placed successfully.");
}
