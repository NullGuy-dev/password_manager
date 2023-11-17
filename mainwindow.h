#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <iostream>
#include <string>

using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QPushButton * deletePassword;
    QPushButton * copyPassword;

    QLineEdit * configLink;
    QPushButton * loadConfig;
    string configData;

    QLineEdit * passwordName;
    QLineEdit * password;
    QPushButton * addPassword;

    QListWidget * passwordsList;

    QLabel * statusLine;

    string code(string stringForCoding);
    string decode(string codedString);

    void setConfigData(string data);
    string getConfigData();

    void statustTextOption(QString text, bool err);

    void addPasswordToList();
    void deletePasswordFromList();
    void copyPasswordFromList();
    void loadDataFromConfig();
    void saveToClipboard(char * text);
};
#endif // MAINWINDOW_H
