#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <vector>

class Patron;

class Item {

public:

    Item(const std::string& title, const std::string& author, const std::string& format);
    virtual ~Item();

    //getters
    int getId() const { return id; }
    std::string getTitle() const { return title; }
    std::string getAuthor() const { return author; }
    std::string getFormat() const { return format; }
    std::string getDetail() const {return detail;}

    bool getAvailabilityStatus() const { return availabilityStatus; }
    void setAvailabilityStatus(bool status) { availabilityStatus = status; }

    std::vector<Patron*> getHoldQueue() const { return holdQueue; }
    int getQueuePosition(const Patron* p) const;

    bool addHold(Patron* p);
    bool removeHold(Patron* p);

private:
    static int nextId;
    int id;
    std::string title;
    std::string author;
    std::string format;
    std::string detail;
    bool availabilityStatus;
    std::vector<Patron*> holdQueue;

};

#endif
