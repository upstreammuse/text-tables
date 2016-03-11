#include "TextTable.h"

using namespace tt;

// public

TextTable::TextTable(QObject* parent) :
   QObject(parent),
   m_columnCount(0),
   m_rowCount(0),
   m_textWidth(80)
{
}

void TextTable::optimize() const
{
   QVector<int> answer;
   int answerHeight(-1);
   QVector<int> widths;
   optimize(answer, answerHeight, widths, 0);
   if (answerHeight != -1)
   {
      format(answer);
   }
   else
   {
      emit text("Table requires more columns\n");
   }
}

void TextTable::resize(int rows, int columns)
{
   m_rowCount = rows;
   m_columnCount = columns;

   m_cells.resize(rows);
   for (int i = 0; i < rows; ++i)
   {
      m_cells[i].resize(columns);
   }
}

void TextTable::resizeColumns(int columns)
{
   resize(m_rowCount, columns);
}

void TextTable::resizeRows(int rows)
{
   resize(rows, m_columnCount);
}

void TextTable::setTextWidth(int textWidth)
{
   m_textWidth = textWidth;
}

void TextTable::setValue(int row, int column, QString value)
{
   m_cells[row][column] = value.split(QRegExp("\\s+"), QString::SkipEmptyParts);
   emit text(row, column, m_cells[row][column].join(" "));
}

// private


int TextTable::tableHeight(const QVector<int>& widths) const
{
   int tableHeight(0);
   for (int row(0); row < m_rowCount; ++row)
   {
      int rowHeight(0);
      for (int column(0); column < m_columnCount; ++column)
      {
         int height(0);
         int width(0);
         for (int word(0); word < m_cells[row][column].size(); ++word)
         {
            int wordWidth(m_cells[row][column][word].size());
            if (widths[column] < wordWidth)
            {
               return -1;
            }
            else if (word != 0 && width + wordWidth + 1 <= widths[column])
            {
               width += wordWidth + 1;
            }
            else
            {
               ++height;
               width = wordWidth;
            }
         }
         if (height > rowHeight)
         {
            rowHeight = height;
         }
      }
      tableHeight += rowHeight;
   }
   return tableHeight;
}

void TextTable::format(const QVector<int>& widths) const
{
   // start with no lines
   QStringList lines;
   int line = -1;

   // add a table start line
   lines += QString(sumWidths(widths), '-');
   ++line;

   // iterate over the rows of the table
   for (int row = 0; row < m_rowCount; ++row)
   {
      // if this isn't the header or first row, add the blank line
      if (row != 0 && row != 1)
      {
         lines += QString();
         ++line;
      }

      // each row resets the "max line" and the cumulative column width
      int maxLine = line;
      int width = 0;

      // iterate over the columns of the table
      for (int col = 0; col < m_columnCount; ++col)
      {
         // if it is not the first column, add a column border
         if (col != 0)
         {
            ++width;
         }

         // each column gets its own line counter based on the current line
         int columnLine = line;

         // each column adds its own width to the cumulative width
         width += widths[col];

         // iterate over the words in the column
         for (int word = 0; word < m_cells[row][col].size(); ++word)
         {
            // handy reference to the current word
            const QString& wordRef = m_cells[row][col][word];

            // if this is a continuation of a line, add the word
            if (word != 0
                && lines[columnLine].size() + wordRef.size() + 1 <= width)
            {
               lines[columnLine] += " ";
               lines[columnLine] += wordRef;
            }

            // otherwise this is a new line for this column, prepare it before
            // adding
            else
            {
               ++columnLine;
               if (columnLine > maxLine)
               {
                  lines += QString();
                  maxLine = columnLine;
               }
               lines[columnLine] += QString(width - widths[col]
                                            - lines[columnLine].size(), ' ');
               lines[columnLine] += wordRef;
            }
         }

         // if this column beat the "max line" count, update it
         if (columnLine > maxLine)
         {
            maxLine = columnLine;
         }
      }

      // update the line count based on the "max line" we reached
      line = maxLine;

      // if this is the first row, add the table header line
      if (row == 0)
      {
         QString header;
         for (int i = 0; i < m_columnCount; ++i)
         {
            if (i != 0)
            {
               header += " ";
            }
            header += QString(widths[i], '-');
         }
         lines += header;
         ++line;
      }
   }

   // add a table end line
   lines += QString(sumWidths(widths), '-');
   ++line;

   // flatten the list into a single string
   emit text(lines.join("\n"));
}

int TextTable::maxWidth(int column) const
{
   int width(1);
   for (int row(0); row < m_rowCount; ++row)
   {
      QString line(m_cells[row][column].join(" "));
      width = std::max(width, line.size());
   }
   return width;
}

int TextTable::minWidth(int column) const
{
   int width(1);
   for (int row(0); row < m_rowCount; ++row)
   {
      for (int word(0); word < m_cells[row][column].size(); ++word)
      {
         width = std::max(width, m_cells[row][column][word].size());
      }
   }
   return width;
}

int TextTable::optimize(QVector<int>& answer, int& answerHeight,
                        QVector<int> widths, int column) const
{
   // end of recursion, we have exceeded the number of columns we have
   if (column >= m_columnCount)
   {
      return 1;
   }

   // add this column to the current solution attempt, and iterate widths
   widths.resize(column + 1);
   int max(maxWidth(column));
   for (widths[column] = minWidth(column);
        widths[column] <= max && sumWidths(widths) <= m_textWidth;
        ++widths[column])
   {
      // recurse
      int status(optimize(answer, answerHeight, widths, column + 1));

      // if this is the last column, use the better, valid answer
      if (status == 1)
      {
         int height(tableHeight(widths));
         if (answerHeight == -1 || (height != -1 && height < answerHeight))
         {
            answer = widths;
            answerHeight = height;

            // return if we have a "best" answer
            if (answerHeight == m_rowCount)
            {
               return -1;
            }
         }
      }

      // somewhere we have a "best" answer, keep returning
      else if (status == -1)
      {
         return -1;
      }
   }

   // not the end of recursion
   return 0;
}

int TextTable::sumWidths(const QVector<int>& widths) const
{
   int sum = widths.size() - 1;
   for (int i = 0; i < widths.size(); ++i)
   {
      sum += widths[i];
   }
   return sum;
}
