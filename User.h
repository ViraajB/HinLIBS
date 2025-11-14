#ifndef USER_H
#define USER_H

#include <string>

class User
{
public:
    User(const std::string& name, const std::string& type);

    // ⭐ Add this line:
    virtual ~User() = default;

    // getters
    std::string getName() const { return userName; }
    std::string getType() const { return userType; }

private:
    std::string userName;
    std::string userType;
};

#endif
