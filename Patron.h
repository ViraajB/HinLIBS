#ifndef PATRON_H
#define PATRON_H

#include <string>
#include <vector>
#include "User.h"

class Item;
class Loan;

class Patron : public User{
public:
    Patron(const std::string& name);

    bool addItemToLoanList(Loan* itm);
    bool removeItemFromLoanList(Loan* itm);
    bool addItemToHoldQueue(Item* itm);
    bool removeItemFromHoldQueue(Item* itm);

    std::vector<Loan*> getActiveLoans() const;
    std::vector<Item*> getActiveHolds() const;

private:
    std::vector<Item*> holdList;
    std::vector<Loan*> loanedList;

};

#endif
