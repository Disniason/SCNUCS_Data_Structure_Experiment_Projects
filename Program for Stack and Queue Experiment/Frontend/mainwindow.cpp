#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QStatusBar>
#include <QMessageBox>
#include <QDebug>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QLineEdit>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    backendProcess = new QProcess(this);

    QString frontendPath = QCoreApplication::applicationFilePath();
    QFileInfo frontendInfo(frontendPath);
    QString frontendDir = frontendInfo.path();
    QString backendRelativePath = "./PSQE_BCP.exe";
    QFileInfo backendInfo(frontendDir + "/" + backendRelativePath);
    QString backendAbsPath = backendInfo.absoluteFilePath();
    qDebug() << "计算出的后端绝对路径：" << backendAbsPath;
    backendProcess->start(backendInfo.absoluteFilePath());

    initUI();
    initBackendCommunication();

}

MainWindow::~MainWindow()
{
    // 1. 确保backendProcess存在
    if (backendProcess) {
        if (backendProcess->state() == QProcess::Running) {
            QString cmd = "PSQE exit\n"; // 带换行符确保后端能识别
            backendProcess->write(cmd.toUtf8());
            if (!backendProcess->waitForBytesWritten(1000)) {
                qDebug() << "退出指令发送超时";
            }

            if (!backendProcess->waitForFinished(2000)) {
                qDebug() << "后端未正常退出，强制终止";
                backendProcess->kill(); // 优雅退出失败再强制终止
                backendProcess->waitForFinished(1000); // 等待强制终止完成
            }
        }

        delete backendProcess;
        backendProcess = nullptr;
    }

    delete ui;
}

void MainWindow::initUI() {
    this->resize(800, 600);
    this->setWindowTitle("停车场管理");

    stackedWidget = new QStackedWidget(this);
    this->setCentralWidget(stackedWidget);

    outputDisplay = new QTextEdit(this);
    outputDisplay->setReadOnly(true);
    //outputDisplay->setPlaceholderText("后端输出信息将显示在这里...");
    outputDisplay->setStyleSheet("font-size: 14px; line-height: 1.5;");

    mainface = createPage(Qt::lightGray);
    stackedWidget->addWidget(mainface);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(stackedWidget, 3);
    mainLayout->addWidget(outputDisplay, 3);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    this->setCentralWidget(centralWidget);

}

void MainWindow::initBackendCommunication() {
    if (backendProcess) {
        connect(backendProcess, &QProcess::readyReadStandardOutput,
                this, &MainWindow::onBackendOutput);
    } else {
        qDebug() << "后端进程未初始化，无法接收后端消息";
    }
}

QWidget* MainWindow::createPage(const QColor &bgColor) {
    QWidget *page = new QWidget(stackedWidget); // 显式指定父对象，由Qt对象树管理
    page->setStyleSheet(QString("background-color: %1;").arg(bgColor.name()));
    QVBoxLayout *pageLayout = new QVBoxLayout(page);

    QLabel *label = new QLabel(page);
    label->setStyleSheet("font-size: 24px; color: black;");
    label->setAlignment(Qt::AlignCenter);

    QPushButton *setstackmaxsizeBtn = new QPushButton("设置停车场容量", page);
    pageLayout->addWidget(setstackmaxsizeBtn);
    connect(setstackmaxsizeBtn, &QPushButton::clicked,this, [=](){
        QDialog dialog(this);
        dialog.setWindowTitle("设置停车场容量");
        dialog.resize(300, 200);
        QVBoxLayout *layout = new QVBoxLayout(&dialog);

        QLabel *sizeLabel = new QLabel("请输入停车场容量：", &dialog);
        QLineEdit *sizeEdit = new QLineEdit(&dialog);
        layout->addWidget(sizeLabel);
        layout->addWidget(sizeEdit);

        QPushButton *okBtn = new QPushButton("确认设置", &dialog);
        connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
        layout->addWidget(okBtn);

        if (dialog.exec() == QDialog::Accepted) {
            QString size = sizeEdit->text().trimmed();

            if (size.isEmpty()){
                QMessageBox::warning(page, "提示", "停车场容量不能为空！");
                return;
            }

            //发送指令给后端
            if (outputDisplay) {
                outputDisplay->clear();
            }
            if (backendProcess) {
                QString cmd = QString("PSQE setstackmaxsize %1").arg(size);
                backendProcess->write((cmd + "\n").toUtf8());
                backendProcess->waitForBytesWritten(1000);
            }
        }
    });

    QPushButton *setqueuemaxsizeBtn = new QPushButton("设置候车场容量", page);
    pageLayout->addWidget(setqueuemaxsizeBtn);
    connect(setqueuemaxsizeBtn, &QPushButton::clicked,this, [=](){
        QDialog dialog(this);
        dialog.setWindowTitle("设置候车场容量");
        dialog.resize(300, 200);
        QVBoxLayout *layout = new QVBoxLayout(&dialog);

        QLabel *sizeLabel = new QLabel("请输入候车场容量：", &dialog);
        QLineEdit *sizeEdit = new QLineEdit(&dialog);
        layout->addWidget(sizeLabel);
        layout->addWidget(sizeEdit);

        QPushButton *okBtn = new QPushButton("确认设置", &dialog);
        connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
        layout->addWidget(okBtn);

        if (dialog.exec() == QDialog::Accepted) {
            QString size = sizeEdit->text().trimmed();

            if (size.isEmpty()){
                QMessageBox::warning(page, "提示", "停车场容量不能为空！");
                return;
            }

            //发送指令给后端
            if (outputDisplay) {
                outputDisplay->clear();
            }
            if (backendProcess) {
                QString cmd = QString("PSQE setqueuemaxsize %1").arg(size);
                backendProcess->write((cmd + "\n").toUtf8());
                backendProcess->waitForBytesWritten(1000);
            }
        }
    });

    QPushButton *cararriveBtn = new QPushButton("车辆到达", page);
    pageLayout->addWidget(cararriveBtn);
    connect(cararriveBtn, &QPushButton::clicked,this, [=](){
        QDialog dialog(this);
        dialog.setWindowTitle("车辆到达");
        dialog.resize(300, 200);
        QVBoxLayout *layout = new QVBoxLayout(&dialog);

        QLabel *sizeLabel = new QLabel("请输入车牌号：", &dialog);
        QLineEdit *sizeEdit = new QLineEdit(&dialog);
        layout->addWidget(sizeLabel);
        layout->addWidget(sizeEdit);

        QLabel *hourLabel = new QLabel("请选择时间(时)：", &dialog);
        QComboBox *hourCombo = new QComboBox(&dialog);
        hourCombo->addItems({"00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23"});
        layout->addWidget(hourLabel);
        layout->addWidget(hourCombo);

        QLabel *minLabel = new QLabel("请选择时间(分)：", &dialog);
        QComboBox *minCombo = new QComboBox(&dialog);
        minCombo->addItems({"00", "05", "10", "15", "20", "25", "30", "35", "40", "45", "50", "55"});
        layout->addWidget(minLabel);
        layout->addWidget(minCombo);

        QPushButton *okBtn = new QPushButton("确认到达", &dialog);
        connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
        layout->addWidget(okBtn);

        if (dialog.exec() == QDialog::Accepted) {
            QString size = sizeEdit->text().trimmed();
            QString hour = hourCombo->currentText();
            QString min = minCombo->currentText();

            if (size.isEmpty() || hour.isEmpty() || min.isEmpty()){
                QMessageBox::warning(page, "提示", "车牌号或时间(时分)不能为空！");
                return;
            }

            //发送指令给后端
            if (outputDisplay) {
                outputDisplay->clear();
            }
            if (backendProcess) {
                QString cmd = QString("PSQE cararrive %1 %2:%3").arg(size, hour, min);
                backendProcess->write((cmd + "\n").toUtf8());
                backendProcess->waitForBytesWritten(1000);
            }
        }
    });

    QPushButton *carleaveBtn = new QPushButton("车辆离开", page);
    pageLayout->addWidget(carleaveBtn);
    connect(carleaveBtn, &QPushButton::clicked,this, [=](){
        QDialog dialog(this);
        dialog.setWindowTitle("车辆离开");
        dialog.resize(300, 200);
        QVBoxLayout *layout = new QVBoxLayout(&dialog);

        QLabel *sizeLabel = new QLabel("请输入车牌号：", &dialog);
        QLineEdit *sizeEdit = new QLineEdit(&dialog);
        layout->addWidget(sizeLabel);
        layout->addWidget(sizeEdit);

        QLabel *hourLabel = new QLabel("请选择时间(时)：", &dialog);
        QComboBox *hourCombo = new QComboBox(&dialog);
        hourCombo->addItems({"00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23"});
        layout->addWidget(hourLabel);
        layout->addWidget(hourCombo);

        QLabel *minLabel = new QLabel("请选择时间(分)：", &dialog);
        QComboBox *minCombo = new QComboBox(&dialog);
        minCombo->addItems({"00", "05", "10", "15", "20", "25", "30", "35", "40", "45", "50", "55"});
        layout->addWidget(minLabel);
        layout->addWidget(minCombo);

        QPushButton *okBtn = new QPushButton("确认离开", &dialog);
        connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
        layout->addWidget(okBtn);

        if (dialog.exec() == QDialog::Accepted) {
            QString size = sizeEdit->text().trimmed();
            QString hour = hourCombo->currentText();
            QString min = minCombo->currentText();

            if (size.isEmpty() || hour.isEmpty() || min.isEmpty()){
                QMessageBox::warning(page, "提示", "车牌号或时间(时分)不能为空！");
                return;
            }

            //发送指令给后端
            if (outputDisplay) {
                outputDisplay->clear();
            }
            if (backendProcess) {
                QString cmd = QString("PSQE carleave %1 %2:%3").arg(size, hour, min);
                backendProcess->write((cmd + "\n").toUtf8());
                backendProcess->waitForBytesWritten(1000);
            }
        }
    });

    QPushButton *displaystackcarBtn = new QPushButton("显示停车场所有车辆", page);
    pageLayout->addWidget(displaystackcarBtn);
    connect(displaystackcarBtn, &QPushButton::clicked,this, [=](){
        if (outputDisplay) {
            outputDisplay->clear();
        }
        if (backendProcess){
            QString cmd = QString("PSQE displaystackcar");
            backendProcess->write(cmd.toUtf8() + "\n"); // 发送给后端
            backendProcess->waitForBytesWritten();
        }
    });

    QPushButton *displayqueuecarBtn = new QPushButton("显示候车场所有车辆", page);
    pageLayout->addWidget(displayqueuecarBtn);
    connect(displayqueuecarBtn, &QPushButton::clicked,this, [=](){
        if (outputDisplay) {
            outputDisplay->clear();
        }
        if (backendProcess){
            QString cmd = QString("PSQE displayqueuecar");
            backendProcess->write(cmd.toUtf8() + "\n"); // 发送给后端
            backendProcess->waitForBytesWritten();
        }
    });

    return page;
}

void MainWindow::onBackendOutput() {
    QByteArray data = backendProcess->readAllStandardOutput();
    QString response = QString::fromUtf8(data);

    qDebug() << "收到后端数据：\n" << response;

    if (outputDisplay) {
        outputDisplay->append(response);
        outputDisplay->moveCursor(QTextCursor::End);
    }
}




