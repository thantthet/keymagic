using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;
using System.Drawing.Drawing2D;

namespace kEditor
{
    public partial class GlyphTable : UserControl
    {
        public GlyphTable()
        {
            InitializeComponent();
        }

        private void GlyphTable_Load(object sender, EventArgs e)
        {
            GenerateCharacters();
            GeneratePointAndRect();
        }

        protected override bool ProcessDialogKey(Keys keyData)
        {
            KeyEventArgs kea = new KeyEventArgs(keyData);
            OnKeyDown(kea);
            return true;
        }

        public int NumbersOfGlyph
        {
            get
            {
                return characters.Length;
            }
        }

        private RectangleF GetTransformCellRect(int index)
        {
            RectangleF rect = GetCellRect(index);
            Region region = new Region(rect);
            region.Translate(AutoScrollPosition.X, AutoScrollPosition.Y);
            rect = region.GetBounds(CreateGraphics());
            return rect;
        }

        private Rectangle GetCellRect(int index)
        {
            if (index >= NumberOfRows * NumberOfColumns)
            {
                throw new IndexOutOfRangeException();
            }

            int rowIndex;
            for (rowIndex = 1; rowIndex < NumberOfRows && NumberOfColumns * rowIndex <= index; rowIndex++) ;
            int colIndex = index - NumberOfColumns * (rowIndex - 1);
            colIndex++;

            Point p = new Point(colLinePoints[colIndex - 1].x1, rowLinePoints[rowIndex - 1].y1);
            Size s = new Size(colLinePoints[colIndex].x1 - colLinePoints[colIndex - 1].x1,
                rowLinePoints[rowIndex].y1 - rowLinePoints[rowIndex - 1].y1);

            Rectangle rect = new Rectangle(p, s);
            return rect;
        }

        struct LinePoints
        {
            public int x1;
            public int y1;
            public int x2;
            public int y2;

            public LinePoints(int x1, int y1, int x2, int y2)
            {
                this.x1 = x1;
                this.y1 = y1;
                this.x2 = x2;
                this.y2 = y2;
            }
        }

        private LinePoints[] rowLinePoints;
        private LinePoints[] colLinePoints;
        private Rectangle[] rectangles;
        private Char[] characters;

        private void GeneratePointAndRect()
        {
            if (autoCellCount)
            {
                int i;
                for (i = 1; i * colWidth <= ClientRectangle.Width; i++) ;
                colCount = --i;
                int charCount = glyphRange.Length;
                colCount = colCount > 0 ? colCount : 1;
                rowCount = (charCount + (colCount - (charCount % colCount))) / colCount;
            }

            if (autoCol)
            {
                int i;
                for (i = 1; i * colWidth <= ClientRectangle.Width; i++) ;
                colCount = --i;
                colCount = colCount > 0 ? colCount : 1;
            }

            List<LinePoints> newRowLinePoints = new List<LinePoints>();
            for (int i = 0; i <= NumberOfRows; i++)
            {
                newRowLinePoints.Add(new LinePoints(0, i * rowHeight, colWidth * NumberOfColumns, i * rowHeight));
            }
            rowLinePoints = newRowLinePoints.ToArray();

            List<LinePoints> newColLinePoints = new List<LinePoints>();
            for (int i = 0; i <= NumberOfColumns; i++)
            {
                newColLinePoints.Add(new LinePoints(i * colWidth, 0, i * colWidth, rowHeight * NumberOfColumns));
            }
            colLinePoints = newColLinePoints.ToArray();

            List<Rectangle> newRectanges = new List<Rectangle>();
            for (int i = 0; i < NumberOfColumns * NumberOfRows; i++)
            {
                newRectanges.Add(GetCellRect(i));
            }

            rectangles = newRectanges.ToArray();

            AutoScroll = true;
            AutoScrollMinSize = new Size(colWidth * colCount, rowHeight * rowCount);
        }

        private void GenerateCharacters()
        {
            List<char> charactersList = new List<char>();
            for (int u = glyphRange.First; u < glyphRange.First + glyphRange.Length; u++)
            {
                charactersList.Add((char)u);
            }
            characters = charactersList.ToArray();
        }

        private void GlyphTable_Paint(object sender, PaintEventArgs e)
        {
            Graphics gx = e.Graphics;
            Pen px = new Pen(Color.Black);

            gx.TranslateTransform(AutoScrollPosition.X, AutoScrollPosition.Y);
            gx.FillRectangle(new SolidBrush(BackColor), ClientRectangle);

            if (colCount < 1 && rowCount < 1)
            {
                return;
            }

            /// Draw Selection
            if (selectedAtCell != -1)
            {
                Rectangle selectedRect = rectangles[selectedAtCell];
                LinearGradientBrush gdBrush = new LinearGradientBrush(selectedRect, focusColorEnd, focusColorStart, LinearGradientMode.Vertical);
                gx.FillRectangle(gdBrush, selectedRect);

            }

            StringFormat stringFormat = new StringFormat(StringFormat.GenericDefault);
            stringFormat.Alignment = StringAlignment.Center;
            stringFormat.LineAlignment = StringAlignment.Center;

            Font glyphFont = new Font(Font.Name, 12);
            Font hexFont = new Font("Tahmoa", 8);

            Brush foreBrush = new SolidBrush(ForeColor);
            Brush focusForeBrush = new SolidBrush(focusForeColor);

            Brush hexBrush = new SolidBrush(HexCodeColor);
            Brush focusHexForeBrush = new SolidBrush(focusHexForeColor);

            LinearGradientBrush cellBrush = new LinearGradientBrush(rectangles[0], cellColorEnd, cellColorStart, LinearGradientMode.Vertical);
            LinearGradientBrush alterBrush = new LinearGradientBrush(rectangles[0], alternateCellColorEnd, alternateCellColorStart, LinearGradientMode.Vertical);

            for (int i = 0; i < glyphRange.Length; i++)
            {
                Rectangle cell = rectangles[i];
                Brush textBrush;

                if (((i - i % NumberOfColumns) / NumberOfColumns) % 2 == 0 && selectedAtCell != i)
                {
                    gx.FillRectangle(alterBrush, cell);
                }
                else if (selectedAtCell != i)
                {
                    gx.FillRectangle(cellBrush, cell);
                }
                ControlPaint.DrawVisualStyleBorder(gx, cell);

                textBrush = selectedAtCell == i ? focusForeBrush : foreBrush;
                gx.DrawString(characters[i].ToString(), glyphFont, textBrush,
                    new Rectangle(cell.X, cell.Y, cell.Width, cell.Height - (cell.Height / 3)), stringFormat);

                textBrush = selectedAtCell == i ? focusHexForeBrush : hexBrush;
                gx.DrawString(((int)characters[i]).ToString(codeFormatStr), hexFont, textBrush,
                    new Rectangle(cell.X, cell.Y + (cell.Height - (cell.Height / 3)), cell.Width, (cell.Height / 3)), stringFormat);
            }

            stringFormat.Dispose();

            glyphFont.Dispose();
            hexFont.Dispose();
            px.Dispose();

            /// Draw Border
            //ControlPaint.DrawBorder3D(gx, new Rectangle(0, 0, (colCount * colWidth) + 2, (rowCount * rowHeight) + 2));
        }

        private int hoverAtCell = -1;

        private void GlyphTable_MouseMove(object sender, MouseEventArgs e)
        {

            int currentCell = GetCellAtPoint(e.Location);
            if (hoverAtCell != currentCell && currentCell != -1)
            {
                Invalidate(hoverAtCell, currentCell);
                hoverAtCell = currentCell;
            }
            else if (currentCell == -1)
            {
                hoverAtCell = currentCell;
            }

            if (selectedAtCell != -1 && selectedAtCell < NumbersOfGlyph)
            {
                if (e.Button == System.Windows.Forms.MouseButtons.Left)
                    DoDragDrop(characters[selectedAtCell].ToString(), DragDropEffects.Copy);
            }
        }

        internal char characterAtCell(int index)
        {
            return characters[index];
        }

        internal int GetCellAtPoint(Point point)
        {
            point.X -= AutoScrollPosition.X;
            point.Y -= AutoScrollPosition.Y;
            for (int i = 0; i < NumbersOfGlyph; i++)
            {
                Rectangle rect = rectangles[i];
                if (rect.Contains(point))
                {
                    return i;
                }
            }
            return -1;
        }

        private int selectedAtCell = -1;

        private void GlyphTable_MouseDown(object sender, MouseEventArgs e)
        {
            int currentCell = GetCellAtPoint(e.Location);
            if (selectedAtCell != currentCell && currentCell != -1)
            {
                Invalidate(selectedAtCell, currentCell);
                selectedAtCell = currentCell;
            }
        }

        private void Invalidate(int i1, int i2)
        {
            Rectangle rect;

            rect = rectangles[i2];
            //rect.X -= AutoScrollPosition.X;
            //rect.Y -= AutoScrollPosition.Y;
            Region r = new Region(rect);
            if (i1 != -1)
            {
                rect = rectangles[i1];
                //rect.X -= AutoScrollPosition.X;
                //rect.Y -= AutoScrollPosition.Y;
                r.Union(rect);
            }
            r.Translate(AutoScrollPosition.X, AutoScrollPosition.Y);
            Invalidate(r);
            r.Dispose();
        }

        private void GlyphTable_KeyDown(object sender, KeyEventArgs e)
        {
            switch (e.KeyCode)
            {
                case Keys.Left:
                    if (selectedAtCell == -1)
                    {
                        Invalidate(selectedAtCell, 0);
                        selectedAtCell = 0;
                        MakeSureCellVisible(selectedAtCell);
                    }
                    else if (selectedAtCell == 0) { }
                    else
                    {
                        Invalidate(selectedAtCell, --selectedAtCell);
                        MakeSureCellVisible(selectedAtCell);
                    }
                    break;
                case Keys.Right:
                    if (selectedAtCell == -1)
                    {
                        Invalidate(selectedAtCell, rectangles.Length - 1);
                        selectedAtCell = rectangles.Length - 1;
                        MakeSureCellVisible(selectedAtCell);
                    }
                    else if (selectedAtCell + 1 == glyphRange.Length) { }
                    else
                    {
                        Invalidate(selectedAtCell, ++selectedAtCell);
                        MakeSureCellVisible(selectedAtCell);
                    }
                    break;
                case Keys.Down:
                    if (selectedAtCell == -1)
                    {
                        Invalidate(selectedAtCell, 0);
                        selectedAtCell = 0;
                        MakeSureCellVisible(selectedAtCell);
                    }
                    else if (selectedAtCell + NumberOfColumns >= glyphRange.Length) { }
                    else
                    {
                        Invalidate(selectedAtCell, (selectedAtCell + NumberOfColumns));
                        selectedAtCell += NumberOfColumns;
                        MakeSureCellVisible(selectedAtCell);
                    }
                    break;
                case Keys.Up:
                    if (selectedAtCell == -1)
                    {
                        Invalidate(selectedAtCell, rectangles.Length - 1);
                        selectedAtCell = rectangles.Length - 1;
                        MakeSureCellVisible(selectedAtCell);
                    }
                    else if (selectedAtCell < NumberOfColumns) { }
                    else
                    {
                        Invalidate(selectedAtCell, (selectedAtCell - NumberOfColumns));
                        selectedAtCell -= NumberOfColumns;
                        MakeSureCellVisible(selectedAtCell);
                    }
                    break;
            }
        }

        private void MakeSureCellVisible(int index)
        {
            Rectangle rect = GetCellRect(index);
            if (rect.Y + rect.Height > -AutoScrollPosition.Y + Height)
            {
                SetDisplayRectLocation(0, -(rect.Y + rect.Height - Height));
                AdjustFormScrollbars(true);
            }
            else if (rect.Y < -AutoScrollPosition.Y)
            {
                SetDisplayRectLocation(0, -rect.Y);
                AdjustFormScrollbars(true);
            }
        }

        private void GlyphTable_Resize(object sender, EventArgs e)
        {
            GeneratePointAndRect();
            Invalidate();
        }

        #region GlyphTable Property

        public CharacterRange GlyphRange
        {
            get
            {
                return glyphRange;
            }
            set
            {
                if (value != glyphRange)
                {
                    selectedAtCell = -1;
                    glyphRange = value;
                    GenerateCharacters();
                    GeneratePointAndRect();
                    Invalidate();
                }
            }
        }
        private CharacterRange glyphRange = new CharacterRange(0x0020, 100);

        [Category("Grid"), DefaultValue(true)]
        public bool AutoCellWidth
        {
            get
            {
                return autoCellWidth;
            }
            set
            {
                if (value != autoCellWidth)
                {
                    autoCellWidth = value;
                    if (value == true)
                    {
                        colWidth = ClientRectangle.Width / colCount;
                    }
                    if (AutoSize == true)
                    {
                        this.Width = colWidth * colCount;
                    }
                    GeneratePointAndRect();
                }
            }
        }
        private bool autoCellWidth = true;

        [Category("Grid"), DefaultValue(true)]
        public bool AutoCellCount
        {
            get
            {
                return autoCellCount;
            }
            set
            {
                if (value != autoCellCount)
                {
                    autoCellCount = value;
                    GeneratePointAndRect();
                }
            }
        }
        private bool autoCellCount = true;

        [Category("Grid"), DefaultValue(5)]
        public int NumberOfRows
        {
            get
            {
                return rowCount;
            }
            set
            {
                if (value != rowCount)
                {
                    rowCount = value;
                    GeneratePointAndRect();
                }
            }
        }
        private int rowCount = 5;

        [Category("Grid"),
           DefaultValue(5)]
        public int NumberOfColumns
        {
            get
            {
                return colCount;
            }
            set
            {
                if (value != colCount)
                {
                    colCount = value;
                    GeneratePointAndRect();
                }
            }
        }
        private int colCount = 5;

        [Category("Grid"), DefaultValue(40)]
        public int RowHeight
        {
            get
            {
                return rowHeight;
            }
            set
            {
                if (value != rowHeight)
                {
                    rowHeight = value;
                    GeneratePointAndRect();
                }
            }
        }
        private int rowHeight = 40;

        [Category("Grid"), DefaultValue(40)]
        public int ColumnWidth
        {
            get
            {
                return colWidth;
            }
            set
            {
                if (value != colWidth)
                {
                    colWidth = value;
                    GeneratePointAndRect();
                }
            }
        }
        private int colWidth = 40;

        [Category("Grid"), DefaultValue(true)]
        public bool AutoColumn
        {
            get
            {
                return autoCol;
            }
            set
            {
                if (value != autoCol)
                {
                    autoCol = value;
                    GeneratePointAndRect();
                }
            }
        }
        private bool autoCol = true;

        [Category("Appearance")]
        public Color AlternateGradientCellColorStart
        {
            get
            {
                return alternateCellColorStart;
            }
            set
            {
                if (value != alternateCellColorStart)
                {
                    alternateCellColorStart = value;
                    Invalidate();
                }
            }
        }
        private Color alternateCellColorStart = Color.LightSkyBlue;

        [Category("Appearance")]
        public Color AlternateGradientCellColorEnd
        {
            get
            {
                return alternateCellColorEnd;
            }
            set
            {
                if (value != alternateCellColorEnd)
                {
                    alternateCellColorEnd = value;
                    Invalidate();
                }
            }
        }
        private Color alternateCellColorEnd = Color.LightSkyBlue;

        [Category("Appearance")]
        public Color HexCodeColor
        {
            get
            {
                return hexCodeColor;
            }
            set
            {
                if (value != hexCodeColor)
                {
                    hexCodeColor = value;
                    Invalidate();
                }
            }
        }
        private Color hexCodeColor = Color.Red;

        [Category("Appearance"), DefaultValue(true)]
        public bool HexNotation
        {
            get
            {
                return codeFormatStr == "X";
            }
            set
            {
                if (value != hexNotation)
                {
                    if (value == true) codeFormatStr = "X";
                    else codeFormatStr = "d";
                    hexNotation = value;
                    Invalidate();
                }
            }
        }
        private bool hexNotation = true;
        private string codeFormatStr = "X";

        [Category("Appearance")]
        public Color CellGradientColorStart
        {
            get
            {
                return cellColorStart;
            }
            set
            {
                if (value != cellColorStart)
                {
                    cellColorStart = value;
                    Invalidate();
                }
            }
        }
        private Color cellColorStart = Color.LightSteelBlue;

        [Category("Appearance")]
        public Color CellGradientColorEnd
        {
            get
            {
                return cellColorEnd;
            }
            set
            {
                if (value != cellColorEnd)
                {
                    cellColorEnd = value;
                    Invalidate();
                }
            }
        }
        private Color cellColorEnd = Color.CornflowerBlue;

        [Category("Appearance")]
        public Color FoucusGradientColorStart
        {
            get
            {
                return focusColorStart;
            }
            set
            {
                if (value != focusColorStart)
                {
                    focusColorStart = value;
                    Invalidate();
                }
            }
        }
        private Color focusColorStart = Color.LimeGreen;

        [Category("Appearance")]
        public Color FoucusGradientColorEnd
        {
            get
            {
                return focusColorEnd;
            }
            set
            {
                if (value != focusColorEnd)
                {
                    focusColorEnd = value;
                    Invalidate();
                }
            }
        }
        private Color focusColorEnd = Color.Lime;

        [Category("Appearance")]
        public Color FoucusForeColor
        {
            get
            {
                return focusForeColor;
            }
            set
            {
                if (value != focusForeColor)
                {
                    focusForeColor = value;
                    Invalidate();
                }
            }
        }
        private Color focusForeColor = Color.White;

        [Category("Appearance")]
        public Color FoucusHexForeColor
        {
            get
            {
                return focusHexForeColor;
            }
            set
            {
                if (value != focusHexForeColor)
                {
                    focusHexForeColor = value;
                    Invalidate();
                }
            }
        }
        private Color focusHexForeColor = Color.White;

        #endregion
    }
}
