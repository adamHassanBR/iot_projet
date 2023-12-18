sshpass -p 'pipi' ssh pi@connectedmiror << EOF
DISPLAY=:0 xrandr --output HDMI-1 --mode 1920x1080
EOF