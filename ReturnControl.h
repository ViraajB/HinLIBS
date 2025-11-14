#ifndef RETURNCONTROL_H
#define RETURNCONTROL_H

#include <QObject>
#include <QString>

class LibrarySystem;
class Loan;

class ReturnControl : public QObject
{
    Q_OBJECT

private:
    LibrarySystem* librarySystem;

public:
    explicit ReturnControl(LibrarySystem* libSys, QObject* parent = nullptr);

signals:
    void returnSuccessful(const QString& successMessage);
    void returnFailed(const QString& reason);

public slots:
    void returnRequested(int itemId);
};

#endif
