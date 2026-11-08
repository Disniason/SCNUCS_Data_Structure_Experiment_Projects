#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QProcess>
#include <QTextEdit>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QStackedWidget *stackedWidget;

    QTextEdit *outputDisplay;

    QWidget *mainface;

    void initUI();
    void initBackendCommunication();

    QProcess *backendProcess;

    QWidget* createPage(const QColor &bgColor);

private slots:
    void onBackendOutput();
};









#endif // MAINWINDOW_H
