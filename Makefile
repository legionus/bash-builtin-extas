all:
	$(CC) -O2 -g -fPIC -DPIC -shared -o timing timing.c -D_GNU_SOURCE -DHAVE_CONFIG_H -I/usr/include/bash
