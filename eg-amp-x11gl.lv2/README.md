Still in work...<br/>
First compile pugl<br/>
```
git clone https://gitlab.com/lv2/pugl.git
cd pugl && meson setup build
cd build && meson compile
```

Then compile the application:<br/>
Have a look:<br/>
https://stackoverflow.com/questions/49138195/whats-the-difference-between-rpath-link-and-l

Compile<g/>
```
make
```

Install<g/>
```
make install
```

Run<g/>
```
jalv.gtk http://lv2plug.in/plugins/eg-amp-x11gl
```