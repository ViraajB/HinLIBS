#include "BorrowControl.h"
#include "LibrarySystem.h"
#include "Item.h"
#include "Patron.h"
#include "Loan.h"
#include <vector>

using namespace std;

BorrowControl::BorrowControl(LibrarySystem* libSys, QObject* parent)
    : QObject(parent), librarySystem(libSys) {}

void BorrowControl::borrowRequested(int itemId) {
    if (!librarySystem) {
        emit borrowFailed("Library system not initialized.");
        return;
    }

    Patron* currentPatron = dynamic_cast<Patron*>(librarySystem->getCurrentUser());
    if (!currentPatron) {
        emit borrowFailed("No patron is currently logged in.");
        return;
    }

    // Find the Item with matching getId()
    vector<Item*> catalogue = librarySystem->getCatalogue();
    Item* requestedItem = nullptr;
    for (Item* it : catalogue) {
        if (it && it->getId() == itemId) {
            requestedItem = it;
            break;
        }
    }

    if (!requestedItem) {
        emit borrowFailed("Selected item not found in catalogue.");
        return;
    }

    // If NOT available, the only option should be "Place Hold", not "Borrow"
    if (!requestedItem->getAvailabilityStatus()) {
        emit borrowFailed("Item is not available for borrowing (already loaned).");
        return;
    }

    // Patron cannot exceed max loans
    if (currentPatron->getActiveLoans().size() >=
        static_cast<size_t>(LibrarySystem::MAX_LOANS_PER_PATRON)) {
        emit borrowFailed("You have reached the maximum number of loans.");
        return;
    }

    // Enforce hold queue: if there is a queue and you are not at the front, you can't borrow
    auto holdQueue = requestedItem->getHoldQueue();
    if (!holdQueue.empty() && holdQueue.front() != currentPatron) {
        emit borrowFailed("Item is on hold for another patron.");
        return;
    }

    // If you are at front of queue, remove your hold
    if (!holdQueue.empty() && holdQueue.front() == currentPatron) {
        requestedItem->removeHold(currentPatron);
        currentPatron->removeItemFromHoldQueue(requestedItem);
    }

    // Create loan, link to this patron only
    Loan* newLoan = new Loan(requestedItem, currentPatron);
    currentPatron->addItemToLoanList(newLoan);
    requestedItem->setAvailabilityStatus(false);   // 🔐 now unavailable to others



    emit borrowSuccessful("Item borrowed successfully.");
}
