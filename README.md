# Deepin Repair Tools
> __A useful tools for repair & clean your Deepin OS.__

### Project Goals
* [x] Grub repair
* [x] DPKG wrong status repair
* [x] Password reset
* [x] APT cache clear

### Build from source
#### Requirements
* Qt5 with components: Core, Widget, Concurrent
* libdtkwidget2-dev
* cmake 3.8.6+
* pkg-config

```
git clone git@github.com:sbwtw/deepin-repair-tools.git
cd deepin-repair-tools
cmake ..
make -j4
sudo make install
```

### LICENSE
Deepin Repair Tools is licensed under GPL