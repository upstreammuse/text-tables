#ifndef TEXTTABLE_H
#define TEXTTABLE_H

#include <QObject>
#include <QPair>
#include <QStringList>
#include <QVector>

namespace tt { class TextTable; }

class tt::TextTable : public QObject
{
   Q_OBJECT

public:
   explicit TextTable(QObject* parent = 0);

public slots:
   void optimize() const;
   void resize(int rows, int columns);
   void resizeColumns(int columns);
   void resizeRows(int rows);
   void setTextWidth(int textWidth);
   void setValue(int row, int column, QString value);

signals:
   void text(QString value) const;
   void text(int row, int column, QString value) const;

private:
   void format(const QVector<int>& widths) const;
   int maxWidth(int column) const;
   int minWidth(int column) const;
   int optimize(QVector<int>& answer, int& answerHeight, QVector<int> widths,
                int column) const;
   int sumWidths(const QVector<int>& widths) const;
   int tableHeight(const QVector<int>& widths) const;

private:
   QVector<QVector<QStringList> > m_cells;
   int m_columnCount;
   int m_rowCount;
   int m_textWidth;
};

#endif
