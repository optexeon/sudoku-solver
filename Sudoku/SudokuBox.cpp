#include "SudokuBox.h"
#include "sudoku.h"
#include <QPushButton>
#include <QFont>
#include <QKeyEvent>

SudokuBox::SudokuBox(QWidget *parent)
	:QLineEdit(parent)
{
	connect(this, &SudokuBox::textChanged, this, &SudokuBox::checkInput);
}


SudokuBox::~SudokuBox(void)
{
}

void SudokuBox::init(int r, int c)
{
	row = r;
	col = c;

	setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	setMaxLength(1);
}

void SudokuBox::focusInEvent(QFocusEvent *event)
{
	setBlack();
	selectAll();

	QLineEdit::focusInEvent(event);
}

void SudokuBox::keyReleaseEvent(QKeyEvent *event)
{
	int next = -1;

	switch(event->key())
	{
	case Qt::Key_Up:
		next = (9+row-1)%9*9 + col;
		break;
	case Qt::Key_Down:
		next = (9+row+1)%9*9 + col;
		break;
	case Qt::Key_Left:
		next = row*9 + (9+col-1)%9;
		break;
	case Qt::Key_Right:
		next = row*9 + (9+col+1)%9;
		break;
	}

	if(event->key() == Qt::Key_Backspace && text().length() == 0)
	{
		next = (81 + row*9 + col - 1)%81;
	}

	if(next >= 0)
	{
		((Sudoku*)parent())->SudokuBoxes[next]->setFocus();
	}

	QLineEdit::keyReleaseEvent(event);
}

void SudokuBox::checkInput()
{
	const auto& str = text();
	if(str.length() > 0)
	{
		if( !(str[0] >= QChar('1') && str[0] <= QChar('9')) )
		{
			clear();
		}
		else
		{
			int next = (row*9 + col + 1)%81;

			((Sudoku*)parent())->SudokuBoxes[next]->setFocus();
		}
	}
}
