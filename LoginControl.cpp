#include "LoginControl.h"
#include "LibrarySystem.h"
#include "User.h"
#include <string>
#include <vector>

using namespace std;

LoginControl::LoginControl(LibrarySystem* libSys, QObject* parent)
    : QObject(parent), librarySystem(libSys) {}

void LoginControl::loginRequested(const QString& username) {
    if (!librarySystem) {
        emit loginFailed("Library system not initialized.");
        return;
    }

    vector<User*> users = librarySystem->getUsers();
    string usernameStr = username.toStdString();

    User* foundUser = nullptr;

    for (User* user : users) {
        if (user->getName() == usernameStr) {
            foundUser = user;
            break;
        }

        if (foundUser) {
            librarySystem->setCurrentUser(foundUser);
            emit loginSuccessful(QString::fromStdString(foundUser->getType()));
            return;
        } else {
            emit loginFailed("Invalid username.");
            return;
        }
    }
}

void LoginControl::logoutRequested() {
    if (librarySystem) {
        librarySystem->setCurrentUser(nullptr);
        emit logoutSuccessful();
    }
}
