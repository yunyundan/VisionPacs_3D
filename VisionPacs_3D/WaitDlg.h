#ifndef WAITDLG_H
#define WAITDLG_H

#include <QDialog>
#include "ui_WaitDlg.h"

class WaitDlg : public QDialog
{
	Q_OBJECT

public:
	WaitDlg(QWidget *parent = 0);
	~WaitDlg();

private:
	Ui::WaitDlg *ui;

	bool m_bShow;

protected:
	void paintEvent(QPaintEvent *event);

private slots:
	void SetProgress(int nValue);

signals:
	void StartProcess();
};

#endif // WAITDLG_H
