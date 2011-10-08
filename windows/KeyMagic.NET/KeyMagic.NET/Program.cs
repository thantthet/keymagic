using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Threading;
using System.Security.AccessControl;
using System.Security.Principal;

namespace KeyMagic
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(true);

            string title = "KeyMagic";

            bool beta = Properties.Settings.Default.BetaRelease;

            if (beta) title += " (beta)";

            string mutexName = "\u1000\u1001";

            // http://stackoverflow.com/questions/229565/what-is-a-good-pattern-for-using-a-global-mutex-in-c/229567
            using (var mutex = new Mutex(false, mutexName))
            {
                // edited by Jeremy Wiebe to add example of setting up security for multi-user usage
                // edited by 'Marc' to work also on localized systems (don't use just "Everyone") 
                var allowEveryoneRule = new MutexAccessRule(new SecurityIdentifier(WellKnownSidType.WorldSid, null), MutexRights.FullControl, AccessControlType.Allow);
                var securitySettings = new MutexSecurity();
                securitySettings.AddAccessRule(allowEveryoneRule);
                mutex.SetAccessControl(securitySettings);

                //edited by acidzombie24
                var hasHandle = false;
                try
                {
                    try
                    {
                        // note, you may want to time out here instead of waiting forever
                        //edited by acidzombie24
                        //mutex.WaitOne(Timeout.Infinite, false);
                        hasHandle = mutex.WaitOne(100, false);
                        if (hasHandle == false) //another instance exist
                        {
                            IntPtr pWnd = NativeMethods.FindWindow(null, title);
                            if (pWnd != IntPtr.Zero)
                            {
                                NativeMethods.ShowWindow(pWnd, 0x05);
                            }
                            return;
                        }
                    }
                    catch (AbandonedMutexException)
                    {
                        // Log the fact the mutex was abandoned in another process, it will still get aquired
                    }

                    frmMain f = new frmMain();
                    Application.Run();
                }
                finally
                {
                    //edit by acidzombie24, added if statemnet
                    if (hasHandle)
                        mutex.ReleaseMutex();
                }
            }
        }
    }
}
