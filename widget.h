#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QComboBox>
#include <QLabel>
#include <QProcess>
#include <QMessageBox>
#include <QDir>
#include <QDebug>
#include <QTemporaryFile>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    QPushButton *btnNext;
    QPushButton *btnBack;
    QPushButton *btnGetInstallMenu;

    QLabel *dataLabel;
    QComboBox *installMenu;
    QGridLayout *gridLayout;

    void populatePackagesList();

private slots:
    void showNextPage();
    void showPrevPage();
    void install();
};
#endif // WIDGET_H
