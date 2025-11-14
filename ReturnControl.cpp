#include "ReturnControl.h"
#include "LibrarySystem.h"
#include "Item.h"
#include "Patron.h"
#include "Loan.h"
#include <vector>

using namespace std;

ReturnControl::ReturnControl(LibrarySystem* libSys, QObject* parent)
    : QObject(parent), librarySystem(libSys) {}

void ReturnControl::returnRequested(int itemId) {
    if (!librarySystem) {
        emit returnFailed("Library system not initialized.");
        return;
    }

    Patron* currentPatron = dynamic_cast<Patron*>(librarySystem->getCurrentUser());
    if (!currentPatron) {
        emit returnFailed("No patron is currently logged in.");
        return;
    }

    vector<Item*> catalogue = librarySystem->getCatalogue();
    vector<Loan*> activeLoans = currentPatron->getActiveLoans();

    Loan* loanToReturn = nullptr;
    for (Loan* loan : activeLoans) {
        if (loan->getItem()->getId() == itemId) {
            loanToReturn = loan;
            break;
        }
    }

    if (!loanToReturn) {
        emit returnFailed("This item is not currently loaned by you.");
        return;
    }

    Item* itemToReturn = loanToReturn->getItem();
    currentPatron->removeItemFromLoanList(loanToReturn);
    itemToReturn->setAvailabilityStatus(true);
    delete loanToReturn;

    emit returnSuccessful("Item returned successfully.");
}

