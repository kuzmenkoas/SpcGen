# SpcGen

## Description
SpcGen software is used to decode 16-bit values from binary files with digitization results from ADC and processing that data for getting spectrum from detectors.
Currently, it supports two ADC: DRS4 and Digitizer.

Also, this software supports two methods signal processing: by charge and amplitude.

A software configuring is done with two parsers: configuring file (.cfg) and console configuration.

SpcGen software is open-source, source code is published in [GitHub](https://github.com/kuzmenkoas/SpcGen).

## Dependencies
* ROOT
* CMake and Make
* C++17

## Installation Guide
### Linux
Dependencies installation (for Ubuntu/Linux Mint):

`sudo apt install gcc cmake`

Software installation:

```
git clone https://github.com/kuzmenkoas/SpcGen
cd SpcGen
mkdir build
cd build
cmake ..
make
```

### Windows
1. Install Visual Studio
2. Install CMake
3. Download source code SpcGen from github
4. Build project with CMake (use MSVC compiler)
5. Build project with Visual Studion (SpcGen.exe button)
6. Add path to executable file (path/SpcGen/out/build/x64-Debug/) to enviromental variable PATH (Win+R, call sysdm.cpl).

## Start
To start program it's necessery to enter path to executable file (in Linux build/SpcGen), in Windows, if path was added to environment variable PATH, it is possible to call program from any path by entering SpcGen.exe.
There are binary files from DRS4 and Digitizer in path /examples/data, alsi there are examples for configuring files in path /examples/config.
Currently, program divides input data (arguments) into configuration (extension .cfg) and binary (for drs - .dat, for digitizer - .bin), so, transmission argument order do not influence on program (it's possible firstly trans,it configuration file, secondly binary file, or vice verse).
The type of device is defined by binary file extension.
There are additional decoding configuration flags (-cut, -threshold and -debug). Flag -cut starts algorithm cutting invalid signals (comparasion every event with normalized average waveform signal by cross-correlation function in percents). Flag -threshold rejects events, which are lower/higher threshold (set by user). Flag -debug adds plots rejected signals. Thus, start program with flags:
```
SpcGen.exe -cut -debug -threshold /examples/config/drs.cfg /examples/data/drs/08_Co60_ch4_NaI_N5_OC_R1307_1500V_30mV_T20_42_1Gss_500ns_20250618s.dat
```
Flag transmissing order is not important.
To start program with configuration file for drs device you have to enter command:
```
SpcGen.exe /examples/config/drs.cfg /examples/data/drs/08_Co60_ch4_NaI_N5_OC_R1307_1500V_30mV_T20_42_1Gss_500ns_20250618s.dat
```

To use console parser without configuration file, use command:
```
SpcGen.exe /examples/data/drs/08_Co60_ch4_NaI_N5_OC_R1307_1500V_30mV_T20_42_1Gss_500ns_20250618s.dat
```

There are two types binary files for digitizer device - with processed data by it's program "psd" and with waveform signals data "waveform". Thus, there are three ways of decoding binary files:
1. psd
2. waveform
3. psd and waveform
In case at the same time processing psd and waveform it's important transmission order. First argument from binary files must be psd file, next one - waveform.
There are examples of processing only psd file:
```
SpcGen.exe /examples/config/psd.cfg /examples/data/digitizer/raw_data_psd_wavetest_2025_11_06__13_28_09.bin
```
```
SpcGen.exe /examples/data/digitizer/raw_data_psd_wavetest_2025_11_06__13_28_09.bin
```
There are examples of processing only waveform file:
```
SpcGen.exe /examples/config/waveform.cfg /examples/data/digitizer/raw_waveform_psd_wavetest_2025_11_06__13_28_09.bin
```
```
SpcGen.exe /examples/data/digitizer/raw_waveform_psd_wavetest_2025_11_06__13_28_09.bin
```
There are examples of processing both psd and waveform files:
```
SpcGen.exe /examples/config/psdWaveform.cfg /examples/data/digitizer/raw_data_psd_wavetest_2025_11_06__13_28_09.bin /examples/data/digitizer/raw_waveform_psd_wavetest_2025_11_06__13_28_09.bin
```
```
SpcGen.exe /examples/data/digitizer/raw_data_psd_wavetest_2025_11_06__13_28_09.bin /examples/data/digitizer/raw_waveform_psd_wavetest_2025_11_06__13_28_09.bin
```

## Configuration files
Shared headers for both ADC - Output, Amplitude, Charge, Config, Histogram. The headers transmission order is not important. Program is realized such as way, that it is searching needed header and stops, when see empty line. Also, parameters transmission order under the header is not strongly typed.

### Output format
Output format is setted under the header Output. Next by symbol "+" separated by space is written output format. There are two output formats - Root and Txt. Example:
```
Output
+ Root
+ Txt
```

### Amplitude processing method settings
The header for amplitude method is Amplitude. Next, parameters are: up (dowm), factor and shift. Signal type parameter (signal peak is down or up) is configurating algorithm such as way, that searches minimal (maximal) value as base point for fitting. Next, parameter factor - multiplier for amplitude, parameter shift - value of shift. Example:
```
Amplitude
+ up
+ factor 1
+ shift 0
```

### Charge processing method settings
The header charge processing configuration - Charge. There are parameters: factor and shift. Example:
```
Charge
+ factor 1
+ shift 0
```

### Signal processing configuration
Signal processing configuration is setting by Config header. Parameters: cut (percent, lower that CCF will reject events as a bad signal), threshold (threshold for rejecting events), baseline (specifies bin limits, where defines an average noise value), range (specifies bin limits, where integral calculated for charge processing and/or fitting area for amplitude processing). For ADC Digitizer also specifies signal length (wavelength). Example:
```
Config
+ cut 0.5
+ threshold -0.05
+ baseline 0 100
+ range 101 221
+ wavelength 1000
```

### Histograms
Histogram is a header for configuration plotting histograms by data. Next, depending on device type, there is a bit different configurations.
For ADC DRS4 sequence of parameters:
1. parameter;
2. amount of bins;
3. minimal value;
4. maximal value;
For ADC Digitizer sequence:
1. file (PSD or Waveform);
2. parameter;
3. amount of bins;
4. minimal value;
5. maximal value;

Example for ADC DRS4:
```
Histogram
+ charge 1000 -5 70
+ amplitude 1000 -0.05 0.6
```

Example for ADC Digitizer:
```
Histogram
+ PSD qShort 1000 -20000 200000
+ Waveform charge 1000 -20000 200000
```

### Data for processing and decoding
There are different headers for devices.
Let's look for a configuration of ADC DRS4.
The header for configuration parameters, that are necessery to get, is Data. There are next parameters:
1. baseline - average noise value in setted limits;
2. charge - integral value in setted limits;
3. amplitude - amplitude;
4. waveform - a plot of average signal waveform;
5. scaler;
6. time;

Configuration example:
```
Data
+ baseline
+ charge
+ amplitude
+ waveform
+ scaler
+ time
```

Let's consider ADC Digitizer.
Because binary file psd may consist different parameters (may record it, may not), for correct program necessery to specify, which parameters were record. For that in header DataPSD lists recorded parameters.
Example:
```
DataPSD
+ qShort
+ qLong
+ cfd_y1
+ cfd_y2
+ baseline
+ height
+ eventCounter
+ eventCounterPSD
+ psdValue
```

Configuration binary file type waveform is conducted under header DataWaveform. Allowed next parameters:
1. baseline;
2. charge;
3. amplitude;
4. waveform.
Example:
```
DataWaveform
+ baseline
+ charge
+ amplitude
+ waveform
```

## Console configuration
### DRS
The following is the sequential output of the program when running without a configuration file.
Firstly, the output format is choosen, the next message is printed:
```
Choose format to output:
(0) Root
(1) Txt
(2) Root and Txt
```

You must enter the selected option into the console.
After that for ADC DRS will print next parameters for record.
```
Enter parameters to use:
(0) all
(1) baseline
(2) charge
(3) amplitude
(4) waveform
(5) scaler
(6) time
```

You must continuously enter the desired parameters, for example: 135 for baseline, amplitude, and scaler. Or enter 0 to record all values.
After this, a message with the processing configuration will be displayed (windows for calculating the baseline and charge with amplitude).
```
Configure limits for signal processing
Enter min value point for baseline: 0
Enter max value point for baseline: 100
Enter min value point for signal: 101
Enter max value point for signal: 450
```

Next, a text will be displayed with settings for the type of signal (going up or down).

```
Choose which form of signal:
(1) up
(2) down
```

After this, the result will be adjusted (the multiplier and term for charge and amplitude).

```
Enter factor for amplitude: 1
Enter shift for amplitude: 0
```

```
Configure the results of charge algorithm
Enter factor for charge: 1
Enter shift for charge: 0
```

After which the histogram configuration will be displayed; you need to select which ones you need by continuously entering numbers (123):

```
Choose parameters to configure histogram
(0) without
(1) baseline
(2) charge
(3) amplitude
(4) scaler
```

If histograms are selected, the configuration for each parameter will be displayed. You must specify the number of bins, minimum, and maximum values ​​for the histograms.
```
Set number of bins for amplitude: 1000
Set minimal value for amplitude: 0
Set maximal value for amplitude: 10
```

If you run the program with the -threshold flag, an additional question will appear:
```
Enter threshold in mV to cut signal: 0.5
```
Which sets the threshold for the signal (whether to consider it a good event or not, or to discard and disregard this event).

With -cut flag will:
```
Enter percents to cut signal: 1.2
```
Which will discard events if the normalized cross-correlation function of the event shows that the current event does not exceed the "similarity" to the average signal shape by a specified percentage.
That is, the algorithm will calculate the VCF, normalize it to a percentage (from 0 to 100), and if this value is below the percentage specified here, it will be considered a "bad" event and discarded.

### Digitizer
Here is the sequence of configuration output when running the program with two files at the same time: psd and waveform.
First, the output format is also configured:
```
Choose format to output:
(0) Root
(1) Txt
(2) Root and Txt
```

You must enter a number in the selected format.
Next, the parameters stored in the PSD file will be displayed. You must specify all the stored parameters, not just the ones you need! They are specified as a continuous list (for example, 1456).
```
Enter parameters to use:
(0) all
(1) qShort
(2) qLong
(3) cfd_y1
(4) cfd_y2
(5) baseline
(6) height
(7) eventCounter
(8) eventCounterPSD
(9) psdValue
```

Next, the Waveform data configuration will be displayed. You must specify which parameters you want to obtain:
```
Enter parameters to use:
(0) all
(1) baseline
(2) charge
(3) amplitude
```
Specify as a continuous number (12).
After this, you need to configure the processing (set windows for calculating the baseline and charge with amplitude):
```
Configure limits for charge algorithm
Enter min value point for baseline: 0
Enter max value point for baseline: 100
Enter min value point for signal: 101
Enter max value point for signal: 450
```
Next, a text will be displayed with the settings for the type of signal (going up or down)
```
Choose which form of signal:
(1) up
(2) down
```
After this, the result will be adjusted (the multiplier and term for charge and amplitude).
```
Enter factor for amplitude: 1
Enter shift for amplitude: 0
```
```
Configure the results of charge algorithm
Enter factor for charge: 1
Enter shift for charge: 0
```
Next, a histogram configuration will be displayed; you need to select which ones you need by entering consecutive numbers (123).
If histograms are selected, the configuration for each parameter will be displayed. You must specify the number of bins, minimum, and maximum values ​​for the histograms.
For PSD data:
```
Choose parameters to configure histogram
(0) without
PSD configuration:
(1) qShort
(2) qLong
(3) cfd_y1
(4) cfd_y2
(5) baseline
(6) height
(7) eventCounter
(8) eventCounterPSD
(9) psdValue
```

```
Set number of bins for qShort: 1000
Set minimal value for qShort: 0
Set maximal value for qShort: 10000
```
For waveform data:
```
Choose parameters to configure histogram
(0) without
Waveform configuration:
(1) baseline
(2) charge
(3) amplitude
```
```
Set number of bins for baseline: 1000
Set minimal value for baseline: 0
Set maximal value for baseline: 1000
```
If you run a program with a single file, you will need to specify what type it is:
```
Enter format file to decode:
(1) PSD
(2) Waveform
```
If you run the program with the -threshold flag, an additional question will appear:
```
Enter threshold in mV to cut signal: 0.5
```
Which sets the threshold for the signal (whether to consider it a good event or not, or to discard and disregard this event).
With -cut flag will:
```
Enter percents to cut signal: 1.2
```