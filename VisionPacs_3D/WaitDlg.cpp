#include "stdafx.h"
#include "WaitDlg.h"

WaitDlg::WaitDlg(QWidget *parent)
	: QDialog(parent),
	ui(new Ui::WaitDlg)
	, m_bShow(false)
{
	ui->setupUi(this);
	setStyleSheet("QDialog{border:2px solid grey;background-color: #323232;} ");
}

WaitDlg::~WaitDlg()
{
	delete ui;
}

void WaitDlg::paintEvent(QPaintEvent *event)
{
	if (m_bShow == false)
	{
		emit StartProcess();
		m_bShow = true;
	}
}

void WaitDlg::SetProgress(int nValue)
{
	ui->LoadProgressBar->setValue(nValue);
	
	if (nValue == 100)
	{ 
		done(0);
		ui->LoadProgressBar->setValue(0);
		m_bShow = false;		
	}
}
