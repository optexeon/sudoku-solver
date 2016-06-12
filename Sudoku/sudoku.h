#ifndef SUDOKU_H
#define SUDOKU_H

#include <QtWidgets/QMainWindow>
#include "ui_sudoku.h"
#include "SudokuBox.h"

QT_BEGIN_NAMESPACE
	class QLineEdit;
	class QPushButton;
	class QStatusBar;
QT_END_NAMESPACE

class Sudoku : public QMainWindow
{
	Q_OBJECT

public:
	Sudoku(QWidget *parent = 0);
	~Sudoku();

	SudokuBox *SudokuBoxes[81];
	QLineEdit *AnswerBoxes[81];

private:
	Ui::SudokuClass ui;
	
	QPushButton *btnSolve;
	QPushButton *btnClear;
	QStatusBar *staBar;

	void solve();
};

#endif // SUDOKU_H
