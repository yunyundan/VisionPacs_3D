#include "stdafx.h"
#include "VisionPacs_3D.h"
#include <QtWidgets/QApplication>

#ifdef _DEBUG 
//#include "vld.h" 
#endif

int main(int argc, char *argv[])
{
	LBase::LoadLibraries(LT_KRN | LT_FIL | LT_DIS | LT_DLG | LT_IMG);
	WRPUNLOCKSUPPORT();
	LSettings::UnlockSupport(L_SUPPORT_MEDICAL, L_KEY_MEDICAL);

	QApplication a(argc, argv);

	QFile file("Image/style.css");
	file.open(QFile::ReadOnly);
	QString qss = QLatin1String(file.readAll());
	qApp->setStyleSheet(qss);

	VisionPacs_3D w;
	w.show();
	return a.exec();
}
