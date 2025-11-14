#ifndef MAGAZINE_H
#define MAGAZINE_H

#include "Item.h"
#include <string>

class Magazine : public Item {
public:
    Magazine(const std::string& title, const std::string& author, int issueNumber, const std::string& publicationDate);

    int getIssueNumber() const { return issueNumber; }
    std::string getPublicationDate() const { return publicationDate; }

private:
    int issueNumber;
    std::string publicationDate;

};
#endif
