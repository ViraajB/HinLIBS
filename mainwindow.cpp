#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QListWidget>
#include <QListWidgetItem>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QPainter>

#include "itemdetaildialog.h"

#include "LibrarySystem.h"
#include "CatalogueControl.h"
#include "BorrowControl.h"
#include "PlaceHoldControl.h"
#include "ReturnControl.h"
#include "CancelHoldControl.h"
#include "ViewAccountStatusControl.h"
#include "Patron.h"
#include "Item.h"
#include "Loan.h"
#include "Librarian.h"
#include "FictionBook.h"
#include "NonFictionBook.h"
#include "Magazine.h"
#include "Movie.h"
#include "VideoGame.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("HinLIBS");
    editCatalogueBtn_ = ui->browseEditCatalogueBtn;
    if (editCatalogueBtn_) {
        editCatalogueBtn_->setVisible(false);
        connect(editCatalogueBtn_, &QPushButton::clicked, [this]{
            statusBar()->showMessage("Edit Catalogue (not implemented yet).", 2000);
        });
    }


    if (ui->loanList) ui->loanList->setIconSize(QSize(40,55));
    if (ui->holdList) ui->holdList->setIconSize(QSize(40,55));

    // Backend objects
    librarySystem_ = new LibrarySystem();
    catalogueControl_ = new CatalogueControl(librarySystem_, this);
    borrowControl_ = new BorrowControl(librarySystem_, this);
    placeHoldControl_ = new PlaceHoldControl(librarySystem_, this);
    returnControl_ = new ReturnControl(librarySystem_, this);
    cancelHoldControl_ = new CancelHoldControl(librarySystem_, this);
    viewAccountStatusControl_ = new ViewAccountStatusControl(librarySystem_, this);

    // Catalogue -> UI
    connect(catalogueControl_, &CatalogueControl::catalogueDataReady,
            this, &MainWindow::onCatalogueDataReady);

    // Borrow signals
    connect(borrowControl_, &BorrowControl::borrowSuccessful,
            this, &MainWindow::onBorrowSuccess);
    connect(borrowControl_, &BorrowControl::borrowFailed,
            this, &MainWindow::onBorrowFailed);

    // Hold signals
    connect(placeHoldControl_, &PlaceHoldControl::placeHoldSuccessful,
            this, &MainWindow::onPlaceHoldSuccess);
    connect(placeHoldControl_, &PlaceHoldControl::placeHoldFailed,
            this, &MainWindow::onPlaceHoldFailed);

    // Return signals
    connect(returnControl_, &ReturnControl::returnSuccessful,
            this, &MainWindow::onReturnSuccess);
    connect(returnControl_, &ReturnControl::returnFailed,
            this, &MainWindow::onReturnFailed);

    // Cancel hold signals
    connect(cancelHoldControl_, &CancelHoldControl::cancelHoldSuccessful,
            this, &MainWindow::onCancelHoldSuccess);
    connect(cancelHoldControl_, &CancelHoldControl::cancelHoldFailed,
            this, &MainWindow::onCancelHoldFailed);

    // Account status
    connect(viewAccountStatusControl_,
            &ViewAccountStatusControl::accountStatusRetrieved,
            this, &MainWindow::onAccountStatusRetrieved);

    initBrowseUi();

    ui->stackedWidget->setCurrentWidget(ui->pageLogin);

    // Login
    connect(ui->loginButton, &QPushButton::clicked, [this]{
        QString username;
        if (ui->usernameEdit) {
            username = ui->usernameEdit->text().trimmed();
        }

        if (username.isEmpty()) {
            statusBar()->showMessage("Enter username like patron1..patron5 or librarian1", 3000);
            return;
        }

        QString wanted = username.toLower();

        auto users = librarySystem_->getUsers();
        User* matchedUser = nullptr;

        for (auto* u : users) {
            QString backendName = QString::fromStdString(u->getName()).toLower();
            if (backendName == wanted) {
                matchedUser = u;
                break;
            }
        }

        if (!matchedUser) {
            statusBar()->showMessage("Unknown user. Use patron1..patron5 or librarian1", 4000);
            return;
        }

        librarySystem_->setCurrentUser(matchedUser);

        if (auto* p = dynamic_cast<Patron*>(matchedUser)) {
            activeRole_ = ActiveRole::Patron;
            setWindowTitle(QString("HinLIBS - Patron (%1)")
                               .arg(QString::fromStdString(p->getName())));
            if (editCatalogueBtn_) editCatalogueBtn_->setVisible(false);
        }
        else if (auto* lib = dynamic_cast<Librarian*>(matchedUser)) {
            activeRole_ = ActiveRole::Librarian;
            setWindowTitle(QString("HinLIBS - Librarian (%1)")
                               .arg(QString::fromStdString(lib->getName())));
            if (editCatalogueBtn_) editCatalogueBtn_->setVisible(true);
        }
        else {
            activeRole_ = ActiveRole::None;
            setWindowTitle("HinLIBS");
            if (editCatalogueBtn_) editCatalogueBtn_->setVisible(false);
        }


        showPage(ui->stackedWidget->indexOf(ui->pageBrowse));
        catalogueControl_->requestCatalogueData();
    });


    //  Page back buttons
    connect(ui->checkoutBackBtn, &QPushButton::clicked, [this]{
        showPage(ui->stackedWidget->indexOf(ui->pageBrowse));
    });
    connect(ui->holdBackBtn, &QPushButton::clicked, [this]{
        showPage(ui->stackedWidget->indexOf(ui->pageBrowse));
    });
    connect(ui->accountBackBtn, &QPushButton::clicked, [this]{
        showPage(ui->stackedWidget->indexOf(ui->pageBrowse));
    });

    //Return & Cancel hold buttons
    connect(ui->returnBtn, &QPushButton::clicked, [this]{
        if (!ui->loanList) return;
        auto selected = ui->loanList->selectedItems();
        for (auto* it : selected) {
            int id = it->data(Qt::UserRole).toInt();
            returnControl_->returnRequested(id);
        }
    });

    connect(ui->cancelHoldBtn, &QPushButton::clicked, [this]{
        if (!ui->holdList) return;
        auto selected = ui->holdList->selectedItems();
        for (auto* it : selected) {
            int id = it->data(Qt::UserRole).toInt();
            cancelHoldControl_->cancelHoldRequested(id);
        }
    });

    // Browse → Account page
    if (ui->browseAccountBtn) {
        connect(ui->browseAccountBtn, &QPushButton::clicked, [this]{
            // trigger backend to fetch current account status
            viewAccountStatusControl_->viewAccountStatusRequested();
        });
    }
    if (ui->browseLogoutBtn) {
        connect(ui->browseLogoutBtn, &QPushButton::clicked, [this]{
            librarySystem_->setCurrentUser(nullptr);
            activeRole_ = ActiveRole::None;
            setWindowTitle("HinLIBS");
            if (editCatalogueBtn_) editCatalogueBtn_->setVisible(false);
            showPage(ui->stackedWidget->indexOf(ui->pageLogin));
        });

    }
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::showPage(int idx) {
    ui->stackedWidget->setCurrentIndex(idx);
}

//Browse / catalogue UI

void MainWindow::initBrowseUi() {
    if (ui->browseFilterBox && ui->browseFilterBox->count() == 0) {
        ui->browseFilterBox->addItems({
            "All", "Fiction Book", "Non-Fiction Book", "Magazine", "Movie", "Video Game"
        });
    }
    currentFilterIndex_ = ui->browseFilterBox ? ui->browseFilterBox->currentIndex() : 0;
    if (ui->browseFilterBox) {
        connect(ui->browseFilterBox,
                QOverload<int>::of(&QComboBox::currentIndexChanged),
                this,
                [this](int idx){
                    currentFilterIndex_ = idx;
                    populateBrowseIcons();      // re-filter & redraw
                });
    }


    if (ui->browseIconList) {
        ui->browseIconList->setViewMode(QListView::IconMode);
        ui->browseIconList->setFlow(QListView::LeftToRight);
        ui->browseIconList->setWrapping(true);
        ui->browseIconList->setResizeMode(QListView::Adjust);
        ui->browseIconList->setMovement(QListView::Static);
        ui->browseIconList->setSpacing(8);
        ui->browseIconList->setIconSize(QSize(96,128));
        ui->browseIconList->setGridSize(QSize(220,180));

        connect(ui->browseIconList, &QListWidget::itemActivated, this, [this](QListWidgetItem* item){
            if (!item) return;
            int id = item->data(Qt::UserRole).toInt();
            const CatalogItem* found = nullptr;
            for (const auto& it : items_) if (it.id == id) { found = &it; break; }
            if (!found) return;

            ItemDetailDialog dlg(this);
            dlg.setItem({found->id, found->title, found->author, found->details});

            // Borrow
            connect(&dlg, &ItemDetailDialog::borrowRequested,
                    [this](const ItemBrief& info){
                        borrowControl_->borrowRequested(info.id);
                    });

            // Hold
            connect(&dlg, &ItemDetailDialog::holdRequested,
                    [this](const ItemBrief& info){
                        placeHoldControl_->placeHoldRequested(info.id);
                    });

            dlg.exec();
        });
    }
}

// Backend -> UI catalogue
void MainWindow::onCatalogueDataReady(const QVector<Item*>& items)
{
    items_.clear();

    for (Item* baseItem : items) {
        if (!baseItem) continue;

        CatalogItem ci;
        ci.id     = baseItem->getId();
        ci.title  = QString::fromStdString(baseItem->getTitle());
        ci.author = QString::fromStdString(baseItem->getAuthor());
        ci.details = QString::fromStdString(baseItem->getDetail());


        QString fmtStr = QString::fromStdString(baseItem->getFormat());
        ci.details = QString("Format: %1").arg(fmtStr);

        QString details;
        ItemFormat fmt;

        // std::string avail = baseItem->getAvailabilityStatus() ? "Availaible" : "Not Availaible";

        if (auto* nf = dynamic_cast<NonFictionBook*>(baseItem)) {
            fmt     = ItemFormat::NonFictionBook;
            details = QString("Non-Fiction Book\nDewey: %1")
                          .arg(QString::fromStdString(nf->getDeweyDecimal()));
        }
        else if (auto* mag = dynamic_cast<Magazine*>(baseItem)) {
            fmt = ItemFormat::Magazine;

            bool avail = baseItem->getAvailabilityStatus();    // bool -> string
            QString availStr = avail ? "Available" : "On loan";

            details = QString("Magazine\nIssue #: %1\nPublication date: %2\nAvailability: %3")
                          .arg(mag->getIssueNumber())
                          .arg(QString::fromStdString(mag->getPublicationDate()))
                          .arg(availStr);
        }
        else if (auto* mov = dynamic_cast<Movie*>(baseItem)) {
            fmt     = ItemFormat::Movie;
            details = QString("Movie\nGenre: %1\nRating: %2")
                          .arg(QString::fromStdString(mov->getGenre()))
                          .arg(QString::fromStdString(mov->getRating()));
        }
        else if (auto* vg = dynamic_cast<VideoGame*>(baseItem)) {
            fmt     = ItemFormat::VideoGame;
            details = QString("Video Game\nGenre: %1\nRating: %2")
                          .arg(QString::fromStdString(vg->getGenre()))
                          .arg(QString::fromStdString(vg->getRating()));
        }
        else if (dynamic_cast<FictionBook*>(baseItem)) {
            fmt     = ItemFormat::FictionBook;
            details = "Fiction Book";
        } else {
            // fallback
            fmt     = ItemFormat::FictionBook;
            details = QString::fromStdString(baseItem->getFormat());
        }

        ci.details = details;
        ci.format  = fmt;
        QColor col;   // ✅ declare it once

        switch (fmt) {
        case ItemFormat::FictionBook:
            col = QColor(205, 231, 255);
            break;
        case ItemFormat::NonFictionBook:
            col = QColor(209, 250, 229);
            break;
        case ItemFormat::Magazine:
            col = QColor(233, 213, 255);
            break;
        case ItemFormat::Movie:
            col = QColor(255, 235, 205);
            break;
        case ItemFormat::VideoGame:
            col = QColor(253, 230, 138);
            break;
        }


        ci.cover  = makeCover(col, ci.title.left(2).toUpper());

        items_.push_back(ci);
    }

    populateBrowseIcons();
}

bool MainWindow::passFilter(const CatalogItem& it) const {
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
        li->setData(Qt::UserRole, it.id);
        ui->browseIconList->addItem(li);
    }
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

//Borrow result handlers

void MainWindow::onBorrowSuccess(const QString& msg)
{
    // Get current patron and find the last loan (or the loan for any item)
    Patron* p = dynamic_cast<Patron*>(librarySystem_->getCurrentUser());
    QString extra;

    if (p) {
        auto loans = p->getActiveLoans();
        if (!loans.empty()) {
            Loan* last = loans.back();
            int days = last->calculateDaysRemaining();
            int remaining = LibrarySystem::MAX_LOANS_PER_PATRON - static_cast<int>(loans.size());
            extra = QString("\nDue in %1 day(s).\nBorrows left: %2")
                        .arg(days).arg(remaining);
        }
    }

    ui->checkoutMsg->setText(msg + extra);
    showPage(ui->stackedWidget->indexOf(ui->pageCheckout));
}

void MainWindow::onBorrowFailed(const QString& reason)
{
    ui->checkoutMsg->setText("Borrow failed:\n" + reason);
    showPage(ui->stackedWidget->indexOf(ui->pageCheckout));
}

//Hold result handler

void MainWindow::onPlaceHoldSuccess(const QString& msg)
{
    Patron* p = dynamic_cast<Patron*>(librarySystem_->getCurrentUser());
    int pos = -1;

    if (p) {
        auto holds = p->getActiveHolds();
        if (!holds.empty()) {
            Item* last = holds.back();
            pos = last->getQueuePosition(p);
        }
    }

    QString extra;
    if (pos > 0) extra = QString("\nYour position in queue: %1").arg(pos);

    ui->holdMsg->setText(msg + extra);
    showPage(ui->stackedWidget->indexOf(ui->pageHold));
}

void MainWindow::onPlaceHoldFailed(const QString& reason)
{
    ui->holdMsg->setText("Hold failed:\n" + reason);
    showPage(ui->stackedWidget->indexOf(ui->pageHold));
}

// Return / Cancel hold handlers

void MainWindow::onReturnSuccess(const QString& msg)
{
    statusBar()->showMessage(msg, 2000);
    viewAccountStatusControl_->viewAccountStatusRequested();
}

void MainWindow::onReturnFailed(const QString& reason)
{
    statusBar()->showMessage("Return failed: " + reason, 3000);
}

void MainWindow::onCancelHoldSuccess(const QString& msg)
{
    statusBar()->showMessage(msg, 2000);
    viewAccountStatusControl_->viewAccountStatusRequested();
}

void MainWindow::onCancelHoldFailed(const QString& reason)
{
    statusBar()->showMessage("Cancel hold failed: " + reason, 3000);
}

// Account status handler

void MainWindow::onAccountStatusRetrieved(std::vector<Loan*> loans,
                                          std::vector<Item*> holds)
{
    User* currentUser = librarySystem_->getCurrentUser();
    Patron* p = dynamic_cast<Patron*>(currentUser);

    ui->loanList->clear();
    ui->holdList->clear();

    // Patron mode: current behaviour
    if (activeRole_ == ActiveRole::Patron && p) {
        QString header = QString("<h3>Account: %1 — Patron</h3>")
                             .arg(QString::fromStdString(p->getName()));
        ui->accountNameLabel->setText(header);

        // Active loans
        for (Loan* loan : loans) {
            if (!loan) continue;
            Item* it = loan->getItem();
            if (!it) continue;

            QString title = QString::fromStdString(it->getTitle());
            int days = loan->calculateDaysRemaining();
            QString right = QString("(due in %1 day(s))").arg(days);

            QPixmap pm = coverForTitle(title).scaled(
                ui->loanList->iconSize(),
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation
                );
            auto* li = new QListWidgetItem(QIcon(pm),
                                           QString("%1 %2").arg(title, right));
            li->setData(Qt::UserRole, it->getId());     // ⭐ still store ID for return
            ui->loanList->addItem(li);
        }

        // Active holds
        for (Item* it : holds) {
            if (!it) continue;
            QString title = QString::fromStdString(it->getTitle());
            int pos = it->getQueuePosition(p);
            QString right = QString("(pos %1)").arg(pos);

            QPixmap pm = coverForTitle(title).scaled(
                ui->holdList->iconSize(),
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation
                );
            auto* li = new QListWidgetItem(QIcon(pm),
                                           QString("%1 %2").arg(title, right));
            li->setData(Qt::UserRole, it->getId());  // ⭐ still store ID for cancel hold
            ui->holdList->addItem(li);
        }
    }
    // Librarian mode: all patrons' loans/holds
    else if (activeRole_ == ActiveRole::Librarian) {
        QString name = currentUser
                           ? QString::fromStdString(currentUser->getName())
                           : QString("Librarian");
        QString header = QString("<h3>Account: %1 — Librarian (All Patrons)</h3>")
                             .arg(name);
        ui->accountNameLabel->setText(header);

        auto users = librarySystem_->getUsers();

        // All loans from all patrons
        for (User* u : users) {
            Patron* patron = dynamic_cast<Patron*>(u);
            if (!patron) continue;

            QString patronName = QString::fromStdString(patron->getName());
            auto patronLoans = patron->getActiveLoans();

            for (Loan* loan : patronLoans) {
                if (!loan) continue;
                Item* it = loan->getItem();
                if (!it) continue;

                QString title = QString::fromStdString(it->getTitle());
                int days = loan->calculateDaysRemaining();
                QString right = QString("(due in %1 day(s))").arg(days);

                QPixmap pm = coverForTitle(title).scaled(
                    ui->loanList->iconSize(),
                    Qt::KeepAspectRatio,
                    Qt::SmoothTransformation
                    );
                auto* li = new QListWidgetItem(
                    QIcon(pm),
                    QString("%1 — %2 %3").arg(patronName, title, right)
                    );

                ui->loanList->addItem(li);
            }
        }

        // All holds from all patrons
        for (User* u : users) {
            Patron* patron = dynamic_cast<Patron*>(u);
            if (!patron) continue;

            QString patronName = QString::fromStdString(patron->getName());
            auto patronHolds = patron->getActiveHolds();

            for (Item* it : patronHolds) {
                if (!it) continue;

                QString title = QString::fromStdString(it->getTitle());
                int pos = it->getQueuePosition(patron);
                QString right = QString("(pos %1)").arg(pos);

                QPixmap pm = coverForTitle(title).scaled(
                    ui->holdList->iconSize(),
                    Qt::KeepAspectRatio,
                    Qt::SmoothTransformation
                    );
                auto* li = new QListWidgetItem(
                    QIcon(pm),
                    QString("%1 — %2 %3").arg(patronName, title, right)
                    );
                ui->holdList->addItem(li);
            }
        }
    }
    else {
        ui->accountNameLabel->setText("<h3>Account: (not logged in)</h3>");
    }

    showPage(ui->stackedWidget->indexOf(ui->pageAccount));
}
