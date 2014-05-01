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
		const String TITLE = "Goomba Save Manager";

		private GoombaSRAM loaded_sram;
		private String filePath;
		private bool dirty;

		public Form1(String filename) {
			InitializeComponent();

			// Update status of Save and Save As items whenever File menu is opened
			fileToolStripMenuItem.DropDownOpening += (o, e) => {
				saveToolStripMenuItem.Enabled = (filePath != null && dirty);
				saveAsToolStripMenuItem.Enabled = (filePath != null);
			};
			this.Closing += (o, e) => {
				e.Cancel = !okToClose();
			};
			listBox1.SelectedIndexChanged += listBox1_SelectedIndexChanged;

			filePath = null;

			/*this->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &goombasav_clr::MainForm::OnDragEnter);
			this->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &goombasav_clr::MainForm::OnDragDrop);
			this->AllowDrop = true;*/

			if (filename != null) {
				load(filename);
			}

			/*GoombaSRAM sram = new GoombaSRAM(File.ReadAllBytes("C:/Users/Owner/Desktop/SharedFolder/pokemon.sav"), true);
			Console.WriteLine(string.Join("\n-----\n", from s in sram.Headers
													select s.GetDescription()));

			MD5CryptoServiceProvider md5 = new MD5CryptoServiceProvider();
			byte[] mh = md5.ComputeHash(sram.ToArray());
			foreach (byte b in mh) Console.Write(b.ToString("X2"));
			Console.WriteLine();
			GoombaSRAM sram2 = sram.CopyAndReplace((Stateheader)sram.Headers[0], File.ReadAllBytes("C:/Users/Owner/Desktop/Pokemon Silver (isaac) (Enable clock reset).sav"));
			mh = md5.ComputeHash(sram2.ToArray());
			foreach (byte b in mh) Console.Write(b.ToString("X2"));
			Console.WriteLine();
			File.WriteAllBytes("../../../pokemon2.sav", sram2.ToArray());

			*/
		}

		private void openToolStripMenuItem_Click(object sender, EventArgs e) {

		}

		private void saveToolStripMenuItem_Click(object sender, EventArgs e) {

		}

		private void saveAsToolStripMenuItem_Click(object sender, EventArgs e) {

		}

		private void exitToolStripMenuItem_Click(object sender, EventArgs e) {

		}

		private void aboutToolStripMenuItem_Click(object sender, EventArgs e) {

		}

		private void btnReplace_Click(object sender, EventArgs e) {

		}

		private void btnExtract_Click(object sender, EventArgs e) {

		}

		private void listBox1_SelectedIndexChanged(object sender, EventArgs e) {
			GoombaHeader h = (GoombaHeader)listBox1.SelectedItem;
			lblSizeVal.Text = h.Size + " bytes";
			lblTypeVal.Text = h.Type == Stateheader.STATESAVE ? "Savestate"
				: h.Type == Stateheader.SRAMSAVE ? "SRAM"
				: h.Type == Stateheader.CONFIGSAVE ? "Config"
				: "Unknown";
			if (h is Stateheader) {
				Stateheader sh = (Stateheader)h;
				flpConfigdata.Visible = false;
				flpStateheader.Visible = true;
				lblUncompressedSize.Text =
					sh.DataSize >= sh.Size
					? "Uncompressed size:"
					: "Compressed size:";
				lblUncompressedSizeVal.Text = sh.DataSize + " bytes";
				lblFramecountVal.Text = sh.Framecount.ToString();
				lblChecksumVal.Text = sh.ROMChecksum.ToString("X8");

				panel1.Visible = true;
				uint hash = sh.CompressedDataHash();
				lblHashVal.Text = hash.ToString("X6");
				hashBox.BackColor = Color.FromArgb((int)(hash | 0xFF000000));
			}
			lblTitleVal.Text = h.Title;
		}

		private bool okToClose() {
			if (filePath != null && dirty) {
				DialogResult dr = MessageBox.Show("Save changes to " + Path.GetFileName(filePath) + "?",
					TITLE, MessageBoxButtons.YesNoCancel);
				if (dr == DialogResult.Yes) {
					save(filePath);
				} else if (dr == DialogResult.Cancel) {
					return false;
				}
			}
			return true;
		}

		private void load(String filename) {
			if (!okToClose()) return;
			byte[] arr = System.IO.File.ReadAllBytes(filename);
			if (arr.Length > GoombaSRAM.ExpectedSize) {
				MessageBox.Show("This file is more than " + GoombaSRAM.ExpectedSize +
					" bytes. If you overwrite the file, the last " + (arr.Length - GoombaSRAM.ExpectedSize) +
					" bytes will be discarded.", "Note", MessageBoxButtons.OK, MessageBoxIcon.Information);
			}
			try {
				loaded_sram = new GoombaSRAM(arr, true);
			} catch (GoombaException e) {
				MessageBox.Show(e.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
				return;
			}
			
			filePath = filename;
			this.Text = (filename == null)
				? TITLE
				: TITLE + " - " + Path.GetFileName(filename);

			headerScan();
		}

		private void save(String path) {
			byte[] arr = loaded_sram.ToArray();
			File.WriteAllBytes(path, arr);

			filePath = path;
			dirty = false;
			this.Text = TITLE + " - " + Path.GetFileName(path);
		}

		private void headerScan() {
			listBox1.Items.Clear();
			//resetDescriptionPanel();
			listBox1.Items.AddRange(loaded_sram.Headers.ToArray());
			if (loaded_sram.Headers.Count == 0) {
				MessageBox.Show("No headers were found in this file. It may not be valid SRAM data", "Note",
					MessageBoxButtons.OK,
					MessageBoxIcon.Information);
			}
		}
	}
}
