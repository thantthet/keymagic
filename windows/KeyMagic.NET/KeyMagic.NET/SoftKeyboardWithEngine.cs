using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using KeyMagicDotNet;
using System.Diagnostics;
using System.Drawing;

namespace KeyMagic
{
    class SoftKeyboardWithEngine : SoftKeyboard
    {
        private KeyMagicEngine engine = null;
        // we use this engine for getting output from engine
        private KeyMagicEngine cloned_engine = null;

        /// <summary>
        ///  
        /// </summary>
        public KeyMagicEngine Engine
        {
            get { return engine; }
            set
            {
                if (engine != value)
                {
                    engine = value;
                    if (cloned_engine != null)
                    {
                        cloned_engine.Dispose();
                        cloned_engine = null;
                    }

                    if (engine != null)
                    {
                        cloned_engine = new KeyMagicEngine(engine);
                        SwitchMap = engine.GetSwitchMap();
                        ButtonFontFamily = engine.GetKeyboard().GetInfoList().GetFontFamily();
                    }
                    else
                    {
                        ButtonFontFamily = null;
                    }
                    Refresh(RefreshFor.Label);
                }
                else if (value == null)
                {
                    Refresh(RefreshFor.Label);
                }
            }
        }

        /// <summary>
        ///  
        /// </summary>
        public SoftKeyboardWithEngine(KeyMagicEngine _engine)
            : base()
        {
            Engine = _engine;
        }

        /// <summary>
        ///  
        /// </summary>
        protected override string GetDisplayStringFromVK(uint vkey)
        {
            bool success = false;
            if (cloned_engine == null)
            {
                return base.GetDisplayStringFromVK(vkey);
            }

            cloned_engine.Reset();
            cloned_engine.SetSwitchMap(SwitchMap);

            success = processKey(vkey, cloned_engine);

            if (!success)
            {
                return base.GetDisplayStringFromVK(vkey);
            }

            return cloned_engine.GetContextText();
        }

        /// <summary>
        ///  
        /// </summary>
        private bool processKey(uint scanCode, KeyMagicEngine e)
        {
            if (e == null)
            {
                return false;
            }

            IntPtr HKLCurrent = (IntPtr)0x04090409;

            if (!e.Options.posBased)
            {
                uint dwProcessId;
                uint threadId = NativeMethods.GetWindowThreadProcessId(NativeMethods.GetForegroundWindow(), out dwProcessId);
                HKLCurrent = NativeMethods.GetKeyboardLayout(threadId);
            }

            byte[] keyboardState = GetVirtualizedKeyboardState();
            e.SetKeyStates(keyboardState);

            uint keyval = GetKeyValue(scanCode, keyboardState, HKLCurrent);
            int modifier = GetKeyMagicEngineModifier();

            return e.ProcessKeyEvent((int)keyval, (int)GetVK(scanCode, HKLCurrent), modifier);
        }

        /// <summary>
        ///  
        /// </summary>
        protected override bool IsSwitchKey(uint scanCode)
        {
            if (cloned_engine == null)
            {
                return base.IsSwitchKey(scanCode);
            }

            cloned_engine.Reset();
            if (processKey(scanCode, cloned_engine) && cloned_engine.GetContextText().Length == 0 && cloned_engine.GetSwitchMap().Count > 0)
            {
                return true;
            }

            return base.IsSwitchKey(scanCode);
        }

        /// <summary>
        ///  
        /// </summary>
        private int GetKeyMagicEngineModifier()
        {
            int modifier = 0;

            if (Control)
            {
                modifier |= (int)KeyMagicDotNet.KeyMagicEngine.ModifierMask.CTRL_MASK;
            }
            if (Alt)
            {
                modifier |= (int)KeyMagicDotNet.KeyMagicEngine.ModifierMask.ALT_MASK;
            }
            if (Shift)
            {
                modifier |= (int)KeyMagicDotNet.KeyMagicEngine.ModifierMask.SHIFT_MASK;
            }

            return modifier;
        }

        /// <summary>
        ///  
        /// </summary>
        //protected override void SimulateKeyPress(SoftKey key)
        //{
        //    base.SimulateKeyPress(key);
        //    return;
        //    if (Engine == null)
        //    {
        //        base.SimulateKeyPress(key);
        //        return;
        //    }

        //    byte[] keyboardState = GetVirtualizedKeyboardState();

        //    IntPtr HKLCurrent = (IntPtr)0x04090409;

        //    if (!engine.Options.posBased)
        //    {
        //        uint dwProcessId;
        //        uint threadId = NativeMethods.GetWindowThreadProcessId(NativeMethods.GetForegroundWindow(), out dwProcessId);
        //        HKLCurrent = NativeMethods.GetKeyboardLayout(threadId);
        //    }

        //    int vkey = (int)GetVK(key.ScanCode, HKLCurrent);

        //    int keyvalue = (int)base.GetKeyValue(key.ScanCode, keyboardState, HKLCurrent);

        //    String contextBefore = engine.GetContextText();

        //    engine.SetKeyStates(keyboardState);

        //    if (engine.ProcessKeyEvent(keyvalue, vkey, GetKeyMagicEngineModifier()))
        //    {
        //        SendInput.SendDifference(contextBefore, engine.GetContextText());
        //        ResetModifiersAsNeeded((int)key.ScanCode);
        //    }
        //    else
        //    {
        //        base.SimulateKeyPress(key);
        //    }
        //}

        Dictionary<int, bool> SwitchMap = new Dictionary<int,bool>();

        protected override void GetKeyStatesTimer_Tick(object sender, EventArgs e)
        {
            if (engine != null && !IsEqualSwitch(SwitchMap, engine.GetSwitchMap()))
            {
                SwitchMap = engine.GetSwitchMap();
                Refresh(RefreshFor.Label);
            }
            base.GetKeyStatesTimer_Tick(sender, e);
        }

        private bool IsEqualSwitch(Dictionary<int, bool> d1, Dictionary<int, bool> d2)
        {
            if (d1.Count != d2.Count)
            {
                return false;
            }

            foreach (var e in d1)
            {
                if (e.Value != d2[e.Key])
                {
                    return false;
                }
            }

            return true;
        }
    }
}
