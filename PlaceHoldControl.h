#ifndef PLACEHOLDCONTROL_H
#define PLACEHOLDCONTROL_H

#include <QObject>
#include <QString>
#include "LibrarySystem.h"

class LibrarySystem;

class PlaceHoldControl : public QObject
{
    Q_OBJECT

private:
    LibrarySystem* librarySystem;

public:
    explicit PlaceHoldControl(LibrarySystem* libSys, QObject* parent = nullptr);

signals:
    void placeHoldSuccessful(const QString& successMessage);
    void placeHoldFailed(const QString& reason);

public slots:
    void placeHoldRequested(int itemId);
};

#endif
