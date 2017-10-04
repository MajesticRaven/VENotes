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
bool isRegConnect = false, isAuthConnect = false, isRemindConnect = false;
QString result1OfDialog, result2OfDialog;

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
    {
        readSettings();
        if(username == "")
            authorization();
        else showNotes();
    }
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
    ui->buttonBox_in_reg->button(QDialogButtonBox::Ok)->setAutoDefault(true);
    ui->buttonBox_in_reg->button(QDialogButtonBox::Ok)->setDefault(true);
    ui->buttonBox_in_reg->button(QDialogButtonBox::Ok)->setShortcut(Qt::Key_Enter);
    ui->buttonBox_in_reg->button(QDialogButtonBox::Cancel)->setShortcut(Qt::Key_Cancel);
    ui->buttonBox_in_reg->button(QDialogButtonBox::Ok)->setText(tr("Створити"));
    ui->buttonBox_in_reg->button(QDialogButtonBox::Cancel)->setText(tr("Скасувати"));
    ui->lineEdit_reg_log->clearFocus();
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

    if(!isRegConnect)
    {
        connect(ui->lineEdit_reg_log, SIGNAL(textChanged(QString)), this, SLOT(setEnabledToRegOk()));
        connect(ui->lineEdit_reg_pass, SIGNAL(textChanged(QString)), this, SLOT(setEnabledToRegOk()));
        connect(ui->lineEdit_reg_pass_check, SIGNAL(textChanged(QString)), this, SLOT(setEnabledToRegOk()));
        connect(ui->lineEdit_reg_email, SIGNAL(textChanged(QString)), this, SLOT(setEnabledToRegOk()));
        connect(ui->buttonBox_in_reg->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(createAccount()));
        isRegConnect = true;
    }
}

void NotesWindow::authorization()
{
    ui->stackedWidget->setCurrentIndex(0);
    this->setFixedWidth(310);
    this->setFixedHeight(470);
    ui->buttonBox_in_auth->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->buttonBox_in_auth->button(QDialogButtonBox::Ok)->setAutoDefault(true);
    ui->buttonBox_in_auth->button(QDialogButtonBox::Ok)->setDefault(true);
    ui->buttonBox_in_auth->button(QDialogButtonBox::Ok)->setShortcut(Qt::Key_Enter);
    ui->buttonBox_in_auth->button(QDialogButtonBox::Cancel)->setShortcut(Qt::Key_Cancel);
    ui->buttonBox_in_auth->button(QDialogButtonBox::Ok)->setText(tr("Увійти"));
    ui->buttonBox_in_auth->button(QDialogButtonBox::Cancel)->setText(tr("Скасувати"));

    ui->pushButton_new_account->setCursor(Qt::PointingHandCursor);
    ui->pushButton_remind_account->setCursor(Qt::PointingHandCursor);

    validator = new QRegExpValidator(validExp, ui->lineEdit_auth_email);
    ui->lineEdit_auth_email->setValidator(validator);

    if(!isAuthConnect)
    {
        connect(ui->lineEdit_auth_email, SIGNAL(textChanged(QString)), this, SLOT(setEnabledToAuthOk()));
        connect(ui->lineEdit_auth_pass, SIGNAL(textChanged(QString)), this, SLOT(setEnabledToAuthOk()));
        connect(ui->buttonBox_in_auth->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(loginInAccount()));
        isAuthConnect = true;
    }
}

void NotesWindow::remind()
{
    ui->stackedWidget->setCurrentIndex(4);
    this->setFixedWidth(310);
    this->setFixedHeight(470);
    ui->buttonBox_in_remind->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->buttonBox_in_remind->button(QDialogButtonBox::Ok)->setAutoDefault(true);
    ui->buttonBox_in_remind->button(QDialogButtonBox::Ok)->setDefault(true);
    ui->buttonBox_in_remind->button(QDialogButtonBox::Ok)->setShortcut(Qt::Key_Enter);
    ui->buttonBox_in_remind->button(QDialogButtonBox::Cancel)->setShortcut(Qt::Key_Cancel);
    ui->buttonBox_in_remind->button(QDialogButtonBox::Ok)->setText(tr("Відновити"));
    ui->buttonBox_in_remind->button(QDialogButtonBox::Cancel)->setText(tr("Скасувати"));

    ui->pushButton_registration->setCursor(Qt::PointingHandCursor);
    ui->pushButton_authorization->setCursor(Qt::PointingHandCursor);

    validator = new QRegExpValidator(validExp, ui->lineEdit_auth_email);
    ui->lineEdit_remind_email->setValidator(validator);

    if(!isRemindConnect)
    {
        connect(ui->lineEdit_remind_email, SIGNAL(textChanged(QString)), this, SLOT(setEnabledToRemindOk()));
        connect(ui->buttonBox_in_remind->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(remindPassword()));
        connect(ui->buttonBox_in_remind->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(returnToAuth()));
        isRemindConnect = true;
    }
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
            username = ui->lineEdit_reg_log->text();
            writeSettings();
            this->setFixedSize(sizeHint());
            showNotes();
            QMessageBox::information(this, tr("Реєстрацію завершено"), "Новий обліковий запис було створено.\n");
            ui->lineEdit_reg_log->clear();
            ui->lineEdit_reg_email->clear();
            ui->lineEdit_reg_pass->clear();
            ui->lineEdit_reg_pass_check->clear();
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
            username = name;
            showNotes();
            writeSettings();
            ui->labelLogin->setText("Вітаємо, " + username + "!");
            ui->lineEdit_auth_email->clear();
            ui->lineEdit_auth_pass->clear();
        }
    }
}

void NotesWindow::returnToAuth()
{
    ui->lineEdit_remind_email->clear();
    authorization();
}

void NotesWindow::remindPassword() {}

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

void NotesWindow::setEnabledToRemindOk()
{
    if(ui->lineEdit_remind_email->hasAcceptableInput())
        ui->buttonBox_in_remind->button(QDialogButtonBox::Ok)->setEnabled(true);
    else
        ui->buttonBox_in_remind->button(QDialogButtonBox::Ok)->setEnabled(false);
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
    ui->SearchButton->setAutoDefault(false);
    ui->SearchButton->clearFocus();
    this->setFixedSize((ui->stackedWidget->currentWidget()->sizeHint()));
    this->setFixedHeight(600);
    readXML();
    makeListOfNotes();
    createMenu();
    ui->labelLogin->setText("Вітаємо, " + username + "!");
}

void NotesWindow::createMenu() {
    QMenu *menu = new QMenu("Open", this);

    QAction *refreshEmail = new QAction("Змінити Email", this);
    refreshEmail->setShortcut(tr("Ctrl+Alt+E"));
    refreshEmail->setToolTip(tr("Змінити email облікового запису"));
    refreshEmail->setStatusTip(tr("Змінити email облікового запису"));
    refreshEmail->setIcon(QIcon("resources/Images/newEmail.png"));

    QAction *refreshPass = new QAction("Змінити пароль", this);
    refreshPass->setShortcut(tr("Ctrl+Alt+P"));
    refreshPass->setToolTip(tr("Змінити пароль облікового запису"));
    refreshPass->setStatusTip(tr("Змінити пароль облікового запису"));
    refreshPass->setIcon(QIcon("resources/Images/refreshKey.png"));

    QAction *changeAcc = new QAction("Вийти з ОЗ", this);
    changeAcc->setShortcut(tr("Ctrl+E"));
    changeAcc->setToolTip(tr("Вийти з облікового запису"));
    changeAcc->setStatusTip(tr("Вийти з облікового запису"));
    changeAcc->setIcon(QIcon("resources/Images/logout.png"));

    QAction *deleteAcc = new QAction("Видалити ОЗ", this);
    deleteAcc->setShortcut(tr("Ctrl+Alt+D"));
    deleteAcc->setToolTip(tr("Видалити обліковий запис"));
    deleteAcc->setStatusTip(tr("Видалити обліковий запис"));
    deleteAcc->setIcon(QIcon("resources/Images/deleteAccount.png"));

    QAction *exit = new QAction("Вийти", this);
    exit->setShortcut(QKeySequence::Close);
    exit->setToolTip(tr("Вийти з програми"));
    exit->setStatusTip(tr("Вийти з програми"));
    exit->setIcon(QIcon("resources/Images/close.png"));

    menu->addAction(refreshEmail);
    menu->addAction(refreshPass);
    menu->addAction(changeAcc);
    menu->addAction(deleteAcc);
    menu->addAction(exit);
    ui->accountSettingsButton->setMenu(menu);
    connect(refreshEmail, SIGNAL(triggered()), this, SLOT(showRefreshEmail()));
    connect(refreshPass, SIGNAL(triggered()), this, SLOT(showRefreshPassword()));
    connect(changeAcc, SIGNAL(triggered()), this, SLOT(changeAccount()));
    connect(deleteAcc, SIGNAL(triggered()), this, SLOT(deleteAccount()));
    connect(exit, SIGNAL(triggered()), this, SLOT(exitButton()));
}

void NotesWindow::exitButton() {
    writeSettings();
    exit(0);
}

void NotesWindow::changeAccount() {
    username = "";
    ui->accountSettingsButton->clearFocus();
    writeSettings();
    authorization();
}

void NotesWindow::showRefreshEmail() {
    ui->accountSettingsButton->clearFocus();
    Dialog * dialog = new Dialog(this, this);
    dialog->setName("Змінити email");
    dialog->setPasswordFlag();
    dialog->setEmailFlag();
    dialog->setLabels("Новий email:", "Підтвердити дію паролем:");
    dialog->show();
}

void NotesWindow::refreshEmail() {
    QSqlQuery query(db);
    query.prepare("SELECT password FROM users WHERE login = :login");
    query.bindValue(":login", username);
    if(!query.exec())
        QMessageBox::warning(this, tr("Помилка"), "Не вдалося під`єднатися до бази даних!");
    else
    {
        query.next();
        QString password = query.value(0).toString();
        if(password != QCryptographicHash::hash(result2OfDialog.toUtf8(), QCryptographicHash::Md5).toHex())
            QMessageBox::warning(this, tr("Помилка"), "Пароль було вказано невірно!");
        else
        {
            query.prepare("UPDATE users SET email = :email WHERE login = :login AND password = :password");
            query.bindValue(":email", result1OfDialog);
            query.bindValue(":password", QCryptographicHash::hash(result2OfDialog.toUtf8(), QCryptographicHash::Md5).toHex());
            query.bindValue(":login", username);
            if(!query.exec())
                QMessageBox::warning(this, tr("Помилка"), "Не вдалося змінити email!");
            else QMessageBox::information(this, tr("Зміна email"), "Email було успішно змінено!");
        }
    }
}

void NotesWindow::showRefreshPassword() {
    ui->accountSettingsButton->clearFocus();
    Dialog * dialog = new Dialog(this, this);
    dialog->setName("Змінити пароль");
    dialog->setPasswordFlag();
    dialog->setLabels("Новий пароль:", "Старий пароль:");
    dialog->show();
}

void NotesWindow::refreshPassword() {
    QSqlQuery query(db);
    query.prepare("SELECT password FROM users WHERE login = :login");
    query.bindValue(":login", username);
    if(!query.exec())
        QMessageBox::warning(this, tr("Помилка"), "Не вдалося під`єднатися до бази даних!");
    else
    {
        query.next();
        QString password = query.value(0).toString();
        if(password != QCryptographicHash::hash(result2OfDialog.toUtf8(), QCryptographicHash::Md5).toHex())
            QMessageBox::warning(this, tr("Помилка"), "Старий пароль було вказано невірно!");
        else
        {
            query.prepare("UPDATE users SET password = :newPassword WHERE login = :login AND password = :oldPassword");
            query.bindValue(":oldPassword", QCryptographicHash::hash(result2OfDialog.toUtf8(), QCryptographicHash::Md5).toHex());
            query.bindValue(":newPassword", QCryptographicHash::hash(result1OfDialog.toUtf8(), QCryptographicHash::Md5).toHex());
            query.bindValue(":login", username);
            if(!query.exec())
                QMessageBox::warning(this, tr("Помилка"), "Не вдалося змінити пароль!");
            else QMessageBox::information(this, tr("Зміна паролю"), "Пароль було успішно змінено!");
        }
    }
}

void NotesWindow::deleteAccount() {
    QMessageBox * pmbx = new QMessageBox();
    pmbx->setWindowTitle(tr("Підтвердження дії"));
    pmbx->setText("Ви впевнені, що хочете видалити обліковий запис?\nУ цьому випадку буде видалено всі нотатки.");
    pmbx->addButton("Так", QMessageBox::AcceptRole);
    pmbx->addButton("Ні", QMessageBox::RejectRole);
    int n = pmbx->exec();
    delete pmbx;
    if (n == QMessageBox::AcceptRole)
    {
        QSqlQuery query(db);
        query.prepare("DELETE FROM users WHERE login = :login");
        query.bindValue(":login", username);
        if(!query.exec())
            QMessageBox::warning(this, tr("Помилка"), "Не вдалося під`єднатися до бази даних!");
        else
        {
            QMessageBox::information(this, tr("Видалення ОЗ"), "Обліковий запис було видалено!");
            QFile file(noteLocation + username + ".txt");
            file.remove();
            username = "";
            registration();
        }
    }
}

void NotesWindow::on_saveNoteButton_clicked() {
    QDate now = QDate::currentDate();
    note buf;


    buf.nameOfNote = ui->notesName->text();
    for(int i = 0; i < notesList.size(); i++) {
        if(ui->notesName->text() == notesList[i].nameOfNote && openID != notesList[i].ID) {
            QMessageBox::warning(this, "Помилка", "Оберiть оригiнальну назву нотатки!");
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
        QString obj = "Назва: ";
        int lenght = 0;
        int max_symb = 21;
        for(int j = 0; j < notesList[i].nameOfNote.size(); j++)
        {
            int temp = j;
            j = notesList[i].nameOfNote.indexOf(" ", j);
            if(j == -1)
            {
                if(notesList[i].nameOfNote.size() - temp + lenght > max_symb)
                {
                    obj.remove(obj.size() - 1, 1);
                    obj += "\r\n";
                }

                obj += notesList[i].nameOfNote.mid(temp, notesList[i].nameOfNote.size() - temp) + "\r\n   ---------------------------------   \n";
                lenght = 0;
                break;
            }
            if(lenght + j - temp > max_symb)
            {
                obj.remove(obj.size() - 1, 1);
                obj += "\r\n" + notesList[i].nameOfNote.mid(temp, j - temp + 1);
                lenght = j - temp + 1;
                max_symb = 25;
            }
            else
            {
                obj += notesList[i].nameOfNote.mid(temp, j - temp + 1);
                lenght += j - temp + 1;
            }
        }
        QString comp = notesList[i].textOfNote.mid(0, notesList[i].textOfNote.indexOf('\n'));
        for(int j = 0; j < 20; j++) {
                obj += comp[j];
        }
        if(comp.size() > 30)
            obj += "...";
        obj += "\n";
        ui->notesShowList->addItem(obj);
    }
}

void NotesWindow::on_notesShowList_itemDoubleClicked() {
   QString name = ui->notesShowList->currentItem()->text();

   int e_pos = name.lastIndexOf("\n");
   e_pos = name.lastIndexOf("\n", e_pos - 1);
   e_pos = name.lastIndexOf("\n", e_pos - 1);
   e_pos--;
   name = name.mid(7, e_pos - 7);
   int pos = name.indexOf("\n"), temp_pos = 0;
   while(pos != -1)
   {
       name.remove(pos - 1, 1);
       name.replace(pos - 1, 1, " ");
       temp_pos = pos + 1;
       pos = name.indexOf("\n", temp_pos);
   }
   for(int i = 0; i < notesList.size(); i++) {
       if(notesList[i].nameOfNote == name) {
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
    {
        //Закомментировал то, что нужно для вызова календаря в том же окне, а не в диалоговом коне
        //ui->stackedWidget->setCurrentIndex(3);
        //this->setFixedSize(ui->stackedWidget->currentWidget()->sizeHint());
        Calendar * calendar = new Calendar(this);
        calendar->show();
        calendar->exec();
    }
}

void NotesWindow::setDate(QString str)
{
    for(int i = 0; i < notesList.size(); i++) {
        if(openID == notesList[i].ID) {
            notesList[i].dateOfNotification = str;
            break;
        }
    }
}

/*
 *Это нужно для работу напоминание в том же окне, а не в диалоговом окне
 *
void NotesWindow::on_CancelButton_clicked()
{
    ui->notificationButton->clearFocus();
    showNotes();
}

void NotesWindow::on_OkButton_clicked()
{
    for(int i = 0; i < notesList.size(); i++) {
        if(openID == notesList[i].ID) {
            notesList[i].dateOfNotification = ui->calendarWidget->selectedDate().toString();
            break;
        }
    }
    ui->notificationButton->clearFocus();
    showNotes();
}
*/

void NotesWindow::on_SearchButton_clicked()
{
    //организуй поиск)
}

void NotesWindow::on_pushButton_auth_clicked()
{
    ui->pushButton_auth->clearFocus();
    authorization();
}

void NotesWindow::readSettings()
{
    QSettings settings("VE Inc.", "VENotes");
    username = settings.value("login").toString();
}

void NotesWindow::writeSettings()
{
    QSettings settings("VE Inc.", "VENotes");
    settings.setValue("login", username);
}

Ui::NotesWindow * NotesWindow::getUi()
{
    return ui;
}

void NotesWindow::on_pushButton_authorization_clicked()
{
    ui->pushButton_authorization->clearFocus();
    authorization();
}

void NotesWindow::on_pushButton_registration_clicked()
{
    ui->pushButton_registration->clearFocus();
    registration();
}

void NotesWindow::on_pushButton_remind_account_clicked()
{
    ui->pushButton_remind_account->clearFocus();
    remind();
}

void NotesWindow::getDataFromDialog(QString s, QString s2, bool email_or_pass)
{
    result1OfDialog = s;
    result2OfDialog = s2;
    if(email_or_pass)
        refreshEmail();
    else refreshPassword();
}
