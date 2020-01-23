using System.Drawing;

namespace kEditor
{
    class SciStyleFont
    {
        public static Font GetFont(Styler.StyleCopy style)
        {
            FontStyle fs = FontStyle.Regular;

            if (style.Italic)
            {
                fs = FontStyle.Italic;
            } else if (style.Bold)
            {
                fs = FontStyle.Bold;
            } else if (style.Underline)
            {
                fs = FontStyle.Underline;
            }

            return new Font(style.Font, style.Size, fs);
        }
    }
}
