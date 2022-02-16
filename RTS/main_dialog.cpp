#include "main_dialog.h"
#include "sequence.h"
#include <QFileDialog>

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Q_Main_Dialog)
{
    ui->setupUi(this);

    ui->checkBoxSequenceMode->setChecked(true);
    ui->checkBoxManualMode->setChecked(false);
}

MainDialog::~MainDialog()
{
    delete ui;
}

void MainDialog::on_checkBoxSequenceMode_clicked(bool checked)
{
    ui->pushButtonSelectSequence->setEnabled(checked);
    ui->lineEditSequencePath->setReadOnly(!checked);
    ui->pushButtonCompile->setEnabled(checked);
    ui->textEditCompileError->setReadOnly(!checked);
    ui->checkBoxManualMode->setChecked(!checked);
}

void MainDialog::on_pushButtonSelectSequence_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,tr("Select Main Sequence"),"",tr("Sequence File(*.ts)"));
    ui->lineEditSequencePath->setText(path);

    on_pushButtonCompile_clicked();
}

void MainDialog::on_pushButtonCompile_clicked()
{
    sequence s(ui->lineEditSequencePath->text().toStdString());
    int32_t r = s.compile();

    ui->textEditCompileError->setText(QString::fromStdString(s.err()));
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!r);
}

void MainDialog::on_checkBoxManualMode_clicked(bool checked)
{
    ui->checkBoxSequenceMode->setChecked(!checked);
    ui->pushButtonSelectSequence->setEnabled(!checked);
    ui->lineEditSequencePath->setReadOnly(checked);
    ui->pushButtonCompile->setEnabled(!checked);
    ui->textEditCompileError->setReadOnly(checked);
}

void MainDialog::on_buttonBox_accepted()
{

}

void MainDialog::on_buttonBox_rejected()
{
    QApplication::exit();
}


