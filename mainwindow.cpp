#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QScrollArea>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QStyleOption>

#include "catalogcard.h"
#include "itemdetaildialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->loanList->setIconSize(QSize(40,55));
    ui->holdList->setIconSize(QSize(40,55));
    setWindowTitle("HinLIBS");

    populateItems();

    buildBrowseGrid();

    ui->stackedWidget->setCurrentWidget(ui->pageLogin);

    connect(ui->loginButton, &QPushButton::clicked, [this]{
        showPage(ui->stackedWidget->indexOf(ui->pageBrowse));
    });

    connect(ui->accountBtn, &QPushButton::clicked, [this]{
        ui->accountHeader->setText("<h3>Account: patronX — Patron</h3>");

        ui->loanList->clear();
        ui->holdList->clear();

        auto addLoan = [&](const QString& title, const QString& due){
            QPixmap pm = coverForTitle(title).scaled(
                ui->loanList->iconSize(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            auto *it = new QListWidgetItem(QIcon(pm), QString("%1 (due %2)").arg(title, due));
            ui->loanList->addItem(it);
        };

        auto addHold = [&](const QString& title, int pos){
            QPixmap pm = coverForTitle(title).scaled(
                ui->holdList->iconSize(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            auto *it = new QListWidgetItem(QIcon(pm), QString("%1 (pos %2)").arg(title).arg(pos));
            ui->holdList->addItem(it);
        };

        addLoan("The Blue Planet", "2025-12-31");
        addHold("Interstellar", 1);

        showPage(ui->stackedWidget->indexOf(ui->pageAccount));
    });


    connect(ui->backToLoginBtn, &QPushButton::clicked, [this]{
        showPage(ui->stackedWidget->indexOf(ui->pageLogin));
    });

    connect(ui->checkoutBackBtn, &QPushButton::clicked, [this]{
        showPage(ui->stackedWidget->indexOf(ui->pageBrowse));
    });
    connect(ui->holdBackBtn, &QPushButton::clicked, [this]{
        showPage(ui->stackedWidget->indexOf(ui->pageBrowse));
    });
    connect(ui->accountBackBtn, &QPushButton::clicked, [this]{
        showPage(ui->stackedWidget->indexOf(ui->pageBrowse));
    });

    connect(ui->returnBtn, &QPushButton::clicked, [this]{
        statusBar()->showMessage("UI-only: would return selected items.",1500);
    });
    connect(ui->cancelHoldBtn, &QPushButton::clicked, [this]{
        statusBar()->showMessage("UI-only: would cancel selected holds.",1500);
    });
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::showPage(int idx) {
    ui->stackedWidget->setCurrentIndex(idx);
}

void MainWindow::buildBrowseGrid() {
    if (browseScroll_) return;

    browseScroll_ = new QScrollArea(ui->pageBrowse);
    browseScroll_->setWidgetResizable(true);

    browseContainer_ = new QWidget;
    browseGrid_ = new QGridLayout(browseContainer_);
    browseGrid_->setContentsMargins(12,12,12,12);
    browseGrid_->setHorizontalSpacing(16);
    browseGrid_->setVerticalSpacing(16);

    const int cols = 3;
    for (int i=0;i<items_.size();++i) {
        const auto& it = items_[i];
        auto* card = new CatalogCard;
        card->setData(it.id, it.title, it.cover);

        connect(card, &CatalogCard::clicked, [this, it](int){
            ItemDetailDialog dlg(this);
            dlg.setItem({it.id, it.title, it.author, it.details});

            connect(&dlg, &ItemDetailDialog::borrowRequested, [this](const ItemBrief& info){
                // UI-only: show Checkout placeholder and navigate
                ui->checkoutMsg->setText(
                    QString("Checkout preview (UI-only)\n"
                            "Title: %1\n"
                            "Due date: 2025-12-31\n"
                            "Borrows left: 3").arg(info.title));
                showPage(ui->stackedWidget->indexOf(ui->pageCheckout));
            });
            connect(&dlg, &ItemDetailDialog::holdRequested, [this](const ItemBrief& info){
                ui->holdMsg->setText(
                    QString("Hold preview (UI-only)\n"
                            "Title: %1\n"
                            "Your position in queue: 1").arg(info.title));
                showPage(ui->stackedWidget->indexOf(ui->pageHold));
            });

            dlg.exec();
        });

        int r = i / cols;
        int c = i % cols;
        browseGrid_->addWidget(card, r, c);
    }

    browseScroll_->setWidget(browseContainer_);
    auto* pageLayout = new QVBoxLayout(ui->pageBrowse);
    pageLayout->setContentsMargins(8,8,8,8);

    pageLayout->addWidget(browseScroll_, 1);
}

void MainWindow::populateItems() {
    // build 12 sample items with colored covers
    items_.clear();
    auto add = [&](int id, const QString& t, const QString& a, const QString& d, const QColor& col){
        items_.push_back({id, t, a, d, makeCover(col, t.left(2).toUpper())});
    };
    add(1, "City of Glass", "Paul Auster", "Postmodern detective novel set in NYC.", QColor("#CDE7FF"));
    add(2, "Interstellar", "Christopher Nolan", "Sci-fi epic. Genre: Sci-Fi, Rating: PG-13.", QColor("#FFEBCD"));
    add(3, "Galactic Weekly #12", "Editorial", "Magazine, Issue #12 (2025-10).", QColor("#E9D5FF"));
    add(4, "The Blue Planet", "J. Smith", "Non-Fiction. Dewey 551.46 (Oceans).", QColor("#D1FAE5"));
    add(5, "Sky Legends", "Dev Studio", "Video Game. Genre: Action; Rating: T.", QColor("#FDE68A"));
    add(6, "A Brief History of Time", "Stephen Hawking", "Cosmology classic.", QColor("#FECACA"));
    add(7, "The Pragmatic Programmer", "Hunt & Thomas", "Software craftsmanship.", QColor("#FDE2E4"));
    add(8, "Clean Code", "Robert C. Martin", "Readable, maintainable code.", QColor("#E6FFFA"));
    add(9, "Design Patterns", "GoF", "Reusable OOP patterns.", QColor("#FFF1F2"));
    add(10,"Structure & Interpretation", "Abelson & Sussman", "SICP.", QColor("#F0F9FF"));
    add(11,"Deep Work", "Cal Newport", "Focus, productivity.", QColor("#FFF7ED"));
    add(12,"Thinking, Fast and Slow", "Daniel Kahneman", "System 1 / System 2.", QColor("#F1F5F9"));
}

QPixmap MainWindow::makeCover(const QColor& c, const QString& text) {
    QPixmap pm(160,220);
    pm.fill(c);
    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::NoPen);

    p.setBrush(QColor(0,0,0,25));
    p.drawRoundedRect(QRectF(0,0,160,40), 0, 0);

    p.setPen(QColor("#111"));
    QFont f = p.font(); f.setBold(true); f.setPointSize(22); p.setFont(f);
    p.drawText(QRect(0,80,160,60), Qt::AlignCenter, text);
    return pm;
}

QPixmap MainWindow::coverForTitle(const QString &t) const{
    for (const auto& it:items_){
        if (it.title == t){
            return  it.cover;
        }
    }
    return QPixmap();
}
