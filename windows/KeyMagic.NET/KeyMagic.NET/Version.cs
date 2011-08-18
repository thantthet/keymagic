using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace KeyMagic
{
    class Version
    {
        string versionString;
        public Version(string VersionString)
        {
            versionString = VersionString;
        }

        public static bool operator >(Version v1, Version v2)
        {
            if (v1 == v2)
            {
                return false;
            }

            string[] s1 = v1.versionString.Split(new char[] { '.' }, 4);
            string[] s2 = v2.versionString.Split(new char[] { '.' }, 4);

            if (Convert.ToInt32(s1[0]) > Convert.ToInt32(s2[0]) ||
                Convert.ToInt32(s1[1]) > Convert.ToInt32(s2[1]) ||
                Convert.ToInt32(s1[2]) > Convert.ToInt32(s2[2]) ||
                Convert.ToInt32(s1[3]) > Convert.ToInt32(s2[3]))
            {
                return true;
            }

            return false;
        }

        public static bool operator <(Version v1, Version v2)
        {
            if (v1 == v2)
            {
                return false;
            }

            return !(v1 > v2);
        }
    }
}
