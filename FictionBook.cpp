#include "FictionBook.h"

using namespace std;

FictionBook::FictionBook(const string& title, const string& author)
    : Item(title, author, "Fiction Book") {}
