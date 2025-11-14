#include "NonFictionBook.h"

using namespace std;

NonFictionBook::NonFictionBook(const string& title, const string& author, const string& deweyDecimal)
    : Item(title, author, "Non Fiction Book"), deweyDecimal(deweyDecimal) {}
