#include <windows.h>
#include "mainwindow.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <iostream>
#include <fstream>
#include <math.h>
#include <cmath>
#include <cstdio>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget * mainCenteralWidget = new QWidget(this);
    setWindowTitle("PASSOGER - your password manager <3 | Created by https://github.com/NullGuy-dev");
    setWindowIcon(QIcon(":/images/passoger_icon.png"));

    QVBoxLayout * mainLayout = new QVBoxLayout(this);
    mainCenteralWidget->setLayout(mainLayout);

    QFont optionedFont = QFont("Courier New", 11);

    QSize minButtonsSize = QSize(100,30);

    QSize maxButtonsSize = QSize(200,35);
    QHBoxLayout * buttonsPanel = new QHBoxLayout(this);
    mainLayout->addLayout(buttonsPanel);
    deletePassword = new QPushButton(this);
    deletePassword->setText("Delete Password");
    deletePassword->setFont(optionedFont);
    deletePassword->setMinimumSize(minButtonsSize);
    buttonsPanel->addWidget(deletePassword);
    connect(deletePassword, &QPushButton::clicked, this, &MainWindow::deletePasswordFromList);
    copyPassword = new QPushButton(this);
    copyPassword->setText("Copy Password");
    copyPassword->setFont(optionedFont);
    copyPassword->setMinimumSize(minButtonsSize);
    buttonsPanel->addWidget(copyPassword);
    connect(copyPassword, &QPushButton::clicked, this, &MainWindow::copyPasswordFromList);

    QHBoxLayout * configOptions = new QHBoxLayout(this);
    mainLayout->addLayout(configOptions);
    configLink = new QLineEdit(this);
    QString configLinkText = "Enter config-file link...";
    configLink->setMinimumSize(QSize(100,30));
    configLink->setMaximumHeight(35);
    configLink->setFont(optionedFont);
    configLink->setPlaceholderText(configLinkText);
    configLink->setToolTip(configLinkText);
    configLink->setText("D:/pswd.txt");
    configOptions->addWidget(configLink);
    loadConfig = new QPushButton(this);
    loadConfig->setMinimumSize(QSize(50,35));
    loadConfig->setMaximumSize(QSize(120,40));
    loadConfig->setText("Load");
    loadConfig->setFont(QFont("Courier New", 14));
    configOptions->addWidget(loadConfig);
    connect(loadConfig, &QPushButton::clicked, this, &MainWindow::loadDataFromConfig);

    QHBoxLayout * addPasswordPanel = new QHBoxLayout(this);
    mainLayout->addLayout(addPasswordPanel);
    passwordName = new QLineEdit(this);
    QString passwordNameText = "Enter password's name...";
    passwordName->setMinimumSize(minButtonsSize);
    passwordName->setMaximumHeight(35);
    passwordName->setFont(optionedFont);
    passwordName->setPlaceholderText(passwordNameText);
    passwordName->setToolTip(passwordNameText);
    addPasswordPanel->addWidget(passwordName);
    password = new QLineEdit(this);
    QString passwordText = "Enter your password...";
    password->setMinimumSize(minButtonsSize);
    password->setMaximumHeight(35);
    password->setFont(optionedFont);
    password->setPlaceholderText(passwordText);
    password->setToolTip(passwordText);
    addPasswordPanel->addWidget(password);
    addPassword = new QPushButton(this);
    addPassword->setMinimumSize(QSize(50,35));
    addPassword->setMaximumSize(QSize(120,40));
    addPassword->setText("Save");
    addPassword->setFont(QFont("Courier New", 14));
    addPasswordPanel->addWidget(addPassword);
    connect(addPassword, &QPushButton::clicked, this, &MainWindow::addPasswordToList);

    passwordsList = new QListWidget(this);
    mainLayout->addWidget(passwordsList);

    statusLine = new QLabel(this);
    statusLine->setFont(optionedFont);
    mainLayout->addWidget(statusLine);

    setCentralWidget(mainCenteralWidget);
}

MainWindow::~MainWindow()
{
}

void MainWindow::statustTextOption(QString text, bool err = false) {
    statusLine->setText("❤️ " + text + " ❤️");
    QString color = (!err) ? "green" : "red";
    statusLine->setStyleSheet("color: " + color);
}

string MainWindow::code(string stringForCoding) {
    string codedString;

    for (int i = 0; i < stringForCoding.length(); i++) {
        int divisionVal = (i == 0) ? 1 : i;
        int codedCHAR = static_cast<int>(stringForCoding[i]) * divisionVal;
        int res = (codedCHAR > 100) ? (pow(codedCHAR, 2)) : (pow(codedCHAR, 4));
        codedString += to_string(res) + ' ';
    }

    return codedString;
}

string MainWindow::decode(string codedString) {
    string decodedString;
    string currentCodedSymbol;
    codedString += " ";

    int lenghtOfPassword = 0;
    for (int i = 0; i < codedString.length(); i++) {
        if (codedString[i] != ' ')
            currentCodedSymbol += codedString[i];
        else {
            int divisionVal = (lenghtOfPassword == 0) ? 1 : lenghtOfPassword;
            int codeOfSymbol = stoi(currentCodedSymbol);
            int sqrtCodeOfSymbol = sqrt(codeOfSymbol)/divisionVal;
            int res = (sqrtCodeOfSymbol < 1000) ? sqrtCodeOfSymbol : sqrtCodeOfSymbol;

            decodedString += static_cast<char>(res);
            currentCodedSymbol = "";
            lenghtOfPassword++;
        }
    }

    return decodedString;
}

void MainWindow::setConfigData(string data) {
    ofstream fileForImportData;
    fileForImportData.open(configLink->text().toStdString());
    fileForImportData << data;
    fileForImportData.close();
}

string MainWindow::getConfigData() {
    string data;
    ifstream fileForExportData;
    fileForExportData.open(configLink->text().toStdString());
    getline(fileForExportData, data);
    fileForExportData.close();
    return data;
}

void MainWindow::addPasswordToList() {
    QString textForFinding = passwordName->text();
    QList<QListWidgetItem *> foundItems = passwordsList->findItems(textForFinding, Qt::MatchFlags());

    if (foundItems.isEmpty() && textForFinding.toStdString() != "" && password->text().toStdString() != "" && textForFinding.toStdString().find(";") == string::npos && textForFinding.toStdString().find("-") == string::npos) {
        passwordsList->addItem(textForFinding);
        string passwordNameData = textForFinding.toStdString();
        string passwordCodeData = code(password->text().toStdString());
        passwordCodeData.pop_back();
        string resultLabel = passwordNameData + " - " + passwordCodeData;
        string lineForAdding = getConfigData() + resultLabel + ";";

        setConfigData(lineForAdding);

        loadDataFromConfig();

        statustTextOption("New password was added");
    }
    else {
        statustTextOption("I'm sorry, you can't add this password. Your password name can't have ';', '-' and edit-line can't be empty", true);
    }

}

void MainWindow::loadDataFromConfig() {
    string stdConfigLink = configLink->text().toStdString();

    configData = getConfigData();

    string currentLabel;
    string a;
    for (char letter : configData) {
        if (letter == '-') {
            currentLabel.pop_back();
            QString textForFinding = QString::fromStdString(currentLabel);
            QList<QListWidgetItem *> foundItems = passwordsList->findItems(textForFinding, Qt::MatchFlags());

            if (foundItems.isEmpty() && textForFinding.toStdString() != "") {
                passwordsList->addItem(textForFinding);
                string passwordNameData = textForFinding.toStdString();
                string passwordCodeData = code(password->text().toStdString());
                passwordCodeData.pop_back();
            }
            currentLabel = "";
        }

        currentLabel += letter;

        if (letter == ';') {
            currentLabel = "";
        }
    }

    statustTextOption("Your config-file is loaded");
}

void MainWindow::saveToClipboard(char * text) {
    HGLOBAL hglb;
    char * pBuf;

    if ( ! OpenClipboard((HWND)NULL) ){
        perror("OpenClipboard");
        exit(1);
    }
    EmptyClipboard();
    if ( ( hglb = GlobalAlloc(GMEM_MOVEABLE, strlen(text) + 1) ) == NULL ){
        perror("GlobalAlloc");
        CloseClipboard();
        exit(1);
    }
    pBuf = (char*)GlobalLock(hglb);
    strcpy(pBuf, text);
    GlobalUnlock(hglb);
    SetClipboardData(CF_OEMTEXT, hglb);
    CloseClipboard();
}

void MainWindow::copyPasswordFromList() {
    configData = getConfigData();

    QListWidgetItem chossedItem = *passwordsList->currentItem();

    string currentLabel;
    string name;
    for (char letter : configData) {
        currentLabel += letter;
        if (name == chossedItem.text().toStdString()) {
            if (letter == ';') {
                currentLabel.pop_back();
                currentLabel.erase(currentLabel.begin());
                break;
            }
        }
        if (letter == '-') {
            currentLabel.pop_back();
            if (currentLabel == (chossedItem.text().toStdString() + " ")) {
                name = currentLabel;
                name.pop_back();
                currentLabel = "";
            }
            else
                currentLabel = "";
        } else {
            if (letter == ';')
                currentLabel = "";
        }
    }

    saveToClipboard(decode(currentLabel).data());
    statustTextOption("Chossed password is saved to the clipboard");
}

void MainWindow::deletePasswordFromList() {
    QListWidgetItem chossedItem = *passwordsList->currentItem();

    QString rightText;
    int firstIndex;
    int lastIndex;
    bool isFounded = false;

    for (int i = 0; i < configData.length(); i++) {
        if (!isFounded) {
            if (rightText == "") {
                firstIndex = i;
            }

            rightText += configData[i];
            if (rightText == chossedItem.text()) {
                isFounded = true;
            }

            if (configData[i] == ';') {
                rightText = "";
                firstIndex = 0;
            }
        }
        else {
            lastIndex = i;

            if (configData[i] == ';') {
                lastIndex++;
                break;
            }
        }
    }

    configData.erase(firstIndex, lastIndex - firstIndex);

    passwordsList->clear();

    setConfigData(configData);

    loadDataFromConfig();

    statustTextOption("Chossed password is deleted");
}
