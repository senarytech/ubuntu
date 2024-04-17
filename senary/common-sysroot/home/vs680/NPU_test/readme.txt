0:

sudo ifconfig eth0 up
sudo dhclient eth0


1:
./synap_cli --nb yolov6s_int16.nb --meta yolov6s_int16.json  -r 10 random

2:
./synap_cli_od --nb yolov6s_int16.nb --meta yolov6s_int16.json  ./bus_.jpg

3:

export LD_LIBRARY_PATH=/opt/syna/lib/:$LD_LIBRARY_PATH

./synap_cli_yolov6  --nb ./yolov6s_int16.nb --meta ./yolov6s_int16.json  ./bus_.jpg



