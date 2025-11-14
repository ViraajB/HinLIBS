#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QPixmap>
#include <QPushButton>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class LibrarySystem;
class CatalogueControl;
class BorrowControl;
class PlaceHoldControl;
class ReturnControl;
class CancelHoldControl;
class ViewAccountStatusControl;
class Item;
class Loan;

enum class ItemFormat { FictionBook, NonFictionBook, Magazine, Movie, VideoGame };
enum class ActiveRole { None, Patron, Librarian, Admin };

struct CatalogItem {
    int id;
    QString title;
    QString author;
    QString details;
    QPixmap cover;
    ItemFormat format;
};

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onCatalogueDataReady(const QVector<Item*>& items);
    void onBorrowSuccess(const QString& msg);
    void onBorrowFailed(const QString& reason);

    void onPlaceHoldSuccess(const QString& msg);
    void onPlaceHoldFailed(const QString& reason);

    void onReturnSuccess(const QString& msg);
    void onReturnFailed(const QString& reason);

    void onCancelHoldSuccess(const QString& msg);
    void onCancelHoldFailed(const QString& reason);

    void onAccountStatusRetrieved(std::vector<Loan*> loans,
                                  std::vector<Item*> holds);

private:
    Ui::MainWindow *ui;

    QVector<CatalogItem> items_;
    int currentFilterIndex_ = 0;

    LibrarySystem* librarySystem_ = nullptr;
    CatalogueControl* catalogueControl_ = nullptr;
    BorrowControl* borrowControl_ = nullptr;
    PlaceHoldControl* placeHoldControl_ = nullptr;
    ReturnControl* returnControl_ = nullptr;
    CancelHoldControl* cancelHoldControl_ = nullptr;
    ViewAccountStatusControl* viewAccountStatusControl_ = nullptr;

    void showPage(int idx);
    void initBrowseUi();
    void populateBrowseIcons();
    bool passFilter(const CatalogItem& it) const;

    void populateItems();                  // (optional demo, not used once backend works)
    QPixmap makeCover(const QColor& c, const QString& text);
    QPixmap coverForTitle(const QString& title) const;
    ActiveRole activeRole_ = ActiveRole::None;
    QPushButton* editCatalogueBtn_ = nullptr;


};

#endif // MAINWINDOW_H
