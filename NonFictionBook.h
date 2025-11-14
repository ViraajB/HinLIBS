#ifndef NONFICTIONBOOK_H
#define NONFICTIONBOOK_H

#include "Item.h"
#include <string>

class NonFictionBook : public Item {
public:
    NonFictionBook(const std::string& title, const std::string& author, const std::string& deweyDecimal);

    std::string getDeweyDecimal() const { return deweyDecimal; }

private:
    std::string deweyDecimal;
};
#endif
