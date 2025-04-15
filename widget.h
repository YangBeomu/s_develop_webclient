#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSslSocket>
#include <QTcpSocket>
#include <QFile>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
private:
    bool httpsEnabled = false;
    QSslSocket socket_;

    void Save(const QString fileName = "saved.ini");
    void Load(const QString fileName = "saved.ini");

private slots:
    void doConnected();
    void doDisconnected();
    void doReadyRead();
    void doStateChanged();

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pbConnect_clicked();
    void on_pbDisconnect_clicked();
    void on_pbSend_clicked();
    void on_pbClear_clicked();

private:
    Ui::Widget *ui;

};
#endif // WIDGET_H
