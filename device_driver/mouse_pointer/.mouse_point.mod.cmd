cmd_/home/onio/Documents/CN/device_driver/mouse_pointer/mouse_point.mod := printf '%s\n'   mouse_point.o | awk '!x[$$0]++ { print("/home/onio/Documents/CN/device_driver/mouse_pointer/"$$0) }' > /home/onio/Documents/CN/device_driver/mouse_pointer/mouse_point.mod