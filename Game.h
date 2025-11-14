#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include "item.h"

class Game : public Item {
public:
    Game(const std::string& title,
         const std::string& author,
         const std::string& format,
         const std::string& genre,
         const std::string& rating)
        : Item(title, author, format),
        genre(genre),
        rating(rating) {}

    std::string getGenre()  const { return genre; }
    std::string getRating() const { return rating; }

    std::vector<std::string> getAllInfo() const override {
        return { getTitle(), getAuthor(), getFormat(), "Game", genre, rating };
    }

private:
    std::string genre;
    std::string rating;
};

#endif // GAME_H
