#ifndef CATALOGUECONTROL_H
#define CATALOGUECONTROL_H

#include <QObject>
#include <QVector>

class LibrarySystem;
class Item;

class CatalogueControl : public QObject
{
    Q_OBJECT

private:
    LibrarySystem* librarySystem;

public:
    explicit CatalogueControl(LibrarySystem* libSys, QObject* parent = nullptr);

signals:
    // ⭐ changed: non-const ref → const ref
    void catalogueDataReady(const QVector<Item*>& items);

public slots:
    void requestCatalogueData();
};

#endif
