#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    LoadPrevInfo();

    QObject::connect(&socket_, &QAbstractSocket::connected,
                     this, &Widget::doConnected);

    QObject::connect(&socket_, &QAbstractSocket::disconnected,
                     this, &Widget::doDisconnected);

    QObject::connect(&socket_, &QIODevice::readyRead,
                     this, &Widget::doReadyRead);

    QObject::connect(&socket_, &QAbstractSocket::stateChanged, this, &Widget::doStateChanged);
}

Widget::~Widget()
{
    Saved();
    delete ui;
}

void Widget::on_pbConnect_clicked() {
    httpsEnabled = ui->rbHttp->isChecked() ? false : true;

    if(socket_.state() != QAbstractSocket::UnconnectedState)
        socket_.abort();


    if(!httpsEnabled) {
        socket_.connectToHost(ui->leHost->text(), ui->lePort->text().toUShort());
    }
    else
        socket_.connectToHostEncrypted(ui->leHost->text(), ui->lePort->text().toUShort());
}

void Widget::on_pbDisconnect_clicked()
{
    socket_.disconnectFromHost();
}

void Widget::on_pbSend_clicked()
{
    socket_.write(ui->pteSend->toPlainText().toUtf8());
}

void Widget::on_pbClear_clicked()
{
    ui->pteRecv->clear();
}

void Widget::doConnected() {
    QString msg = "connected \r\n";

    ui->pteRecv->insertPlainText(msg);
}

void Widget::doDisconnected() {
    QString msg = "Disconnected \r\n";

    ui->pteRecv->insertPlainText(msg);
}

void Widget::doReadyRead() {
    QString msg = socket_.readAll();
    ui->pteRecv->appendPlainText(msg);
}

void Widget::doStateChanged() {
    switch(socket_.state()) {
    case QAbstractSocket::ConnectedState:
        ui->frRadios->setEnabled(false);
        ui->pbConnect->setEnabled(false);
        ui->pbDisconnect->setEnabled(true);
        ui->pbSend->setEnabled(true);
        break;
    case QAbstractSocket::UnconnectedState:
        ui->frRadios->setEnabled(true);
        ui->pbConnect->setEnabled(true);
        ui->pbDisconnect->setEnabled(false);
        ui->pbSend->setEnabled(false);
        break;
    }
}

void Widget::Saved(const QString fileName) {
    QRect rect = this->geometry();

    QSettings file(fileName, QSettings::IniFormat);

    file.setValue("posX", rect.x());
    file.setValue("posY", rect.y());
    file.setValue("width", rect.width());
    file.setValue("height", rect.height());
    file.setValue("pteRecv", ui->pteRecv->toPlainText());
    qDebug()<<ui->pteRecv->toPlainText();
    file.setValue("pteSend", ui->pteSend->toPlainText());
    qDebug()<<ui->pteSend->toPlainText();
}

void Widget::LoadPrevInfo(const QString fileName) {
    if(!QFile(fileName).exists()) return;

    QSettings file(fileName, QSettings::IniFormat);



    QRect wndPos;

    wndPos.setX(file.value("posX").toInt());
    wndPos.setY(file.value("posY").toInt());
    wndPos.setWidth(file.value("width").toInt());
    wndPos.setHeight(file.value("height").toInt());

    this->setGeometry(wndPos);

    ui->pteRecv->setPlainText(file.value("pteRecv").toString());
    ui->pteSend->setPlainText(file.value("pteSend").toString());
}
