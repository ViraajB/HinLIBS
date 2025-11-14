#ifndef CANCELHOLDCONTROL_H
#define CANCELHOLDCONTROL_H

#include <QObject>
#include <QString>
#include "LibrarySystem.h"

class LibrarySystem;

class CancelHoldControl : public QObject
{
    Q_OBJECT

private:
    LibrarySystem* librarySystem;

public:
    explicit CancelHoldControl(LibrarySystem* libSys, QObject* parent = nullptr);

signals:
    void cancelHoldSuccessful(const QString& successMessage);
    void cancelHoldFailed(const QString& reason);

public slots:
    void cancelHoldRequested(int itemId);
};

#endif
