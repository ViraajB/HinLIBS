#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QPixmap>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QGridLayout;
class QScrollArea;
class CatalogCard;
class ItemDetailDialog;

struct CatalogItem {
    int id;
    QString title;
    QString author;
    QString details;
    QPixmap cover;   // placeholder solid color
};

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    // UI for browse grid
    QScrollArea* browseScroll_ = nullptr;
    QWidget*     browseContainer_ = nullptr;
    QGridLayout* browseGrid_ = nullptr;

    // data (UI-only)
    QVector<CatalogItem> items_;

    // helpers
    void showPage(int idx);
    void buildBrowseGrid();
    void populateItems();       // demo data + placeholder covers
    QPixmap makeCover(const QColor& c, const QString& text);
    QPixmap coverForTitle(const QString&  title) const;
};

#endif // MAINWINDOW_H
