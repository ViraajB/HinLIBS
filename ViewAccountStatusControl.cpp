#include "ViewAccountStatusControl.h"
#include "LibrarySystem.h"
#include "Patron.h"
#include "Loan.h"
#include "Item.h"
#include <vector>

using namespace std;

ViewAccountStatusControl::ViewAccountStatusControl(LibrarySystem* libSys, QObject* parent)
    : QObject(parent), librarySystem(libSys) {}

void ViewAccountStatusControl::viewAccountStatusRequested() {
    if (!librarySystem) {
        emit accountStatusRetrieved({}, {});
        return;
    }

    Patron* currentPatron = dynamic_cast<Patron*>(librarySystem->getCurrentUser());
    if (!currentPatron) {
        emit accountStatusRetrieved({}, {});
        return;
    }

    vector<Loan*> activeLoans = currentPatron->getActiveLoans();
    vector<Item*> activeHolds = currentPatron->getActiveHolds();

    emit accountStatusRetrieved(activeLoans, activeHolds);
}
