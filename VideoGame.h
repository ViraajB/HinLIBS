#ifndef VIDEOGAME_H
#define VIDEOGAME_H

#include "Item.h"
#include <string>

class VideoGame : public Item {
public:
    VideoGame(const std::string& title, const std::string& author, const std::string& genre, const std::string& rating);

    std::string getGenre() const { return genre; }
    std::string getRating() const { return rating; }

private:
    std::string genre;
    std::string rating;
};

#endif
