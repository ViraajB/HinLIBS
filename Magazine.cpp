#include "Magazine.h"

using namespace std;

Magazine::Magazine(const string& title, const string& author, int issueNumber, const string& publicationDate)
    : Item(title, author, "Magazine"), issueNumber(issueNumber), publicationDate(publicationDate) {}
