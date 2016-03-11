#include "MainWindow.h"

#include "TextTable.h"
#include "ui_MainWindow.h"

using namespace tt;

// public

MainWindow::MainWindow(QWidget* parent) :
   QMainWindow(parent),
   m_table(new TextTable(this)),
   ui(new Ui::MainWindow)
{
   ui->setupUi(this);
   m_table->resize(ui->numRows->value(), ui->numColumns->value());
   m_table->setTextWidth(ui->textWidth->value());

   connect(ui->optimize, SIGNAL(clicked()), m_table, SLOT(optimize()));
   connect(m_table, SIGNAL(text(QString)),
           ui->text, SLOT(setPlainText(QString)));
   connect(m_table, SIGNAL(text(int, int, QString)),
           this, SLOT(setText(int, int, QString)));
}

MainWindow::~MainWindow()
{
   delete ui;
}

// private slots

void MainWindow::on_numColumns_valueChanged(int columns)
{
   ui->table->setColumnCount(columns);
   m_table->resizeColumns(columns);
}

void MainWindow::on_numRows_valueChanged(int rows)
{
   ui->table->setRowCount(rows);
   m_table->resizeRows(rows);
}

void MainWindow::on_table_cellChanged(int row, int column)
{
   m_table->setValue(row, column, ui->table->item(row, column)->text());
}

void MainWindow::on_textWidth_valueChanged(int textWidth)
{
   m_table->setTextWidth(textWidth);
}

void MainWindow::setText(int row, int column, QString text)
{
   ui->table->item(row, column)->setText(text);
}
