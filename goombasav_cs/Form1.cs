using Goombasav;
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

			GoombaSRAM sram = new GoombaSRAM(File.ReadAllBytes("../../../Debug/pokemon.sav"));
			Console.WriteLine(string.Join(", ", sram.Headers));

			MD5CryptoServiceProvider md5 = new MD5CryptoServiceProvider();
			byte[] hash = md5.ComputeHash(sram.ToArray());
			foreach (byte b in hash) Console.Write(b.ToString("X2"));
			Console.WriteLine();
			sram.Replace(sram.Headers[0], File.ReadAllBytes("C:/Users/Owner/Desktop/Pokemon Silver (isaac) (Enable clock reset).sav"));
			hash = md5.ComputeHash(sram.ToArray());
			foreach (byte b in hash) Console.Write(b.ToString("X2"));
			Console.WriteLine();
			File.WriteAllBytes("../../../pokemon2.sav", sram.ToArray());
		}
	}
}
