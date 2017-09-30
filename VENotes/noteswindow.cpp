#include "noteswindow.h"
#include "ui_noteswindow.h"

struct note {
    QString nameOfNote;
    QString textOfNote;
    QString dateOfChanges;
    QString dateOfNotification = "0";
    int ID = -1;
};
int openID = -1;
QString noteLocation = "resources/Notes/";
QString username = "";
QList<note> notesList;
QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
QRegExp validExp("\\b[a-zA-Z0-9]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,4}\\b");

NotesWindow::NotesWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NotesWindow)
{
    ui->setupUi(this);

    QSqlQuery query;
    query.exec("SELECT * FROM users");
    int res = 0;
    while (query.next())
        res++;
    if(res == 0)
        registration();
    else
        //authorization();
        showNotes();
}

NotesWindow::~NotesWindow()
{
    delete ui;
}

bool NotesWindow::connectDB()
{
    db.setDatabaseName(qApp->applicationDirPath() + "/users_data.db");

    if(!db.open())
    {
        QMessageBox::critical(NULL, tr("Помилка"), "Не вдалося під`єднатися до бази даних!");
        db.close();
        return false;
    }
    else
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM users");
        if(!query.exec())
        {
            query.exec("CREATE TABLE users (id INTEGER PRIMARY KEY UNIQUE NOT NULL, login STRING NOT NULL, password STRING NOT NULL, email STRING NOT NULL)");
            return true;
        }
    }
}

void NotesWindow::registration()
{
    ui->stackedWidget->setCurrentIndex(1);
    this->setFixedWidth(310);
    ui->buttonBox_in_reg->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->buttonBox_in_reg->button(QDialogButtonBox::Ok)->setDefault(true);
    ui->buttonBox_in_reg->button(QDialogButtonBox::Ok)->setText(tr("Створити"));
    ui->buttonBox_in_reg->button(QDialogButtonBox::Cancel)->setText(tr("Скасувати"));

    ui->pushButton_auth->setCursor(Qt::PointingHandCursor);

    validator = new QRegExpValidator(validExp, ui->lineEdit_reg_email);
    ui->lineEdit_reg_email->setValidator(validator);

    QSqlQuery query;
    query.exec("SELECT * FROM users");
    int res = 0;
    while (query.next())
        res++;
    if(res == 0)
    {
        this->setFixedHeight(563);
        ui->pushButton_auth->hide();
    }
    else this->setFixedHeight(548 + ui->pushButton_auth->height());
    connect(ui->lineEdit_reg_log, SIGNAL(textChanged(QString)), this, SLOT(setEnabledToRegOk()));
    connect(ui->lineEdit_reg_pass, SIGNAL(textChanged(QString)), this, SLOT(setEnabledToRegOk()));
    connect(ui->lineEdit_reg_pass_check, SIGNAL(textChanged(QString)), this, SLOT(setEnabledToRegOk()));
    connect(ui->lineEdit_reg_email, SIGNAL(textChanged(QString)), this, SLOT(setEnabledToRegOk()));
    connect( ui->buttonBox_in_reg->button(QDialogButtonBox::Ok), SIGNAL(clicked(bool)), this, SLOT(createAccount()));
}

void NotesWindow::authorization()
{
    ui->stackedWidget->setCurrentIndex(0);
    this->setFixedWidth(310);
    this->setFixedHeight(470);
    ui->buttonBox_in_auth->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->buttonBox_in_auth->button(QDialogButtonBox::Ok)->setDefault(true);
    ui->buttonBox_in_auth->button(QDialogButtonBox::Ok)->setText(tr("Увійти"));
    ui->buttonBox_in_auth->button(QDialogButtonBox::Cancel)->setText(tr("Скасувати"));

    ui->pushButton_new_account->setCursor(Qt::PointingHandCursor);
    ui->pushButton_remind_account->setCursor(Qt::PointingHandCursor);

    validator = new QRegExpValidator(validExp, ui->lineEdit_auth_email);
    ui->lineEdit_auth_email->setValidator(validator);

    connect(ui->lineEdit_auth_email, SIGNAL(textChanged(QString)), this, SLOT(setEnabledToAuthOk()));
    connect(ui->lineEdit_auth_pass, SIGNAL(textChanged(QString)), this, SLOT(setEnabledToAuthOk()));
    connect( ui->buttonBox_in_auth->button(QDialogButtonBox::Ok), SIGNAL(clicked(bool)), this, SLOT(loginInAccount()));
}

void NotesWindow::createAccount()
{
    QString message;
    if(ui->lineEdit_reg_pass->text().compare(ui->lineEdit_reg_pass_check->text()))
        message += "Паролі не співпадають.\n";
    QSqlQuery query(db);
    query.prepare("SELECT * FROM users WHERE email = :email");
    query.bindValue(":email", ui->lineEdit_reg_email->text());
    query.exec();
    int res = 0;
    while (query.next())
        res++;
    if(res)
        message += "Обліковий запис з таким email вже існує.\n";
    if(!message.isEmpty())
        QMessageBox::warning(this, tr("Помилка реєстрації"), message + "Повторіть введення даних!");
    else
    {
        QSqlQuery query(db);
        query.prepare("INSERT INTO users (login, password, email) VALUES (:login, :password, :email)");
        query.bindValue(":login", ui->lineEdit_reg_log->text());
        query.bindValue(":password", QCryptographicHash::hash(ui->lineEdit_reg_pass->text().toUtf8(), QCryptographicHash::Md5).toHex());
        query.bindValue(":email", ui->lineEdit_reg_email->text());
        if(!query.exec())
            QMessageBox::warning(this, tr("Помилка реєстрації"), "Не вдалося під`єднатися до бази даних!");
        else
        {
            QMessageBox::information(this, tr("Реєстрацію завершено"), "Новий обліковий запис було створено.\nВітаємо, " + ui->lineEdit_reg_log->text() + "!");
            username = ui->lineEdit_reg_log->text();
            this->setFixedSize(sizeHint());
            showNotes();
        }
    }
}

void NotesWindow::loginInAccount()
{
    QSqlQuery query(db);
    query.prepare("SELECT login FROM users WHERE email = :email AND password = :password");
    query.bindValue(":password", QCryptographicHash::hash(ui->lineEdit_auth_pass->text().toUtf8(), QCryptographicHash::Md5).toHex());
    query.bindValue(":email", ui->lineEdit_auth_email->text());
    if(!query.exec())
        QMessageBox::warning(this, tr("Помилка авторизації"), "Не вдалося під`єднатися до бази даних!");
    else
    {
        int res = 0;
        while (query.next())
            res++;
        if(res == 0)
            QMessageBox::warning(this, tr("Помилка авторизації"), "Не знайдено вказаний обліковий запис!");
        else
        {
            query.previous();
            QString name = query.value(0).toString();
            QMessageBox::information(this, tr("Авторизацію завершено"), "Вітаємо, " + name + "!");
            username = name;
            this->setFixedSize(sizeHint());
            showNotes();
        }
    }
}

void NotesWindow::setEnabledToRegOk()
{
    if(!ui->lineEdit_reg_log->text().isEmpty() && !ui->lineEdit_reg_pass->text().isEmpty() && !ui->lineEdit_reg_pass_check->text().isEmpty() && ui->lineEdit_reg_email->hasAcceptableInput())
        ui->buttonBox_in_reg->button(QDialogButtonBox::Ok)->setEnabled(true);
    else ui->buttonBox_in_reg->button(QDialogButtonBox::Ok)->setEnabled(false);
}

void NotesWindow::setEnabledToAuthOk()
{
    if(ui->lineEdit_auth_email->hasAcceptableInput() && !ui->lineEdit_auth_pass->text().isEmpty())
        ui->buttonBox_in_auth->button(QDialogButtonBox::Ok)->setEnabled(true);
    else
        ui->buttonBox_in_auth->button(QDialogButtonBox::Ok)->setEnabled(false);
}

void NotesWindow::on_pushButton_new_account_clicked()
{
    ui->pushButton_new_account->clearFocus();
    registration();
}

void NotesWindow::readXML() {
    notesList.clear();
    QFile readFrom(noteLocation + username + ".txt");
    readFrom.open(QIODevice::ReadOnly);
    QString str = "";
    note bufForNotes;
    while(!readFrom.atEnd()) {
        QString buf;
        buf = readFrom.readLine();
        if(buf.contains("<nameOfNote>")) {
            str.clear();
        }
        else if(buf.contains("<textOfNote>")) {
            bufForNotes.nameOfNote = str.mid(0, str.indexOf('\n'));
            str.clear();
        }
        else if(buf.contains("<dateOfChanges>")) {
            bufForNotes.textOfNote = str;
            str.clear();
        }
        else if(buf.contains("<dateOfNotification>")) {
            bufForNotes.dateOfChanges = str.mid(0, str.indexOf('\n'));
            str.clear();
        }
        else if(buf.contains("<endOfNote>")) {
            bufForNotes.dateOfNotification = str.mid(0, str.indexOf('\n'));
            str.clear();
            bufForNotes.ID = notesList.size();
            notesList.push_back(bufForNotes);
        }
        else {
            str += buf;
        }
    }
    readFrom.close();
}

void NotesWindow::WriteXML() {
    QFile writeTo(noteLocation + username + ".txt");
    writeTo.open(QFile::WriteOnly);
    QString fileContain = "";
    for(int i = 0; i < notesList.size(); i++) {
        fileContain += "<nameOfNote>\r\n";
        fileContain += notesList[i].nameOfNote;
        fileContain += "\r\n<textOfNote> \r\n";
        fileContain += notesList[i].textOfNote;
        fileContain += "\r\n<dateOfChanges> \r\n";
        fileContain += notesList[i].dateOfChanges;
        fileContain += "\r\n<dateOfNotification> \r\n";
        fileContain += notesList[i].dateOfNotification;
        fileContain += "\r\n<endOfNote>\r\n";
    }
    QTextStream out(&writeTo);
    out.setCodec("UTF-8");
    out << fileContain;
    writeTo.close();
}

void NotesWindow::on_fontComboBox_activated() {
    ui->notesText->setFont(ui->fontComboBox->currentFont());
}

void NotesWindow::on_spinBox_valueChanged() {
    ui->notesText->setStyleSheet("font: " + QString::number(ui->spinBox->value()) + "pt;");
}

void NotesWindow::showNotes() {
    ui->notesName->setEnabled(false);
    ui->notesText->setEnabled(false);
    ui->spinBox->setEnabled(false);
    ui->fontComboBox->setEnabled(false);
    ui->stackedWidget->setCurrentIndex(2);
    this->setFixedSize((ui->stackedWidget->currentWidget()->sizeHint()));
    this->setFixedHeight(600);
    readXML();
    makeListOfNotes();
    createMenu();
}

void NotesWindow::createMenu() {
    QMenu *menu = new QMenu("Open", this);
    QAction *changeAcc = new QAction("Змiнити ОЗ", this);
    QAction *exit = new QAction("Вийти", this);

    menu->addAction(changeAcc);
    menu->addAction(exit);
    ui->accountSettingsButton->setMenu(menu);
    connect(changeAcc, SIGNAL(triggered()), this, SLOT(changeAccount()));
    connect(exit, SIGNAL(triggered()), this, SLOT(exitButton()));
}

void NotesWindow::exitButton() {
    exit(0);
}

void NotesWindow::changeAccount() {
    authorization();
}

void NotesWindow::on_saveNoteButton_clicked() {
    QDate now = QDate::currentDate();
    note buf;


    buf.nameOfNote = ui->notesName->text();
    for(int i = 0; i < notesList.size(); i++) {
        if(ui->notesName->text() == notesList[i].nameOfNote && openID != notesList[i].ID) {
            QMessageBox::critical(this, "Помилка", "Оберiть оригiнальну назву нотатка!");
            return;
        }
    }
    buf.textOfNote = ui->notesText->toPlainText();
    buf.dateOfChanges = now.toString();
    buf.ID = openID;

    if(buf.ID != -1) {
        notesList[buf.ID].dateOfChanges = buf.dateOfChanges;
        notesList[buf.ID].nameOfNote = buf.nameOfNote;
        notesList[buf.ID].textOfNote = buf.textOfNote;
    }
    else {
        buf.ID = notesList.size();
        openID = buf.ID;
        notesList.push_back(buf);
    }

    WriteXML();
    makeListOfNotes();
}

void NotesWindow::makeListOfNotes() {
    ui->notesShowList->clear();
    for(int i = 0; i < notesList.size(); i++) {
        QString obj = notesList[i].nameOfNote + "\r\n-----------------------------\n";
        QString comp = notesList[i].textOfNote.mid(0, notesList[i].textOfNote.indexOf('\n'));
        for(int j = 0; j < 20; j++) {
                obj += comp[j];
        }
        if(comp.size() > 20)
            obj += "...";
        ui->notesShowList->addItem(obj);
    }
}

void NotesWindow::on_notesShowList_itemDoubleClicked() {
   QString name = ui->notesShowList->currentItem()->text();
   int num = name.indexOf('\n');
   QString nameText = name.mid(0, num - 1);
   for(int i = 0; i < notesList.size(); i++) {
       if(notesList[i].nameOfNote == nameText) {
           ui->notesName->setEnabled(true);
           ui->notesText->setEnabled(true);
           ui->spinBox->setEnabled(true);
           ui->fontComboBox->setEnabled(true);

           ui->notesName->setText(notesList[i].nameOfNote);
           ui->notesText->setText(notesList[i].textOfNote);
           openID = notesList[i].ID;
           break;
       }
   }
}

void NotesWindow::on_newNoteButton_clicked() {
    ui->notesName->setEnabled(true);
    ui->notesName->setText("");
    ui->notesText->setEnabled(true);
    ui->notesText->setText("");
    ui->spinBox->setEnabled(true);
    ui->fontComboBox->setEnabled(true);
    openID = -1;
}

void NotesWindow::on_deleteNoteButton_clicked() {
    for(int i = 0; i < notesList.size(); i++) {
        if(openID == notesList[i].ID) {
            notesList.erase(notesList.begin() + i);
        }
    }
    openID = -1;
    ui->notesName->setText("");
    ui->notesText->setText("");
    ui->notesName->setEnabled(false);
    ui->notesText->setEnabled(false);
    ui->spinBox->setEnabled(false);
    ui->fontComboBox->setEnabled(false);

    WriteXML();
    makeListOfNotes();
}

void NotesWindow::on_notificationButton_clicked()
{
    if(openID != -1)
        ui->stackedWidget->setCurrentIndex(3);
}

void NotesWindow::on_CancelButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void NotesWindow::on_OkButton_clicked()
{
    for(int i = 0; i < notesList.size(); i++) {
        if(openID == notesList[i].ID) {
            notesList[i].dateOfNotification = ui->calendarWidget->selectedDate().toString();
            break;
        }
    }
    ui->stackedWidget->setCurrentIndex(2);
}

void NotesWindow::on_SearchButton_clicked()
{
    //организуй поиск)
}

void NotesWindow::on_pushButton_auth_clicked()
{
    ui->pushButton_auth->clearFocus();
    authorization();
}
