#ifndef LOGINCONTROL_H
#define LOGINCONTROL_H

#include <QObject>
#include <QString>

class LibrarySystem;

class LoginControl : public QObject
{
    Q_OBJECT

private:
    LibrarySystem* librarySystem;

public:
    explicit LoginControl(LibrarySystem* libSys, QObject* parent = nullptr);

signals:
    void loginSuccessful(const QString& usertype);
    void loginFailed(const QString& reason);

    void logoutSuccessful();

public slots:
    void loginRequested(const QString& username);
    void logoutRequested();
};

#endif
