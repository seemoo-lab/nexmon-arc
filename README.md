# Nexmon for ARC
The nexmon C-based firmware patching framework adapted for the ARC architecture, that enables you to write your own firmware patches for ARC based WiFi chips such as the QCA9500.

## Getting Started
The following explained how to use nexmon-arc and compile our hello world application for the wil6210 firmware v4.1.0.55, which prints a debug output that is readable from the device driver. The following instructions have been tested with Ubuntu 16.04.03.

### Setting up the environment
* Installing dependencies (mostly for the ARC toolchain):

  ```bash
  sudo apt-get install texinfo byacc flex libncurses5-dev zlib1g-dev libexpat1-dev texlive build-essential git wget bison gawk libgmp3-dev
  ```

* Run `make` in the root directory, this will download and compile the ARC toolchain.
* Download the original FW file (version 4.1.0.55) from the [linux-firmware repository](https://git.kernel.org/pub/scm/linux/kernel/git/firmware/linux-firmware.git/plain/wil6210.fw) and place it in this directory: `firmwares/wil6210/4-1-0_55/`

### Patch the firmware
* Setup the build environment for Nexmon:

  ```bash
  source setup_env.sh
  ```

* Go to the hello world example in the patches directory and execute `make`. This will build a patched firmware which prints a string at the initialization time of the fw and uc code. 

  ```bash
  cd patches/wil6210/4-1-0_55/hello_world
  make
  ```
* Copy the resulting `wil6210.fw` to your device (the default place in the filesystem is `/lib/firmware/wil6210.fw`)
* Restart the interface (executed on the device):

  ```bash
  ifconfig wlan2 down && ifconfig wlan2 up
  ```
* You should be able to get the following results by reading the `console_fw` and `console_uc` files in the debugfs:

  ```bash
  root@TALON1:~# cat /sys/kernel/debug/ieee80211/phy2/wil6210/console_dump_fw 
  FW: INITIALIZED
  root@TALON1:~# cat /sys/kernel/debug/ieee80211/phy2/wil6210/console_dump_uc 
  UC: INITIALIZED
  ```

  The `console_fw` and `console_uc` debugfs interfaces are custom extensions of the wil6210 driver that allow to read the output buffer from the firmware. This function is integrated in our [lede-ad7200](https://github.com/seemoo-lab/lede-ad7200) image for TP-Link Talon AD7200 devices and provided by [this](https://github.com/seemoo-lab/lede-ad7200/blob/release/overlay/seemoo/mac80211/patches/0002-adding-support-to-read-console-output.patch) patch. 

### Write your own patch
  To write your own patches, check the example files in *patches/wil6210/4-1-0_55/hello_world/src* and consider the original [Nexmon](https://nexmon.org) project for further documentation on the patching process.

## DISCLAIMER
This software might damage your hardware and may void your hardware’s warranty. You use our tools at your risk and responsibility.

You acknowledge that you are solely responsible for how you use the software, & for complying with all relevant laws in your area. 
You also acknowledge that neither of the developers of nexmon-arc nor any of its affiliates & associates may be held liable, 
responsible or accountable for any type of damage, litigation or other legal action, which may arise either from your legal or 
illegal use of nexmon-arc, or any other software. We do not tolerate the use of our software for any illegal purpose. 
By using our software in any way, you acknowledge & approve to use it exclusively in a lawful manner.

## Statistics
Nexmon is mainly intended as a research project that we share with the community so that others can benefit from our tools.
We want to collect statistics to figure out how often Nexmon is used in general and which platform and firmware version is the most popular.
We also intent to share our findings in the future. For further information please check the original [Nexmon](https://nexmon.org) project page.

### What kind of statistics do you collect?

Every time you run a Nexmon firmware build, we collect the following information:
* A unique identifier based on a random number (e.g., 5O31UY9Z5IEX3O9KL680V5IHNASIE1SB)
* The name, release, machine and processor of your build system (`uname -srmp`, e.g., `Linux 4.2.0-42-generic x86_64 x86_64`)
* Git internal path to the built project (e.g., `patches/wil6210/4-1-0_55/hello_world/`)
* Git version (e.g., `2.2.1-55-g3684a80c-dirty`)
* Git repository URL (e.g., `git@github.com:seemoo-lab/nexmon-arc.git`)

### How do I disable the collection of statistics?

If you have privacy concerns, we also offer to opt-out of the statistic collections. To this end, you simply have to create a `DISABLE_STATISTICS` file in your Nexmon root directory.

## Talon Tools
This software has been released as part of [Talon Tools: The Framework for Practical IEEE 802.11ad Research](https://seemoo.de/talon-tools/). Any use of it, which results in an academic publication or other publication which includes a bibliography is encouraged to appreciate this work and include a citation the Talon Tools project and any of our papers. You can find all references on Talon Tools in our [bibtex file](https://seemoo-lab.github.io/talon-tools/talon-tools.bib). Please also check the [project page](https://seemoo.de/talon-tools/) for supplemental tools.

## Give us Feedback
We want to learn how people use our platform and what aspects we might improve. Please report any issues or comments using the bug-tracker and do not hesitate to approach us via e-mail.

## Contact
* [Daniel Steinmetzer](https://seemoo.tu-darmstadt.de/dsteinmetzer) <<dsteinmetzer@seemoo.tu-darmstadt.de>>
* Daniel Wegemer <<dwegemer@seemoo.tu-darmstadt.de>>
* [Matthias Schulz](https://seemoo.tu-darmstadt.de/mschulz) <<mschulz@seemoo.tu-darmstadt.de>>

## Powered By
<a href="https://www.seemoo.tu-darmstadt.de">![SEEMOO logo](logos/seemoo.png)</a> &nbsp;
<a href="https://www.nicer.tu-darmstadt.de">![NICER logo](logos/nicer.png)</a> &nbsp;
<a href="https://www.crossing.tu-darmstadt.de">![CROSSING logo](logos/crossing.jpg)</a>&nbsp;
<a href="https://www.crisp-da.de">![CRSIP logo](logos/crisp.jpg)</a>&nbsp;
<a href="http://www.maki.tu-darmstadt.de/">![MAKI logo](logos/maki.png)</a> &nbsp;
<a href="https://www.cysec.tu-darmstadt.de">![CYSEC logo](logos/cysec.jpg)</a>&nbsp;
<a href="https://www.tu-darmstadt.de/index.en.jsp">![TU Darmstadt logo](logos/tudarmstadt.png)</a>&nbsp;
