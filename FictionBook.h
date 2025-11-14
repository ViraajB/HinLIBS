#ifndef FICTIONBOOK_H
#define FICTIONBOOK_H

#include "Item.h"
#include <string>

class FictionBook : public Item {
public:
    FictionBook(const std::string& title, const std::string& author);
};

#endif
