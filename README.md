# tutos.ui.lv2
Some examples of projects on the programming of graphical interfaces of LV2 plugins with robtk toolkit<br/>
Most of the code comes from those of Robin Gareus, the creator of robtk:<br/>
https://github.com/x42<br/>

The first, "eg-amp", is taken from the LV2 distribution:<br/>
I only change the gain amplitude to 24dB<br/>
https://github.com/lv2/lv2/tree/master/plugins/eg-amp.lv2<br/>

Reading references<br/>
https://lv2plug.in/book/<br/>

## eg-amp.lv2<br/>
Makefile only works for linux.<br/>
This plugin does not have its own UI, the plugin host provides access to gain settings.<br/>

Carla:<br/>
![alt text](https://github.com/lherg/tutos.ui.lv2/blob/main/png/eg-amp-carla.png)<br/>

Jalv:<br/>
![alt text](https://github.com/lherg/tutos.ui.lv2/blob/main/png/eg-amp-jalv.png)<br/>

## eg-amp-gtk.lv2<br/>
(Makefile only works for linux)<br/>
Adding a gtk2 UI.<br/>
![alt text](https://github.com/lherg/tutos.ui.lv2/blob/main/png/eg-amp-gtk2.png)<br/>

## eg-amp-gtk3.lv2<br/>
(Makefile only works for linux)<br/>
Adding a gtk3 UI.<br/>
![alt text](https://github.com/lherg/tutos.ui.lv2/blob/main/png/eg-amp-gtk3.png)<br/>

## eg-amp-robtk.lv2<br/>
Adding a robtk UI.<br/>
Simplification of  Robin Gareus matrixmixer plugins to have only one input output:<br/>
https://github.com/x42/matrixmixer.lv2 <br/>
the Makefile is adapted from that of matrixmixer.<br/>
![alt text](https://github.com/lherg/tutos.ui.lv2/blob/main/png/eg-amp-robtk.png)<br/>

## eg-amp-fgain.lv2<br/>
Creating graduation for the knob: eg-amp-robtk.lv2 <br/>
![alt text](https://github.com/lherg/tutos.ui.lv2/blob/main/png/eg-amp-fgain.png)<br/>

## eg-amp-fgain-ano.lv2<br/>
Creating annotation for the knob to display level: eg-amp-fgain.lv2 <br/>
![alt text](https://github.com/lherg/tutos.ui.lv2/blob/main/png/eg-amp-fgain-ano.png)<br/>