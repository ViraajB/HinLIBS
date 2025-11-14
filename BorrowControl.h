#ifndef BORROWCONTROL_H
#define BORROWCONTROL_H

#include <QObject>
#include <QString>

class LibrarySystem;

class BorrowControl : public QObject
{
    Q_OBJECT

private:
    LibrarySystem* librarySystem;

public:
    explicit BorrowControl(LibrarySystem* libSys, QObject* parent = nullptr);

signals:
    void borrowSuccessful(const QString& successMessage);
    void borrowFailed(const QString& reason);

public slots:
    void borrowRequested(int itemId);
};

#endif
