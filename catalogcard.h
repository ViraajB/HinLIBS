#pragma once
#include <QFrame>
#include <QPixmap>

class QLabel;

class CatalogCard : public QFrame {
    Q_OBJECT
public:
    explicit CatalogCard(QWidget* parent=nullptr);
    void setData(int id, const QString& title, const QPixmap& cover);

signals:
    void clicked(int id);

protected:
    void mousePressEvent(QMouseEvent* e) override;

private:
    int id_ = -1;
    QLabel* coverLabel_;
    QLabel* titleLabel_;
};


