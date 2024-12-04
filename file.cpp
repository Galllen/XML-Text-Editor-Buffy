#include "file.h"
#include "ui_file.h"
#include <QPushButton>

File::File(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::File)
{
    ui->setupUi(this);
    connect(ui->buttonBox->button(QDialogButtonBox::Ok),SIGNAL(clicked()),SLOT(okCliced()));
    connect(ui->buttonBox->button(QDialogButtonBox::Cancel),SIGNAL(clicked()),SLOT(close()));
}

void File::okCliced(){
    emit filePath(ui->lineEdit->text());
    close();
}


File::~File()
{
    delete ui;
}
