#ifndef SUDOKUBOX_H
#define SUDOKUBOX_H

#include <QLineEdit>
#include <QPalette>

QT_BEGIN_NAMESPACE

QT_END_NAMESPACE

class SudokuBox : public QLineEdit
{
	Q_OBJECT

public:
	SudokuBox(QWidget *parent = (QWidget *)0);
	virtual ~SudokuBox(void);

	void init(int r, int c);

 	static QPalette redFont, blackFont;
 	void setRed(){setPalette(redFont);};
 	void setBlack(){setPalette(blackFont);};

protected:
	int row, col;

	void focusInEvent(QFocusEvent *event)override;
	void keyReleaseEvent(QKeyEvent *event)override;
	void checkInput();
};

#endif