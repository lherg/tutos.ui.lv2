## eg-amp-robtk.lv2<br/>
A copie of eg-amp from  lv2 distribution.<br/>
Adding a robtk UI.<br/>
Simplification of  Robin Gareus matrixmixer plugins to have only one input output:<br/>
https://github.com/x42/matrixmixer.lv2 <br/>
the Makefile is adapted from that of matrixmixer.<br/>
![alt text](https://github.com/lherg/tutos.ui.lv2/blob/main/png/eg-amp-robtk.png)<br/>

First clone robtk<bg/>
```
git clone https://github.com/x42/robtk.git
```

Compile<bg/>
```
make
```

Install<bg/>
```
make install
```

Run<bg/>
```
jalv.gtk http://github.com/lherg/lv2/eg-amp-robtk
```