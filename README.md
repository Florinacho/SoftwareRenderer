# SoftwareRenderer
3D Rasterizer that outputs in the linux devfb

If /dev/fb is not enabled:
1. uncomment the GRUB_GFXMODE=640x480 entry in /etc/default/grub
2. call sudo update-grub
3. call sudo apt-get install v86d
4. call sudo modprobe uvesafb
