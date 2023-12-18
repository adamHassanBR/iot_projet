sshpass -p 'pipi' ssh pi@connectedmiror << EOF
DISPLAY=:0 xrandr --output HDMI-1 --off
EOF