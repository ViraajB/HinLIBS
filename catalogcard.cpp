
#include "catalogcard.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QSizePolicy>

CatalogCard::CatalogCard(QWidget* parent) : QFrame(parent) {
    setFrameShape(QFrame::StyledPanel);
    setFrameShadow(QFrame::Raised);
    setObjectName("catalogCard");
    setCursor(Qt::PointingHandCursor);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setMinimumWidth(200);

    coverLabel_ = new QLabel;
    coverLabel_->setFixedSize(160, 220);
    coverLabel_->setAlignment(Qt::AlignCenter);
    coverLabel_->setStyleSheet("background:#EEE;");

    titleLabel_ = new QLabel;
    titleLabel_->setWordWrap(true);
    titleLabel_->setAlignment(Qt::AlignHCenter);
    titleLabel_->setStyleSheet("font-weight:600;");

    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(10,10,10,10);
    root->setSpacing(8);
    root->addWidget(coverLabel_, 0, Qt::AlignHCenter);
    root->addWidget(titleLabel_);
}

void CatalogCard::setData(int id, const QString& title, const QPixmap& cover) {
    id_ = id;
    titleLabel_->setText(title);
    if (!cover.isNull()) {
        coverLabel_->setPixmap(cover.scaled(coverLabel_->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        // fallback placeholder
        QPixmap pm(160,220);
        pm.fill(QColor("#d9eaff"));
        coverLabel_->setPixmap(pm);
    }
}

void CatalogCard::mousePressEvent(QMouseEvent* e) {
    if (e->button() == Qt::LeftButton && id_ != -1) emit clicked(id_);
    QFrame::mousePressEvent(e);
}
