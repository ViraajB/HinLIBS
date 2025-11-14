#ifndef LIBRARYSYSTEM_H
#define LIBRARYSYSTEM_H

#include <vector>

class User;
class Item;

class LibrarySystem {
private:

    std::vector<Item*> catalogue;
    std::vector<User*> users;
    User* currentUser;

    void loadStartingData();

public:

    LibrarySystem();
    ~LibrarySystem();

    std::vector<Item*> getCatalogue() const { return catalogue; }
    std::vector<User*> getUsers() const { return users; }

    User* getCurrentUser() const { return currentUser; }
    void setCurrentUser(User* user) { currentUser = user; }

    static const int MAX_LOANS_PER_PATRON;
    static const int LOAN_PERIOD_DAYS;
};

#endif



