#include "sudoku.h"
#include <QPushButton>
#include <QFont>
#include <QStatusBar>
#include "sudo.h"
#include <QTime>

QPalette SudokuBox::redFont;
QPalette SudokuBox::blackFont;

Sudoku::Sudoku(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	SudokuBox::redFont.setColor(QPalette::Text, Qt::red);
	SudokuBox::blackFont.setColor(QPalette::Text, Qt::black);

	for(int i = 0; i < 81; i++)
	{
		int row = i/9;
		int col = i%9;

		SudokuBoxes[i] = new SudokuBox(this);
		SudokuBoxes[i]->init(row, col);
		SudokuBoxes[i]->setGeometry(50+col*30+(col/3)*10, 50+row*30+(row/3)*10, 30, 30);

		connect(SudokuBoxes[i], &SudokuBox::textChanged, this, &Sudoku::solve);

		AnswerBoxes[i] = new QLineEdit(this);
		AnswerBoxes[i]->setGeometry(390+col*30+(col/3)*10, 50+row*30+(row/3)*10, 30, 30);
		AnswerBoxes[i]->setReadOnly(true);
		AnswerBoxes[i]->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
		AnswerBoxes[i]->setFocusPolicy(Qt::NoFocus);
	}

	this->setFixedSize(730, 400);
	setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);

// 	btnSolve = new QPushButton(this);
// 	btnSolve->setGeometry(70, 360, 100, 50);
// 	btnSolve->setText("Solve");
// 
// 	connect(btnSolve, &QPushButton::clicked, this, &Sudoku::solve);
// 
// 	btnClear = new QPushButton(this);
// 	btnClear->setGeometry(220, 360, 100, 50);
// 	btnClear->setText("Clear");
// 
// 	connect(btnClear, &QPushButton::clicked, [&](){for(int i = 0; i < 81; i++)SudokuBoxes[i]->clear();});
// 
// 	btnClear->setFocus();
// 
// 	staBar = statusBar();
	
	setFocus();
	solve();
}

Sudoku::~Sudoku()
{

}

void Sudoku::solve()
{
	Sudo solver;
	std::vector<bool> filled;
	filled.assign(81, false);

	for(int i = 0; i < 81; i++)
	{
		SudokuBoxes[i]->setBlack();
	}
	
	for(int i = 0; i < 81; i++)
	{
		int row = i/9;
		int col = i%9;

		const auto& str = SudokuBoxes[i]->text();
		if(str.length() > 0)
		{
			filled[i] = true;

			int a = str.toInt();
			solver.fill_in(row+1, col+1, a);
		}
	}

	bool is_solved = solver.solve();

	if(is_solved)
	{
		const auto& block = solver.get_block();

// 		for(int i = 0; i < 9; i++)
// 		{
// 			for(int j = 0; j < 9; j++)
// 			{
// 				int idx = i*9 + j;
// 
// 				if(!filled[idx])
// 				{
// 					SudokuBoxes[idx]->setText(QString::number(block[idx]));
// 				}
// 			}
// 		}
// 
// 		btnClear->setFocus();
// 		for(int i = 0; i < 81; i++)
// 		{
// 			if(!filled[i])
// 			{
// 				SudokuBoxes[i]->setRed();
// 			}
// 		}

		for(int i = 0; i < 9; i++)
		{
			for(int j = 0; j < 9; j++)
			{
				int idx = i*9 + j;

				AnswerBoxes[idx]->setText(QString::number(block[idx]));

				if(!filled[idx])
				{
					AnswerBoxes[idx]->setPalette(SudokuBox::redFont);
				}
				else
				{
					AnswerBoxes[idx]->setPalette(SudokuBox::blackFont);
				}
			}
		}

/*		staBar->showMessage("Solved!", 5000);*/
	}
	else
	{
		for(int i = 0; i < 9; i++)
		{
			for(int j = 0; j < 9; j++)
			{
				int idx = i*9 + j;

				AnswerBoxes[idx]->setPalette(SudokuBox::blackFont);

				if(!filled[idx])
				{
					AnswerBoxes[idx]->clear();
				}
				else
				{
					AnswerBoxes[idx]->setText(SudokuBoxes[idx]->text());
				}
			}
		}

/*		staBar->showMessage("No solution...", 5000);*/
	}
}
