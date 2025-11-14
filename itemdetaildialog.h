
#pragma once
#include <QDialog>

struct ItemBrief {
    int id;
    QString title;
    QString author;
    QString detail;
};

class QLabel;
class QPushButton;

class ItemDetailDialog : public QDialog {
    Q_OBJECT
public:
    explicit ItemDetailDialog(QWidget* parent=nullptr);
    void setItem(const ItemBrief& info);

signals:
    void borrowRequested(const ItemBrief& info);
    void holdRequested(const ItemBrief& info);

private:
    QLabel* title_;
    QLabel* author_;
    QLabel* detail_;
    QPushButton* borrowBtn_;
    QPushButton* holdBtn_;

    ItemBrief current_;
};
