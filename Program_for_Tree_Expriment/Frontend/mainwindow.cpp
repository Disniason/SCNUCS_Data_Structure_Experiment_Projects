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
    //QString backendRelativePath = "../../../Backend_TSon/Backend.exe";
    QString backendRelativePath = "../../../Backend_TSB/Backend.exe";
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
            QString cmd = "PTE exit\n"; // 带换行符确保后端能识别
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
    this->resize(800, 1000);
    this->setWindowTitle("TC公司管理");

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
    mainLayout->addWidget(outputDisplay, 5);

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
    QWidget *page = new QWidget(stackedWidget);
    page->setStyleSheet(QString("background-color: %1;").arg(bgColor.name()));
    QVBoxLayout *pageLayout = new QVBoxLayout(page);

    QLabel *label = new QLabel(page);
    label->setStyleSheet("font-size: 24px; color: black;");
    label->setAlignment(Qt::AlignCenter);

    QPushButton *displayBtn = new QPushButton("打印树", page);
    pageLayout->addWidget(displayBtn);
    connect(displayBtn, &QPushButton::clicked,this, [=](){
        QDialog dialog(this);
        dialog.setWindowTitle("打印树");
        dialog.resize(300, 200);
        QVBoxLayout *layout = new QVBoxLayout(&dialog);

        QLabel *nameLabel = new QLabel("请输入部门名称", &dialog);
        QLineEdit *nameEdit = new QLineEdit(&dialog);
        nameEdit->setText("total");
        layout->addWidget(nameLabel);
        layout->addWidget(nameEdit);

        QPushButton *okBtn = new QPushButton("确认打印", &dialog);
        connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
        layout->addWidget(okBtn);

        if (dialog.exec() == QDialog::Accepted) {
            QString name = nameEdit->text().trimmed();

            if (name.isEmpty()){
                QMessageBox::warning(page, "提示", "显示的部门不能为空！");
                return;
            }

            //发送指令给后端
            if (outputDisplay) {
                outputDisplay->clear();
            }
            if (backendProcess) {
                QString cmd = QString("PTE display %1").arg(name);
                backendProcess->write((cmd + "\n").toUtf8());
                backendProcess->waitForBytesWritten(1000);
            }
        }
    });

    QPushButton *outputBtn = new QPushButton("输出树", page);
    pageLayout->addWidget(outputBtn);
    connect(outputBtn, &QPushButton::clicked,this, [=](){
        QDialog dialog(this);
        dialog.setWindowTitle("输出树");
        dialog.resize(300, 200);
        QVBoxLayout *layout = new QVBoxLayout(&dialog);

        QLabel *nameLabel = new QLabel("请输入部门名称", &dialog);
        QLineEdit *nameEdit = new QLineEdit(&dialog);
        nameEdit->setText("total");
        layout->addWidget(nameLabel);
        layout->addWidget(nameEdit);

        QLabel *pathLabel = new QLabel("请输入下载路径", &dialog);
        QLineEdit *pathEdit = new QLineEdit(&dialog);
        layout->addWidget(pathLabel);
        layout->addWidget(pathEdit);

        QPushButton *okBtn = new QPushButton("确认输出", &dialog);
        connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
        layout->addWidget(okBtn);

        if (dialog.exec() == QDialog::Accepted) {
            QString name = nameEdit->text().trimmed();
            QString path = pathEdit->text().trimmed();

            if (name.isEmpty() || path.isEmpty()){
                QMessageBox::warning(page, "提示", "输出的部门和下载路径都不能为空！");
                return;
            }

            //发送指令给后端
            if (outputDisplay) {
                outputDisplay->clear();
            }
            if (backendProcess) {
                QString cmd = QString("PTE output %1 %2").arg(name, path);
                backendProcess->write((cmd + "\n").toUtf8());
                backendProcess->waitForBytesWritten(1000);
            }
        }
    });

    QPushButton *sonnumBtn = new QPushButton("查询下属一级子部门数", page);
    pageLayout->addWidget(sonnumBtn);
    connect(sonnumBtn, &QPushButton::clicked,this, [=](){
        QDialog dialog(this);
        dialog.setWindowTitle("查询下属一级子部门数");
        dialog.resize(300, 200);
        QVBoxLayout *layout = new QVBoxLayout(&dialog);

        QLabel *nameLabel = new QLabel("请输入部门名称", &dialog);
        QLineEdit *nameEdit = new QLineEdit(&dialog);
        layout->addWidget(nameLabel);
        layout->addWidget(nameEdit);

        QPushButton *okBtn = new QPushButton("确认查询", &dialog);
        connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
        layout->addWidget(okBtn);

        if (dialog.exec() == QDialog::Accepted) {
            QString name = nameEdit->text().trimmed();

            if (name.isEmpty()){
                QMessageBox::warning(page, "提示", "查询的部门不能为空！");
                return;
            }

            //发送指令给后端
            if (outputDisplay) {
                outputDisplay->clear();
            }
            if (backendProcess) {
                QString cmd = QString("PTE sonnum %1").arg(name);
                backendProcess->write((cmd + "\n").toUtf8());
                backendProcess->waitForBytesWritten(1000);
            }
        }
    });

    QPushButton *grandsonnumBtn = new QPushButton("查询最低一级子部门数", page);
    pageLayout->addWidget(grandsonnumBtn);
    connect(grandsonnumBtn, &QPushButton::clicked,this, [=](){
        QDialog dialog(this);
        dialog.setWindowTitle("查询最低一级子部门数");
        dialog.resize(300, 200);
        QVBoxLayout *layout = new QVBoxLayout(&dialog);

        QLabel *nameLabel = new QLabel("请输入部门名称", &dialog);
        QLineEdit *nameEdit = new QLineEdit(&dialog);
        layout->addWidget(nameLabel);
        layout->addWidget(nameEdit);

        QPushButton *okBtn = new QPushButton("确认查询", &dialog);
        connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
        layout->addWidget(okBtn);

        if (dialog.exec() == QDialog::Accepted) {
            QString name = nameEdit->text().trimmed();

            if (name.isEmpty()){
                QMessageBox::warning(page, "提示", "查询的部门不能为空！");
                return;
            }

            //发送指令给后端
            if (outputDisplay) {
                outputDisplay->clear();
            }
            if (backendProcess) {
                QString cmd = QString("PTE grandsonnum %1").arg(name);
                backendProcess->write((cmd + "\n").toUtf8());
                backendProcess->waitForBytesWritten(1000);
            }
        }
    });

    QPushButton *allsonnumBtn = new QPushButton("查询下属总子部门数", page);
    pageLayout->addWidget(allsonnumBtn);
    connect(allsonnumBtn, &QPushButton::clicked,this, [=](){
        QDialog dialog(this);
        dialog.setWindowTitle("查询下属总子部门数");
        dialog.resize(300, 200);
        QVBoxLayout *layout = new QVBoxLayout(&dialog);

        QLabel *nameLabel = new QLabel("请输入部门名称", &dialog);
        QLineEdit *nameEdit = new QLineEdit(&dialog);
        layout->addWidget(nameLabel);
        layout->addWidget(nameEdit);

        QPushButton *okBtn = new QPushButton("确认查询", &dialog);
        connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
        layout->addWidget(okBtn);

        if (dialog.exec() == QDialog::Accepted) {
            QString name = nameEdit->text().trimmed();

            if (name.isEmpty()){
                QMessageBox::warning(page, "提示", "查询的部门不能为空！");
                return;
            }

            //发送指令给后端
            if (outputDisplay) {
                outputDisplay->clear();
            }
            if (backendProcess) {
                QString cmd = QString("PTE allsonnum %1").arg(name);
                backendProcess->write((cmd + "\n").toUtf8());
                backendProcess->waitForBytesWritten(1000);
            }
        }
    });

    QPushButton *allnumBtn = new QPushButton("查询部门总人数", page);
    pageLayout->addWidget(allnumBtn);
    connect(allnumBtn, &QPushButton::clicked,this, [=](){
        QDialog dialog(this);
        dialog.setWindowTitle("查询部门总人数");
        dialog.resize(300, 200);
        QVBoxLayout *layout = new QVBoxLayout(&dialog);

        QLabel *nameLabel = new QLabel("请输入部门名称", &dialog);
        QLineEdit *nameEdit = new QLineEdit(&dialog);
        layout->addWidget(nameLabel);
        layout->addWidget(nameEdit);

        QPushButton *okBtn = new QPushButton("确认查询", &dialog);
        connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
        layout->addWidget(okBtn);

        if (dialog.exec() == QDialog::Accepted) {
            QString name = nameEdit->text().trimmed();

            if (name.isEmpty()){
                QMessageBox::warning(page, "提示", "查询的部门不能为空！");
                return;
            }

            //发送指令给后端
            if (outputDisplay) {
                outputDisplay->clear();
            }
            if (backendProcess) {
                QString cmd = QString("PTE allnum %1").arg(name);
                backendProcess->write((cmd + "\n").toUtf8());
                backendProcess->waitForBytesWritten(1000);
            }
        }
    });

    QPushButton *parentBtn = new QPushButton("查询上一级部门", page);
    pageLayout->addWidget(parentBtn);
    connect(parentBtn, &QPushButton::clicked,this, [=](){
        QDialog dialog(this);
        dialog.setWindowTitle("查询上一级部门");
        dialog.resize(300, 200);
        QVBoxLayout *layout = new QVBoxLayout(&dialog);

        QLabel *nameLabel = new QLabel("请输入部门名称", &dialog);
        QLineEdit *nameEdit = new QLineEdit(&dialog);
        layout->addWidget(nameLabel);
        layout->addWidget(nameEdit);

        QPushButton *okBtn = new QPushButton("确认查询", &dialog);
        connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
        layout->addWidget(okBtn);

        if (dialog.exec() == QDialog::Accepted) {
            QString name = nameEdit->text().trimmed();

            if (name.isEmpty()){
                QMessageBox::warning(page, "提示", "查询的部门不能为空！");
                return;
            }

            //发送指令给后端
            if (outputDisplay) {
                outputDisplay->clear();
            }
            if (backendProcess) {
                QString cmd = QString("PTE parent %1").arg(name);
                backendProcess->write((cmd + "\n").toUtf8());
                backendProcess->waitForBytesWritten(1000);
            }
        }
    });

    QPushButton *nodenumBtn = new QPushButton("查询总结点数", page);
    pageLayout->addWidget(nodenumBtn);
    connect(nodenumBtn, &QPushButton::clicked,this, [=](){

        if (outputDisplay) {
            outputDisplay->clear();
        }
        if (backendProcess) {
            QString cmd = QString("PTE nodenum");
            backendProcess->write((cmd + "\n").toUtf8());
            backendProcess->waitForBytesWritten(1000);
        }

    });

    QPushButton *employeenumBtn = new QPushButton("查询公司员工总数", page);
    pageLayout->addWidget(employeenumBtn);
    connect(employeenumBtn, &QPushButton::clicked,this, [=](){

        if (outputDisplay) {
            outputDisplay->clear();
        }
        if (backendProcess) {
            QString cmd = QString("PTE employeenum");
            backendProcess->write((cmd + "\n").toUtf8());
            backendProcess->waitForBytesWritten(1000);
        }

    });

    QPushButton *leafnumBtn = new QPushButton("查询叶子结点数", page);
    pageLayout->addWidget(leafnumBtn);
    connect(leafnumBtn, &QPushButton::clicked,this, [=](){

        if (outputDisplay) {
            outputDisplay->clear();
        }
        if (backendProcess) {
            QString cmd = QString("PTE leafnum");
            backendProcess->write((cmd + "\n").toUtf8());
            backendProcess->waitForBytesWritten(1000);
        }

    });

    QPushButton *deleteBtn = new QPushButton("销毁树", page);
    pageLayout->addWidget(deleteBtn);
    connect(deleteBtn, &QPushButton::clicked,this, [=](){
        QDialog dialog(this);
        dialog.setWindowTitle("销毁树");
        dialog.resize(300, 200);
        QVBoxLayout *layout = new QVBoxLayout(&dialog);

        QLabel *nameLabel = new QLabel("请输入结点", &dialog);
        QLineEdit *nameEdit = new QLineEdit(&dialog);
        nameEdit->setText("total");
        layout->addWidget(nameLabel);
        layout->addWidget(nameEdit);

        QPushButton *okBtn = new QPushButton("确认销毁", &dialog);
        connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
        layout->addWidget(okBtn);

        if (dialog.exec() == QDialog::Accepted) {
            QString name = nameEdit->text().trimmed();

            if (name.isEmpty()){
                QMessageBox::warning(page, "提示", "销毁的部门不能为空！");
                return;
            }

            //发送指令给后端
            if (outputDisplay) {
                outputDisplay->clear();
            }
            if (backendProcess) {
                QString cmd = QString("PTE delete %1").arg(name);
                backendProcess->write((cmd + "\n").toUtf8());
                backendProcess->waitForBytesWritten(1000);
            }
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




