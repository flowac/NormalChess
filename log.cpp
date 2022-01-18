#include "lib.h"

void LOGF(QString str, int level)
{
	static bool _INIT_ = false;
	static QFile fd("game.log");
	static QTextStream outs;
	QString timeVal = QDateTime::currentDateTime().toString("mm:ss") + "\t";

	if (logArea)
		logArea->append(timeVal + str);
	if (!_INIT_) {
		if (!fd.open(QIODevice::WriteOnly | QIODevice::Text))
			return;
		outs.setDevice(&fd);
		_INIT_ = true;
	}

	switch (level) {
	case 0:
		outs << timeVal << str << "\n";
		break;
	case 1:
		outs << str << "\n";
		break;
	case -1:
		LOGF(str);
		outs << "_EOF_\n\n";
		_INIT_ = false;
		fd.close();
		break;
	default:
		outs << "WARNING: Invalid log level\t" << str;
		outs.flush();
		break;
	}
}
