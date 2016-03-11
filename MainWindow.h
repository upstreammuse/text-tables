#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace tt { class MainWindow; }
namespace tt { class TextTable; }
namespace Ui { class MainWindow; }

class tt::MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   explicit MainWindow(QWidget* parent = 0);
   ~MainWindow();

private slots:
   void on_numColumns_valueChanged(int columns);
   void on_numRows_valueChanged(int rows);
   void on_table_cellChanged(int row, int column);
   void on_textWidth_valueChanged(int textWidth);
   void setText(int row, int column, QString text);

private:
   TextTable* m_table;
   Ui::MainWindow* ui;
};

#endif
