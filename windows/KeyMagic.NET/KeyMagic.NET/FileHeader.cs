using System;
using System.Collections.Generic;
using System.Text;

namespace KeyMagic
{
    class FileHeader
    {
        public struct LayoutOptions
        {
            public bool trackCaps;
            public bool autoBksp;
            public bool eat;
            public bool posBased;
        }

        public FileHeader(System.IO.BinaryReader br)
        {
            majorVersion = 0;
            minorVersion = 0;
            stringCount = 0;
            infoCount = 0;
            ruleCount = 0;
            layoutOptions = new LayoutOptions();

            byte[] magicCode = br.ReadBytes(4);
            if (magicCode[0] != 'K' && magicCode[1] != 'M' && magicCode[2] != 'K' && magicCode[3] != 'L')
            {
                throw new Exception("Invalid KeyMagic keyboard layout file.");
            }

            majorVersion = br.ReadByte();
            minorVersion = br.ReadByte();

            if (majorVersion == 1 && minorVersion > 4)
            {
                throw new Exception("Cannot load newer keyboard file.");
            }

            stringCount = br.ReadInt16();
            if (majorVersion == 1 && minorVersion > 3)
            {
                infoCount = br.ReadInt16();
            }
            ruleCount = br.ReadInt16();

            layoutOptions.trackCaps = br.ReadBoolean();
            layoutOptions.autoBksp = br.ReadBoolean();
            layoutOptions.eat = br.ReadBoolean();
            layoutOptions.posBased = br.ReadBoolean();
        }

        public int majorVersion;
        public int minorVersion;
        public int stringCount;
        public int infoCount;
        public int ruleCount;
        public LayoutOptions layoutOptions;
    }
}
