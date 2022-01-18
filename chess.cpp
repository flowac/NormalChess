#include "lib.h"

Chess::Chess(QWidget *p):QGraphicsView(p)
{
	bred = QBrush(QColor(255, 0, 0), Qt::SolidPattern);
	bgray = QBrush(QColor(155, 155, 155), Qt::SolidPattern);
	bblack = QBrush(QColor(0, 0, 0), Qt::SolidPattern);
	bwhite = QBrush(QColor(255, 255, 255), Qt::SolidPattern);
	pred = QPen(QColor(255, 0, 0), 1);
	pwhite = QPen(QColor(255, 255, 255), 1);
	pgray = QPen(QColor(155, 155, 155), 1);
	pgreen = QPen(QColor(0, 255, 0), 1);
	pblack = QPen(QColor(0, 0, 0), 1);

	setScene(&scn);
	setContentsMargins(0, 0, 0, 0);
	setInteractive(true);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setCursor(Qt::CrossCursor);
	reset(true);
}

void Chess::draw()
{
	int i, j;
	BoardInfo k;

	if (!inGame)
		return;

	sp.h = height();
	sp.w = width();
	sp.div = sp.w > sp.h ? sp.h / 11 : sp.w / 11;
	sp.r = sp.div * 0.8;

	scn.clear();
	resetCachedContent();
	grid();

	for (i = 0; i < 9; i++) {
		for (j = 0; j < 10; j++) {
			k = binfo[i][j];
			if (k.val == NONE)
				continue;
			putPiece(k.val, i, j, pwhite, k.isRed ? bred : bgray);
		}
	}
}

void Chess::reset(bool isRed)
{
	int i, j;
	BoardInfo k = {.isRed = 0,.val = NONE };

	LOGF(QString::asprintf("Initialize new game with player %s as RED.", isRed ? "ONE" : "TWO"));
	for (i = 0; i < 9; i++)
		for (j = 0; j < 10; j++)
			binfo[i][j] = k;
	inGame = true;
	isSelValid = false;
	p1Red = isRed;
	p1Turn = isRed;

	//player one
	binfo[0][3] = binfo[2][3] = binfo[4][3] =
	    binfo[6][3] = binfo[8][3] = {.isRed = isRed,.val = SOLDIER };
	binfo[1][2] = binfo[7][2] = {.isRed = isRed,.val = CANNON };
	binfo[0][0] = binfo[8][0] = {.isRed = isRed,.val = CAR };
	binfo[1][0] = binfo[7][0] = {.isRed = isRed,.val = HORSE };
	binfo[2][0] = binfo[6][0] = {.isRed = isRed,.val = MINISTER };
	binfo[3][0] = binfo[5][0] = {.isRed = isRed,.val = ADVISOR };
	binfo[4][0] = {.isRed = isRed,.val = GENERAL };
	p1Gen[0] = 4;
	p1Gen[1] = 0;

	//player two
	binfo[0][6] = binfo[2][6] = binfo[4][6] =
	    binfo[6][6] = binfo[8][6] = {.isRed = !isRed,.val = SOLDIER };
	binfo[1][7] = binfo[7][7] = {.isRed = !isRed,.val = CANNON };
	binfo[0][9] = binfo[8][9] = {.isRed = !isRed,.val = CAR };
	binfo[1][9] = binfo[7][9] = {.isRed = !isRed,.val = HORSE };
	binfo[2][9] = binfo[6][9] = {.isRed = !isRed,.val = MINISTER };
	binfo[3][9] = binfo[5][9] = {.isRed = !isRed,.val = ADVISOR };
	binfo[4][9] = {.isRed = !isRed,.val = GENERAL };
	p2Gen[0] = 4;
	p2Gen[1] = 9;

	draw();
}

void Chess::addText(double xpos, double ypos, QString str, QColor color)
{
	QGraphicsTextItem *tmp = new QGraphicsTextItem();

	tmp->setDefaultTextColor(color);
	tmp->setPos(xpos, ypos);
	tmp->setPlainText(str);
	scn.addItem(tmp);
}

void Chess::addText(double xpos, double ypos, const char *str, QColor col)
{
	addText(xpos, ypos, QString(str), col);
}

void Chess::gameStatus(MoveStatus ms)
{
	QString statStr = QString::asprintf("Turn: %s", p1Red ^ p1Turn ? "BLACK" : "RED");
	switch (ms) {
	case P1WON:
		statStr = QString("Player 1 wins");
		break;
	case P2WON:
		statStr = QString("Player 2 wins");
		break;
	case PDRAW:
		statStr = QString("Draw");
		break;
	case MCHECK:
		statStr += QString(" Checked");
		break;
	default:
		if (inGame)
			goto INGAME;
		else
			return;
		break;
	}
	LOGF(statStr);
  INGAME:
	turnPrompt->setText(statStr);
}

void Chess::grid()
{
	int i, j;
	const double div = sp.div;
	double tmpf;

	//vertical markers
	for (i = 0; i < 9; i++) {
		tmpf = (i + 0.7) * div;
		addText(tmpf - 5, 10.1 * div, QString::number(i + 1));
		scn.addLine(tmpf, 0.7 * div, tmpf, 4.7 * div, pwhite);
		scn.addLine(tmpf, 5.7 * div, tmpf, 9.7 * div, pwhite);
	}
	//horizontal markers
	for (j = 0; j < 10; j++) {
		tmpf = (j + 0.7) * div;
		addText(1, tmpf - 7, QString(QChar('A' + 9 - j)));
		scn.addLine(0.7 * div, tmpf, 8.7 * div, tmpf, pwhite);
	}
	scn.addLine(3.7 * div, 0.7 * div, 5.7 * div, 2.7 * div, pwhite);
	scn.addLine(3.7 * div, 2.7 * div, 5.7 * div, 0.7 * div, pwhite);
	scn.addLine(3.7 * div, 7.7 * div, 5.7 * div, 9.7 * div, pwhite);
	scn.addLine(3.7 * div, 9.7 * div, 5.7 * div, 7.7 * div, pwhite);

	scn.addLine(0, sp.h - 1, sp.w - 1, sp.h - 1, pblack);   //horizontal anchor
	scn.addLine(sp.w - 1, 0, sp.w - 1, sp.h - 1, pblack);   //vertical anchor
}

MoveStatus Chess::mateCheck()
{
	int i, j;
	MoveStatus ms = MERROR;

	if (p1Gen[0] == p2Gen[0]) {
		for (j = p1Gen[1] + 1; j < p2Gen[1]; j++)
			if (binfo[p1Gen[0]][j].val != NONE)
				ms = MOKAY;
		if (ms != MOKAY) {
			LOGF("General checkmated");
			if (p1Turn)
				return P1WON;
			else
				return P2WON;
		}
	}

	for (i = 0; i < 9; i++) {
		for (j = 0; j < 10; j++) {
			if (binfo[i][j].val == NONE)
				continue;
			if (MOKAY == validMove(i, j, p1Gen[0], p1Gen[1])) {
				if (p1Turn) {
					ms = MCHECK;
					goto IS_CHECKED;
				} else {
					ms = P2WON;
					goto IS_MATED;
				}
			}
			if (MOKAY == validMove(i, j, p2Gen[0], p2Gen[1])) {
				if (p1Turn) {
					ms = P1WON;
					goto IS_MATED;
				} else {
					ms = MCHECK;
					goto IS_CHECKED;
				}
			}
		}
	}
	return ms;
  IS_CHECKED:
	LOGF(QString::asprintf("Check %c %d", j + 'A', i + 1));
	return ms;
  IS_MATED:
	LOGF(QString::asprintf("Mated %c %d", j + 'A', i + 1));
	return ms;
}

void Chess::mousePressEvent(QMouseEvent * evt)
{
	QPoint pos = mapFromGlobal(cursor().pos());

	switch (evt->button()) {
	case Qt::LeftButton:
		if (!selectPiece(pos))
			LOGF(QString::asprintf("select failed %d, %d", pos.x(), pos.y()));
		break;
	case Qt::RightButton:
		LOGF(QString("right click"));
		break;
	default:
		break;
	}
}

void Chess::putPiece(PieceType val, int xpos, int ypos, QPen qp, QBrush qb)
{
	double xd = sp.div * (xpos + 0.3), yd = sp.div * (9.3 - ypos);

	scn.addEllipse(xd, yd, sp.r, sp.r, qp, qb);
	yd += sp.r * 0.2;
	switch (val) {
	case SOLDIER:
		addText(xd, yd, "Soldier", Qt::green);
		break;
	case CANNON:
		addText(xd, yd, "Cannon", Qt::green);
		break;
	case CAR:
		addText(xd, yd, "Chariot", Qt::green);
		break;
	case HORSE:
		addText(xd, yd, "Horse", Qt::green);
		break;
	case MINISTER:
		addText(xd, yd, "Minister", Qt::green);
		break;
	case ADVISOR:
		addText(xd, yd, "Advisor", Qt::green);
		break;
	case GENERAL:
		addText(xd, yd, "General", Qt::green);
		break;
	case NONE:
	default:
		addText(xd, yd, "???", Qt::yellow);
		break;
	}
}

bool Chess::selectPiece(QPoint pos)
{
	static int xsel, ysel;
	double div = sp.div;
	int xp = pos.x(), yp = pos.y();
	int i = round((xp - sp.r / 2 - 0.3 * div) / div);
	int j = round((sp.r / 2 + 9.3 * div - yp) / div);
	MoveStatus ms;

	if (!inGame)
		return false;

	draw();
	if (i < 0 || i > 8 || j < 0 || j > 9)
		return false;
	if (!isSelValid && (binfo[i][j].val == NONE || binfo[i][j].isRed == (p1Red ^ p1Turn)))
		return false;
	if (isSelValid) {
		if (validMove(xsel, ysel, i, j) != MOKAY) {
			isSelValid = false;
			return false;
		}
		LOGF(QString::asprintf("Moving %d from %c %d to %c %d", binfo[i][j].val,
		     ysel + 'A', xsel + 1, j + 'A', i + 1));
		binfo[i][j].val = binfo[xsel][ysel].val;
		binfo[i][j].isRed = binfo[xsel][ysel].isRed;
		binfo[xsel][ysel].val = NONE;
		if (binfo[i][j].val == GENERAL) {
			if (p1Turn) {
				p1Gen[0] = i;
				p1Gen[1] = j;
			} else {
				p2Gen[0] = i;
				p2Gen[1] = j;
			}
		}
		p1Turn = !p1Turn;
		isSelValid = false;
		draw();

		switch (ms = mateCheck()) {
		case MOKAY:
			break;
		case MCHECK:
			break;
		case P1WON:
		case P2WON:
		case PDRAW:
			inGame = false;
			gameStatus(ms);
			return true;
			break;
		case MERROR:
			isSelValid = false;
			return false;
			break;
		}
		gameStatus(ms);
	} else {
		isSelValid = true;
		LOGF(QString::asprintf("Selected %d at %c %d", binfo[i][j].val, j + 'A', i + 1));
		xsel = i;
		ysel = j;
		putPiece(binfo[i][j].val, i, j, binfo[i][j].isRed ? pred : pgray, bwhite);
	}
	return true;
}

MoveStatus Chess::validMove(int x1, int y1, int x2, int y2)
{
	using namespace std;
	int i, j = 0;
	BoardInfo bsrc = binfo[x1][y1], bdest = binfo[x2][y2];

	//movement check
	if (bsrc.isRed == bdest.isRed && bdest.val != NONE)
		return MERROR;
	switch (bsrc.val) {
	case SOLDIER:
		//TODO: fix check detection for soldier moves
		if (p1Turn) {
			if (y2 < 5 && (x1 != x2 || (y2 - y1) != 1))
				return MERROR;
			if (!((abs(x1 - x2) == 1) ^ ((y2 - y1) == 1))
			   || abs(abs(x1 - x2) + abs(y1 - y2)) != 1)
				return MERROR;
		} else {
			if (y2 > 4 && (x1 != x2 || (y2 - y1) != -1))
				return MERROR;
			if (!((abs(x1 - x2) == 1) ^ ((y2 - y1) == -1))
			   || abs(abs(x1 - x2) + abs(y1 - y2)) != 1)
				return MERROR;
		}
		break;
	case CANNON:
		if (x1 != x2) {
			if (y1 != y2)
				return MERROR;
			for (i = min(x1, x2) + 1, j = 0; i < max(x1, x2); i++)
				if (binfo[i][y1].val != NONE)
					j++;
		} else {
			if (y1 == y2)
				return MERROR;
			for (i = min(y1, y2) + 1, j = 0; i < max(y1, y2); i++)
				if (binfo[x1][i].val != NONE)
					j++;
		}
		if (!((j == 0 && bdest.val == NONE)
		   || (j == 1 && bdest.val != NONE)))
			return MERROR;
		break;
	case CAR:
		if (x1 != x2) {
			if (y1 != y2)
				return MERROR;
			for (i = min(x1, x2) + 1; i < max(x1, x2); i++)
				if (binfo[i][y1].val != NONE)
					return MERROR;
		} else {
			if (y1 == y2)
				return MERROR;
			for (i = min(y1, y2) + 1; i < max(y1, y2); i++)
				if (binfo[x1][i].val != NONE)
					return MERROR;
		}
		break;
	case HORSE:
		if (abs(x1 - x2) > 2 || abs(y1 - y2) > 2
		 || abs(abs(x1 - x2) + abs(y1 - y2)) != 3)
			return MERROR;
		if ((abs(x1 - x2) == 2 && binfo[(x1 + x2) / 2][y1].val != NONE)
		 || (abs(y1 - y2) == 2 && binfo[x1][(y1 + y2) / 2].val != NONE))
			return MERROR;
		break;
	case MINISTER:
		if (abs(x1 - x2) != 2 || abs(y1 - y2) != 2)
			return MERROR;
		if (y2 == 3 || y2 == 6)
			return MERROR;
		if (binfo[(x1 + x2) / 2][(y1 + y2) / 2].val != NONE)
			return MERROR;
		break;
	case ADVISOR:
		if (abs(x1 - x2) != 1 || abs(y1 - y2) != 1)
			return MERROR;
		goto HOUSE_CHECK;
		break;
	case GENERAL:
		if (abs(abs(x1 - x2) + abs(y1 - y2)) != 1)
			return MERROR;
  HOUSE_CHECK:
		if (x2 < 3 || x2 > 5 || y2 == 3 || y2 == 6)
			return MERROR;
		break;
	case NONE:
	default:
		return MERROR;
		break;
	}

	return MOKAY;
}
