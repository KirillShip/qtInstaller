#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Установщик тестовое Шипулин");
    setMinimumSize(300, 200);

    btnNext = new QPushButton("Далее");
    btnBack = new QPushButton("Назад");
    btnGetInstallMenu = new QPushButton("Меню");
    dataLabel = new QLabel("Шипулин Кирилл");

    installMenu = new QComboBox(this);
    populatePackagesList();
    installMenu->setVisible(false);

    gridLayout = new QGridLayout(this);
    gridLayout->addWidget(installMenu, 1, 1, Qt::AlignCenter);
    gridLayout->addWidget(dataLabel, 1, 1, Qt::AlignCenter);
    gridLayout->addWidget(btnBack, 2, 0, Qt::AlignLeft);
    gridLayout->addWidget(btnNext, 2, 2, Qt::AlignRight);

    btnBack->setEnabled(false);
    connect(btnNext, &QPushButton::clicked, this, &Widget::showNextPage);
    connect(btnBack, &QPushButton::clicked, this, &Widget::showPrevPage);
}

//Загружает в список доступные для установки пакеты из ресурсов
void Widget::populatePackagesList()
{
    QString resoursePath = ":/packages";
    QDir resourseDir(resoursePath);
    QStringList packageFiles = resourseDir.entryList(QDir::Files);

    installMenu->clear();
    for(const QString &package: packageFiles)
    {
        installMenu->addItem(package, QVariant(package));
    }
}

void Widget::showPrevPage()
{
    btnNext->setText("Далее");
    installMenu->setVisible(false);
    dataLabel->setVisible(true);
    disconnect(btnNext, &QPushButton::clicked, this, &Widget::install);
    connect(btnNext, &QPushButton::clicked, this, &Widget::showNextPage);
}

void Widget::showNextPage()
{
    btnBack->setEnabled(true);
    installMenu->setVisible(true);
    dataLabel->setVisible(false);
    btnNext->setText("Установить");
    disconnect(btnNext, &QPushButton::clicked, this, &Widget::showNextPage);
    connect(btnNext, &QPushButton::clicked, this, &Widget::install);
}

void Widget::install()
{
    QMessageBox msgBox(this);
    QString packageName = installMenu->currentData().toString();
    QFile debFile(QString(":/packages/%1").arg(packageName));
    if (!debFile.open(QIODevice::ReadOnly))
    {
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("Не удалось открыть файл пакета " + QString(packageName));
        msgBox.exec();
        return;
    }

    QTemporaryFile tempFile;
    if (!tempFile.open())
    {
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("Не удалось создать временный файл");
        msgBox.exec();
        return;
    }

    if (tempFile.write(debFile.readAll()) == -1)
    {
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("Ошибка записи во временный файл");
        msgBox.exec();
        return;
    }
    tempFile.close();

    QProcess process;
    process.start("pkexec", QStringList() << "--user" << "root" << "dpkg" << "-i" << tempFile.fileName());

    if (!process.waitForFinished())
    {
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("Таймаут установки");
        msgBox.exec();
        return;
    }

    if (process.exitCode() != 0)
    {
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("Ошибка установки " + QString(process.exitCode()));
        msgBox.exec();
        return;
    }

    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText("Пакет успешно установлен!");
    msgBox.exec();
}

Widget::~Widget() {}
