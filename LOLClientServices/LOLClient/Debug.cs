﻿using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace LOLClient
{
    public class Debug
    {
        public static void Log(string message)
        {
            MessageBox.Show(message);
        }
    }
}
