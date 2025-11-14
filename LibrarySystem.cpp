#include "LibrarySystem.h"
#include "User.h"
#include "Patron.h"
#include "Librarian.h"
#include "SystemAdministrator.h"
#include "Item.h"
#include "FictionBook.h"
#include "NonFictionBook.h"
#include "Magazine.h"
#include "Movie.h"
#include "VideoGame.h"

using namespace std;

const int LibrarySystem::MAX_LOANS_PER_PATRON = 3;
const int LibrarySystem::LOAN_PERIOD_DAYS = 14;

LibrarySystem::LibrarySystem() : currentUser(nullptr) {
    loadStartingData();
}

LibrarySystem::~LibrarySystem() {
    for (Item* item : catalogue) {
        delete item;
    }
    for (User* user : users) {
        delete user;
    }
}

void LibrarySystem::loadStartingData() {
    // 7 sample users: 5 patrons, 1 librarian, 1 system administrator
    users.push_back(new Patron("Patron1"));
    users.push_back(new Patron("Patron2"));
    users.push_back(new Patron("Patron3"));
    users.push_back(new Patron("Patron4"));
    users.push_back(new Patron("Patron5"));
    users.push_back(new Librarian("Librarian1"));
    users.push_back(new SystemAdministrator("SystemAdministrator1"));

    // 20 sample items: 5 fiction books, 5 non-fiction books, 3 magazines, 3 movies, 4 video games. This can be edited
    catalogue.push_back(new FictionBook("To Kill a Mockingbird", "Harper Lee"));
    catalogue.push_back(new FictionBook("Beloved", "Toni Morrison"));
    catalogue.push_back(new FictionBook("The Great Gatsby", "F. Scott Fitzgerald"));
    catalogue.push_back(new FictionBook("Invisible Man", "Ralph Ellison"));
    catalogue.push_back(new FictionBook("Pride and Prejudice", "Jane Austen"));

    catalogue.push_back(new NonFictionBook("Thinking Fast And Slow", "Daniel Kahneman", "153.11"));
    catalogue.push_back(new NonFictionBook("In Cold Blood", "Truman Capote", "364.12"));
    catalogue.push_back(new NonFictionBook("Educated", "Tara Westover", "920.13"));
    catalogue.push_back(new NonFictionBook("Silent Spring", "Rachel Carson", "363.14"));
    catalogue.push_back(new NonFictionBook("Guns, Germs, and Steel", "Jared Diamond", "900.15"));

    catalogue.push_back(new Magazine("The Vogue #12", "Editorial", 12, "2025-10-31"));
    catalogue.push_back(new Magazine("The Economist", "Editorial", 9486, "2025-11-01"));
    catalogue.push_back(new Magazine("The Rake Magazine", "Editorial", 101, "2025-10-15"));

    catalogue.push_back(new Movie("Interstellar", "Christopher Nolan", "Sci-fi", "PG-13"));
    catalogue.push_back(new Movie("Avengers Endgame", "Russo Brothers", "Sci-fi", "PG-13"));
    catalogue.push_back(new Movie("Spiderman - No Way Home", "Jon Watts", "Sci-Fi", "PG-13"));

    catalogue.push_back(new VideoGame("Spiderman Remastered", "Insomniac", "Action", "PG-13"));
    catalogue.push_back(new VideoGame("GTA V", "Rockstar", "Crime", "18+"));
    catalogue.push_back(new VideoGame("The Last Of Us I", "Naughty Dog", "Horror-Survival", "18+"));
    catalogue.push_back(new VideoGame("The Last Of Us II", "Naughty Dog", "Horror-Survival", "18+"));
}

