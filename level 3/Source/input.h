#pragma once
#include <QtWidgets/QDialog>
#include "ui_input.h"
using namespace std;

class input : public QDialog
{
	Q_OBJECT
public:
	input(QWidget* parent = Q_NULLPTR);
	int readnum()
	{
		return stoi(strValue);
	}
	void reject()
	{
		QDialog::reject();
	}
signals:
	void Sdata(QString text);

public slots:
	void sclick(); // �Է� �� Ȯ�� �� ó��

private:
	Ui::input ui;
	string strValue = "";
};
