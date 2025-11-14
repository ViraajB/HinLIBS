#include "Movie.h"

using namespace std;

Movie::Movie(const string& title, const string& author, const string& genre, const string& rating)
    : Item(title, author, "Movie"), genre(genre), rating(rating) {}
