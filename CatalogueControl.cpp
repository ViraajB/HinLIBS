#include "CatalogueControl.h"
#include "LibrarySystem.h"
#include "Item.h"

#include <vector>

CatalogueControl::CatalogueControl(LibrarySystem* libSys, QObject* parent)
    : QObject(parent),
    librarySystem(libSys)
{
}

void CatalogueControl::requestCatalogueData()
{
    if (!librarySystem) {
        QVector<Item*> empty;
        emit catalogueDataReady(empty);
        return;
    }

    // LibrarySystem returns std::vector<Item*>
    std::vector<Item*> catalogueStd = librarySystem->getCatalogue();

    // Manually convert std::vector<Item*> -> QVector<Item*>
    QVector<Item*> catalogueQt;
    catalogueQt.reserve(static_cast<int>(catalogueStd.size()));
    for (Item* it : catalogueStd) {
        catalogueQt.push_back(it);
    }

    // Emit by const-reference (see next section)
    emit catalogueDataReady(catalogueQt);
}
