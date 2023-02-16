cmd_/home/onio/Documents/CN/device_driver/modules.order := {   echo /home/onio/Documents/CN/device_driver/test.ko; :; } | awk '!x[$$0]++' - > /home/onio/Documents/CN/device_driver/modules.order
