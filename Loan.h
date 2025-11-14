#ifndef LOAN_H
#define LOAN_H

#include <chrono>

class Item;
class Patron;

class Loan {
private:
    Item* item;
    Patron* borrower;
    std::chrono::system_clock::time_point borrowedDate;
    std::chrono::system_clock::time_point dueDate;

public:
    Loan(Item* item, Patron* patron);

    Item* getItem() const { return item; }
    Patron* getBorrower() const { return borrower; }
    std::chrono::system_clock::time_point getBorrowedDate() const { return borrowedDate; }
    std::chrono::system_clock::time_point getDueDate() const { return dueDate; }

    int calculateDaysRemaining() const;
};

#endif
