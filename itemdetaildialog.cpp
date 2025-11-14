
#include "itemdetaildialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

ItemDetailDialog::ItemDetailDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Item Details");

    title_ = new QLabel;   title_->setStyleSheet("font-size:16px; font-weight:700;");
    author_ = new QLabel;  author_->setStyleSheet("color:#444;");
    detail_ = new QLabel; detail_->setWordWrap(true);

    borrowBtn_ = new QPushButton("Borrow");
    holdBtn_   = new QPushButton("Place Hold");
    auto* closeBtn = new QPushButton("Close");

    auto* btnRow = new QHBoxLayout;
    btnRow->addStretch();
    btnRow->addWidget(borrowBtn_);
    btnRow->addWidget(holdBtn_);
    btnRow->addWidget(closeBtn);

    auto* root = new QVBoxLayout(this);
    root->addWidget(title_);
    root->addWidget(author_);
    root->addSpacing(6);
    root->addWidget(detail_);
    root->addStretch();
    root->addLayout(btnRow);

    connect(closeBtn, &QPushButton::clicked, this, &QDialog::close);
    // Emit signals when buttons are clicked (MainWindow decides navigation)
    connect(borrowBtn_, &QPushButton::clicked, [this]{

        emit borrowRequested(current_);
        close();
    });
    connect(holdBtn_, &QPushButton::clicked, [this]{

        emit holdRequested(current_);
        close();
    });
}

void ItemDetailDialog::setItem(const ItemBrief& info) {
    // ⭐ store for later so borrowRequested/holdRequested send correct data
    current_ = info;

    title_->setText(info.title);
    author_->setText(QString("by %1").arg(info.author));
    detail_->setText(info.detail);

}
