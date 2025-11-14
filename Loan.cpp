#include "Loan.h"
#include "Item.h"
#include "Patron.h"
#include "LibrarySystem.h"

using namespace std;

Loan::Loan(Item* item, Patron* borrower)
    : item(item), borrower(borrower) {
    borrowedDate = chrono::system_clock::now();
    dueDate = borrowedDate + chrono::hours(24 * LibrarySystem::LOAN_PERIOD_DAYS);
}

int Loan::calculateDaysRemaining() const {
    std::chrono::system_clock::time_point now = chrono::system_clock::now();
    std::chrono::hours diff = chrono::duration_cast<chrono::hours>(dueDate - now);
    int remainingDays = diff.count() / 24;
    return remainingDays;
}
