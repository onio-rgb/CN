cmd_/home/onio/Documents/CN/device_driver/test.mod := printf '%s\n'   test.o | awk '!x[$$0]++ { print("/home/onio/Documents/CN/device_driver/"$$0) }' > /home/onio/Documents/CN/device_driver/test.mod
