# i3-wm-layout-indicator
Layout indicator for i3-wm

# How to install
Build with: <br />
`gcc layout_file.c -lX11 -lxkbfile -o layout_file` <br />
And put it somewhere (e.g. `.local/bin` or `/usr/local/bin`)

Add lines to your i3status config from the example config

Add this to your i3 config: <br />
`exec --no-startup-id /path/to/program/layout_file` <br />
To start the program at startup

# How it looks
![a](https://user-images.githubusercontent.com/25824718/120046584-2ad23000-c01b-11eb-83a6-15593da9be87.png)
