#ifndef VIEWACCOUNTSTATUSCONTROL_H
#define VIEWACCOUNTSTATUSCONTROL_H

#include <QObject>
#include <vector>      // ⭐ use STL vector, not "vector"

class LibrarySystem;
class Loan;
class Item;

class ViewAccountStatusControl : public QObject
{
    Q_OBJECT

private:
    LibrarySystem* librarySystem;

public:
    explicit ViewAccountStatusControl(LibrarySystem* libSys, QObject* parent = nullptr);

signals:
    void accountStatusRetrieved(std::vector<Loan*> activeLoans,
                                std::vector<Item*> activeHolds);

public slots:
    void viewAccountStatusRequested();
};

#endif
