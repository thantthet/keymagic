using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ScintillaNET;
using System.Text.RegularExpressions;

namespace kEditor
{
    class KmsLexer
    {
        public static class STYLE
        {
            public const int Default    = 0;
            public const int Keyword    = 1;
            public const int Identifier = 2;
            public const int String     = 4;
            public const int Comment    = 5;
            public const int Variable   = 6;
            public const int HexCode    = 7;
            public const int Include    = 8;
            public const int KeyState   = 9;
            public const int Switch     = 10;
            public const int Reference  = 11;
        }

        private enum STATE
        {
            UNKNOWN, IDENTIFIER, STRING, COMMENT, BLOCK_COMMENT, DOLLAR, REFERENCE, VARIABLE, HEXCODE, OPEN_BRACKET, SWITCH_STRING, EXPECT_CLOSE_BRACKET, KEYWORD
        }

        private HashSet<string> keywords;

        public void Style(Scintilla scintilla, int startPos, int endPos)
        {
            // Back up to the line start
            var line = scintilla.LineFromPosition(startPos);
            startPos = scintilla.Lines[line].Position;

            var length = 0;
            var state = STATE.UNKNOWN;
            var quoteChar = '"';

            // Start styling
            scintilla.StartStyling(startPos);
            while (startPos < endPos)
            {
                var c = (char)scintilla.GetCharAt(startPos);
                var cc = (char)scintilla.GetCharAt(startPos + 1);

            REPROCESS:
                switch (state)
                {
                    case STATE.KEYWORD:
                        if (c == '(')
                        {
                            scintilla.SetStyling(1, STYLE.Default);
                            state = STATE.UNKNOWN;
                            break;
                        }
                        state = STATE.UNKNOWN;
                        goto REPROCESS;
                    case STATE.UNKNOWN:
                        if (c == '"' || c == '\'')
                        {
                            // Start of "string"
                            scintilla.SetStyling(1, STYLE.String);
                            state = STATE.STRING;
                            quoteChar = c;
                        }
                        else if (c == '/' && cc == '/')
                        {
                            scintilla.SetStyling(2, STYLE.Comment);
                            startPos++;
                            state = STATE.COMMENT;
                        }
                        else if (c == '/' && cc == '*')
                        {
                            scintilla.SetStyling(2, STYLE.Comment);
                            startPos++;
                            state = STATE.BLOCK_COMMENT;
                        }
                        else if (c == '$')
                        {
                            state = STATE.DOLLAR;
                            length++;
                        }
                        else if (c == 'U')
                        {
                            scintilla.SetStyling(1, STYLE.HexCode);
                            state = STATE.HEXCODE;
                        }
                        else if (c == '(')
                        {
                            state = STATE.OPEN_BRACKET;
                            length++;
                        }
                        else if (Char.IsLetter(c))
                        {
                            state = STATE.IDENTIFIER;
                            goto REPROCESS;
                        }
                        else
                        {
                            // Everything else
                            scintilla.SetStyling(1, STYLE.Default);
                        }
                        break;

                    case STATE.OPEN_BRACKET:
                        if (c == '"' || c == '\'')
                        {
                            // Start of "switch string"
                            length++;
                            state = STATE.SWITCH_STRING;
                            quoteChar = c;
                        }
                        else if (c == ' ')
                        {
                            length++;
                        }
                        break;

                    case STATE.SWITCH_STRING:
                        if (c == quoteChar)
                        {
                            length++;
                            state = STATE.EXPECT_CLOSE_BRACKET;
                        }
                        else
                        {
                            length++;
                        }
                        break;

                    case STATE.EXPECT_CLOSE_BRACKET:
                        if (c == ')')
                        {
                            length++;
                            scintilla.SetStyling(length, STYLE.Switch);
                            length = 0;
                            state = STATE.UNKNOWN;
                        } else if (c == ' ')
                        {
                            length++;
                        }
                        break;

                    case STATE.STRING:
                        if (c == quoteChar)
                        {
                            length++;
                            scintilla.SetStyling(length, STYLE.String);
                            length = 0;
                            state = STATE.UNKNOWN;
                        }
                        else
                        {
                            length++;
                        }
                        break;

                    case STATE.COMMENT:
                        if (c == '\n')
                        {
                            length++;
                            scintilla.SetStyling(length, STYLE.Comment);
                            length = 0;
                            state = STATE.UNKNOWN;
                        }
                        else
                        {
                            length++;
                        }
                        break;

                    case STATE.BLOCK_COMMENT:
                        if (c == '*' && cc == '/')
                        {
                            length++;
                            length++;
                            startPos++;
                            scintilla.SetStyling(length, STYLE.Comment);
                            length = 0;
                            state = STATE.UNKNOWN;
                        }
                        else
                        {
                            length++;
                        }
                        break;

                    case STATE.IDENTIFIER:
                        if (Char.IsLetterOrDigit(c) || c == '_')
                        {
                            length++;
                        }
                        else
                        {
                            var style = STYLE.Identifier;
                            var identifier = scintilla.GetTextRange(startPos - length, length);
                            state = STATE.UNKNOWN;
                            if (keywords.Contains(identifier))
                            {
                                style = STYLE.Keyword;
                                state = STATE.KEYWORD;
                            }
                            else if (identifier == "include")
                            {
                                style = STYLE.Include;
                                state = STATE.KEYWORD;
                            }

                            scintilla.SetStyling(length, style);
                            length = 0;
                            goto REPROCESS;
                        }
                        break;

                    case STATE.DOLLAR:
                        if (Char.IsDigit(c))
                        {
                            state = STATE.REFERENCE;
                            length++;
                        }
                        else
                        {
                            state = STATE.VARIABLE;
                            length++;
                        }
                        break;

                    case STATE.REFERENCE:
                        if (Char.IsDigit(c))
                        {
                            length++;
                        }
                        else
                        {
                            scintilla.SetStyling(length, STYLE.Reference);
                            length = 0;
                            state = STATE.UNKNOWN;
                            goto REPROCESS;
                        }
                        break;

                    case STATE.VARIABLE:
                        if (Char.IsLetterOrDigit(c) || c == '_')
                        {
                            length++;
                        }
                        else
                        {
                            scintilla.SetStyling(length, STYLE.Variable);
                            length = 0;
                            state = STATE.UNKNOWN;
                            goto REPROCESS;
                        }
                        break;

                    case STATE.HEXCODE:
                        if (Char.IsLetterOrDigit(c) && length < 4)
                        {
                            length++;
                        }
                        else
                        {
                            scintilla.SetStyling(length, STYLE.HexCode);
                            length = 0;
                            state = STATE.UNKNOWN;
                            goto REPROCESS;
                        }
                        break;
                }

                startPos++;
            }
        }

        public KmsLexer(string keywords)
        {
            // Put keywords in a HashSet
            var list = Regex.Split(keywords ?? string.Empty, @"\s+").Where(l => !string.IsNullOrEmpty(l));
            this.keywords = new HashSet<string>(list);
        }
    }
}