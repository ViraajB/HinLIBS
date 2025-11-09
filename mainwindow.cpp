#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QListWidget>
#include <QListWidgetItem>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QPainter>

#include "itemdetaildialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("HinLIBS");

    // Account lists: show small cover icons
    if (ui->loanList) ui->loanList->setIconSize(QSize(40,55));
    if (ui->holdList) ui->holdList->setIconSize(QSize(40,55));

    // Demo dataset
    populateItems();

    // Build Browse page using Designer widgets (icon list + filter)
    initBrowseUi();

    // Start on Login page
    ui->stackedWidget->setCurrentWidget(ui->pageLogin);

    // ----- Navigation (UI-only) -----
    connect(ui->loginButton, &QPushButton::clicked, [this]{
        showPage(ui->stackedWidget->indexOf(ui->pageBrowse));
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

    // Account page no-ops
    connect(ui->returnBtn, &QPushButton::clicked, [this]{
        statusBar()->showMessage("UI-only: would return selected items.", 1500);
    });
    connect(ui->cancelHoldBtn, &QPushButton::clicked, [this]{
        statusBar()->showMessage("UI-only: would cancel selected holds.", 1500);
    });

    // Browse top bar buttons
    if (ui->browseAccountBtn) {
        connect(ui->browseAccountBtn, &QPushButton::clicked, [this]{
            ui->accountHeader->setText("<h3>Account: patronX — Patron</h3>");
            ui->loanList->clear();
            ui->holdList->clear();

            auto addWithIcon = [&](QListWidget* w, const QString& title, const QString& right){
                QPixmap pm = coverForTitle(title).scaled(
                    w->iconSize(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                auto* it = new QListWidgetItem(QIcon(pm), QString("%1 %2").arg(title, right));
                w->addItem(it);
            };

            addWithIcon(ui->loanList, "The Blue Planet", "(due 2025-12-31)");
            addWithIcon(ui->holdList, "Interstellar", "(pos 1)");

            showPage(ui->stackedWidget->indexOf(ui->pageAccount));
        });
    }
    if (ui->browseLogoutBtn) {
        connect(ui->browseLogoutBtn, &QPushButton::clicked, [this]{
            showPage(ui->stackedWidget->indexOf(ui->pageLogin));
        });
    }
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::showPage(int idx) {
    ui->stackedWidget->setCurrentIndex(idx);
}

// --------------------------
// Browse (icon list + filter)
// --------------------------
void MainWindow::initBrowseUi() {
    // 1) Ensure filter box has items (in case it's empty in Designer)
    if (ui->browseFilterBox && ui->browseFilterBox->count() == 0) {
        ui->browseFilterBox->addItems({
            "All", "Fiction Book", "Non-Fiction Book", "Magazine", "Movie", "Video Game"
        });
    }
    currentFilterIndex_ = ui->browseFilterBox ? ui->browseFilterBox->currentIndex() : 0;

    // 2) Set up the icon grid list
    if (ui->browseIconList) {
        ui->browseIconList->setViewMode(QListView::IconMode);
        ui->browseIconList->setFlow(QListView::LeftToRight);
        ui->browseIconList->setWrapping(true);
        ui->browseIconList->setResizeMode(QListView::Adjust);
        ui->browseIconList->setMovement(QListView::Static);
        ui->browseIconList->setSpacing(8);
        ui->browseIconList->setIconSize(QSize(96,128));
        ui->browseIconList->setGridSize(QSize(220,180)); // tile size; tweak to taste

        // open item details on double-click or Enter
        connect(ui->browseIconList, &QListWidget::itemActivated, this, [this](QListWidgetItem* item){
            if (!item) return;
            int id = item->data(Qt::UserRole).toInt();
            const CatalogItem* found = nullptr;
            for (const auto& it : items_) if (it.id == id) { found = &it; break; }
            if (!found) return;

            ItemDetailDialog dlg(this);
            dlg.setItem({found->id, found->title, found->author, found->details});

            connect(&dlg, &ItemDetailDialog::borrowRequested, [this](const ItemBrief& info){
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
    }

    // 3) Populate the list now
    populateBrowseIcons();

    // 4) Filter changes
    if (ui->browseFilterBox) {
        connect(ui->browseFilterBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, [this](int idx){
                    currentFilterIndex_ = idx;
                    populateBrowseIcons();
                });
    }
}

bool MainWindow::passFilter(const CatalogItem& it) const {
    // 0=All, 1=Fiction, 2=Non-Fiction, 3=Magazine, 4=Movie, 5=Video Game
    switch (currentFilterIndex_) {
    case 0: return true;
    case 1: return it.format == ItemFormat::FictionBook;
    case 2: return it.format == ItemFormat::NonFictionBook;
    case 3: return it.format == ItemFormat::Magazine;
    case 4: return it.format == ItemFormat::Movie;
    case 5: return it.format == ItemFormat::VideoGame;
    }
    return true;
}

void MainWindow::populateBrowseIcons() {
    if (!ui->browseIconList) return;
    ui->browseIconList->clear();

    for (const auto& it : items_) {
        if (!passFilter(it)) continue;

        QIcon icon(it.cover);
        auto* li = new QListWidgetItem(icon, it.title);
        li->setData(Qt::UserRole, it.id); // store id for lookup later
        ui->browseIconList->addItem(li);
    }
}

// --------------------------
// Demo data & utils
// --------------------------
void MainWindow::populateItems() {
    items_.clear();
    auto add = [&](int id, const QString& t, const QString& a, const QString& d,
                   const QColor& col, ItemFormat fmt){
        items_.push_back({id, t, a, d, makeCover(col, t.left(2).toUpper()), fmt});
    };

    add(1,  "City of Glass",            "Paul Auster",
        "Postmodern detective novel set in NYC.",
        QColor("#CDE7FF"), ItemFormat::FictionBook);

    add(2,  "Interstellar",             "Christopher Nolan",
        "Sci-fi epic. Genre: Sci-Fi, Rating: PG-13.",
        QColor("#FFEBCD"), ItemFormat::Movie);

    add(3,  "Galactic Weekly #12",      "Editorial",
        "Magazine, Issue #12 (2025-10).",
        QColor("#E9D5FF"), ItemFormat::Magazine);

    add(4,  "The Blue Planet",          "J. Smith",
        "Non-Fiction. Dewey 551.46 (Oceans).",
        QColor("#D1FAE5"), ItemFormat::NonFictionBook);

    add(5,  "Sky Legends",              "Dev Studio",
        "Video Game. Genre: Action; Rating: T.",
        QColor("#FDE68A"), ItemFormat::VideoGame);

    add(6,  "A Brief History of Time",  "Stephen Hawking",
        "Cosmology classic.",
        QColor("#FECACA"), ItemFormat::NonFictionBook);

    add(7,  "The Pragmatic Programmer", "Hunt & Thomas",
        "Software craftsmanship.",
        QColor("#FDE2E4"), ItemFormat::NonFictionBook);

    add(8,  "Clean Code",               "Robert C. Martin",
        "Readable, maintainable code.",
        QColor("#E6FFFA"), ItemFormat::NonFictionBook);

    add(9,  "Design Patterns",          "GoF",
        "Reusable OOP patterns.",
        QColor("#FFF1F2"), ItemFormat::NonFictionBook);

    add(10, "Structure & Interpretation","Abelson & Sussman",
        "SICP.",
        QColor("#F0F9FF"), ItemFormat::NonFictionBook);

    add(11, "Deep Work",                "Cal Newport",
        "Focus, productivity.",
        QColor("#FFF7ED"), ItemFormat::NonFictionBook);

    add(12, "Thinking, Fast and Slow",  "Daniel Kahneman",
        "System 1 / System 2.",
        QColor("#F1F5F9"), ItemFormat::NonFictionBook);
}

QPixmap MainWindow::makeCover(const QColor& c, const QString& text) {
    QPixmap pm(160, 220);
    pm.fill(c);
    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing);
    p.setBrush(QColor(0,0,0,25));
    p.setPen(Qt::NoPen);
    p.drawRect(0,0,160,40);
    p.setPen(QColor("#111"));
    QFont f = p.font(); f.setBold(true); f.setPointSize(22); p.setFont(f);
    p.drawText(QRect(0,80,160,60), Qt::AlignCenter, text);
    return pm;
}

QPixmap MainWindow::coverForTitle(const QString& title) const {
    for (const auto& it : items_)
        if (it.title == title) return it.cover;
    return QPixmap();
}
