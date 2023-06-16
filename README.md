# i3status

## Description

This fork of i3status has a brightness module.
It works by reading the brightness files in /sys/backlight.

![Alt text](/images/brightness.png "Brightness module demo")


## Usage

Simply add `order += brightness` to your i3status config file.

You may need to configure the paths to the files that store brightness data:

```
brightness {
    "actual_brightness_path", "/sys/class/backlight/intel_backlight/actual_brightness"
    "max_brightness_path", "/sys/class/backlight/intel_backlight/max_brightness"
}
```

You may also wish to edit the format options

``` 
brightness {
    format = " 󰖨 %brightness_bar [%brightness_percentage] (%actual_brightness/%max_brightness) "
    format_down = "UNKNOWN BRIGHTNESS"
}
```

## Installation 

Dependencies: meson and ninja 
```
git clone https://github.com/david-mcpherson/i3status
cd i3status
mkdir build
meson setup ..
ninja
sudo ninja install
```
This installs to `/usr/local/bin/i3status`.

You can now delete the cloned i3status repository.

You'll need to tell i3 to use this forked status program.

Include something like this in your i3 config:

```
bar {
    i3bar_command i3bar -t
    status_command /usr/local/bin/i3status
}
```

