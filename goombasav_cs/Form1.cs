using Goombasav;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace goombasav_cs {
	public partial class Form1 : Form {
		public Form1() {
			InitializeComponent();

			byte[] b = File.ReadAllBytes("../../../Debug/pokemon.sav");
			GoombaSRAM sram = new GoombaSRAM(b);
			Console.WriteLine(string.Join(", ", sram.FindHeaders()));
		}
	}
}
