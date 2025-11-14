#include "VideoGame.h"

using namespace std;

VideoGame::VideoGame(const string& title, const string& author, const string& genre, const string& rating)
    : Item(title, author, "Video Game"), genre(genre), rating(rating) {}
