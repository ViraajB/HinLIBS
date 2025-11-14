#include "Patron.h"
#include "User.h"
#include "Item.h"
#include "Loan.h"
#include <string>
#include <vector>

using namespace std;

Patron::Patron(const string& name)
    : User(name, "Patron") {}

bool Patron::addItemToLoanList(Loan* itm) {
    loanedList.push_back(itm);
    return true;
}

bool Patron::removeItemFromLoanList(Loan* itm){
    for (size_t i = 0; i < loanedList.size(); ++i) {
        if (loanedList[i] == itm) {
            for (size_t j = i; j < loanedList.size() - 1; ++j) {
                loanedList[j] = loanedList[j + 1];
            }
            loanedList.pop_back();
            return true;
        }
    }
    return false;
}

bool Patron::addItemToHoldQueue(Item* itm){
    holdList.push_back(itm);
    return true;
}

bool Patron::removeItemFromHoldQueue(Item* itm) {
    for (size_t i = 0; i < holdList.size(); ++i) {
        if (holdList[i] == itm) {
            for (size_t j = i; j < holdList.size() - 1; ++j) {
                holdList[j] = holdList[j + 1];
            }
            holdList.pop_back();
            return true;
        }
    }
    return false;
}

vector<Loan*> Patron::getActiveLoans() const{
    return loanedList;
}

vector<Item*> Patron::getActiveHolds() const{
    return holdList;
}
