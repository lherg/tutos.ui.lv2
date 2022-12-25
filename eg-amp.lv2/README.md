# eg-amp.lv2<br/>
This plugin does not have its own UI, the plugin host provides access to gain settings.<br/>
![alt text](https://github.com/lherg/tutos.ui.lv2/blob/main/png/eg-amp-jalv.png)<br/>

"eg-amp", is taken from the LV2 distribution:<br/>
I only change the gain amplitude to 24dB<br/>
https://github.com/lv2/lv2/tree/master/plugins/eg-amp.lv2<br/>

Reading references<br/>
https://lv2plug.in/book/<br/>

Makefile only works for linux.<br/>

Compile<br/>
```
make
```
Install<br/>
```
make install
```

Run<br/>
```
jalv.gtk http://lv2plug.in/plugins/eg-amp
```