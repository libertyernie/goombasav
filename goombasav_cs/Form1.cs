﻿using Goombasav;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace goombasav_cs {
	public partial class Form1 : Form {
		public Form1() {
			InitializeComponent();

			GoombaSRAM sram = new GoombaSRAM(File.ReadAllBytes("C:/Users/Owner/Desktop/SharedFolder/pokemon.sav"), true);
			Console.WriteLine(string.Join("\n-----\n", from s in sram.Headers
													select s.GetDescription()));

			MD5CryptoServiceProvider md5 = new MD5CryptoServiceProvider();
			byte[] hash = md5.ComputeHash(sram.ToArray());
			foreach (byte b in hash) Console.Write(b.ToString("X2"));
			Console.WriteLine();
			GoombaSRAM sram2 = sram.CopyAndReplace((Stateheader)sram.Headers[0], File.ReadAllBytes("C:/Users/Owner/Desktop/Pokemon Silver (isaac) (Enable clock reset).sav"));
			hash = md5.ComputeHash(sram2.ToArray());
			foreach (byte b in hash) Console.Write(b.ToString("X2"));
			Console.WriteLine();
			File.WriteAllBytes("../../../pokemon2.sav", sram2.ToArray());
		}
	}
}