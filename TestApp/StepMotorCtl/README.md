## build
```
mkdir build && cd build
camke .. && make -j8
```

## use
```
Usage: ./StepMotorCtl [node_id] [command] [value(s)]
Commands:
  enable [0|1]
  calibrate
  set_current [float]
  set_vel [float]
  set_pos [float]
  set_pos_time [pos(float)] [time(float)]
  set_pos_vel_limit [pos(float)] [vel_limit(float)]
  get_current
  get_vel
  get_pos
  get_home_offset
  erase_configs
  reboot
```

## config can for linux start

create a file /etc/systemd/network/80-can.network with the following content:

```
[Match]
Name=can*

[CAN]
BitRate=500K
```
Instead of "can*" you can also specify a specific interface (e.g. "can0").

Then (re-)start the systemd-networkd service:
```
sudo systemctl restart systemd-networkd
```
To have it start automatically when booting:
```
sudo systemctl enable systemd-networkd
```
