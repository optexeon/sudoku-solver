/********************************************************************************
** Form generated from reading UI file 'sudoku.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SUDOKU_H
#define UI_SUDOKU_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SudokuClass
{
public:
    QWidget *centralWidget;

    void setupUi(QMainWindow *SudokuClass)
    {
        if (SudokuClass->objectName().isEmpty())
            SudokuClass->setObjectName(QStringLiteral("SudokuClass"));
        SudokuClass->resize(600, 400);
        centralWidget = new QWidget(SudokuClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        SudokuClass->setCentralWidget(centralWidget);

        retranslateUi(SudokuClass);

        QMetaObject::connectSlotsByName(SudokuClass);
    } // setupUi

    void retranslateUi(QMainWindow *SudokuClass)
    {
        SudokuClass->setWindowTitle(QApplication::translate("SudokuClass", "Sudoku", 0));
    } // retranslateUi

};

namespace Ui {
    class SudokuClass: public Ui_SudokuClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SUDOKU_H
