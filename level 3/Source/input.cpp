#include "input.h"
#include <QMessageBox>

input::input(QWidget* parent) : QDialog(parent)
{
    ui.setupUi(this);
    connect(ui.pushButton, &QPushButton::clicked, this, &input::sclick);
}

void input::sclick() 
{
    if (!ui.lineEdit->text().isEmpty())
    {
        strValue = ui.lineEdit->text().toStdString();
        accept();
        close();
    }
    else
    {
        QMessageBox::warning(this, "Input Error", "Please enter a value.");
    }
}
