#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QPixmap>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum class ItemFormat { FictionBook, NonFictionBook, Magazine, Movie, VideoGame };

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

private:
    Ui::MainWindow *ui;

    QVector<CatalogItem> items_;

    int currentFilterIndex_ = 0;

    void showPage(int idx);
    void initBrowseUi();
    void populateBrowseIcons();
    bool passFilter(const CatalogItem& it) const;

    void populateItems();                  // demo content
    QPixmap makeCover(const QColor& c, const QString& text);
    QPixmap coverForTitle(const QString& title) const;
};

#endif // MAINWINDOW_H
