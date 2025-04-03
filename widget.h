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

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pbConnect_clicked();
    void on_pbDisconnect_clicked();
    void on_pbSend_clicked();
    void on_pbClear_clicked();

    void doConnected();
    void doDisconnected();
    void doReadyRead();
    void doStateChanged();

private:
    bool httpsEnabled = false;
    Ui::Widget *ui;
    QSslSocket socket_;

    void Saved(const QString fileName = "saved.ini");
    void LoadPrevInfo(const QString fileName = "saved.ini");
};
#endif // WIDGET_H
