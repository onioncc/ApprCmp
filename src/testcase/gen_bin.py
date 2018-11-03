import sys
import numpy as np
import struct

data1 = [0.2876, 0.7082, -0.1792, 1.2081, -1.8271, 0.7167, 0.4221]
data2 = [0.1875, -1.2712, -1.3989, -0.4811, 1.0065, 0.2407, -1.3566]
data3 = [-0.3127, -1.2663, -0.6327, 0.7857, 1.1984, 1.2021, -0.3519]

s = struct.pack('f'*len(data1), *data1)
f = open('test.bin', 'wb')
f.write(s)

s = struct.pack('f'*len(data2), *data2)
f = open('test.bin', 'ab')
f.write(s)

s = struct.pack('f'*len(data3), *data3)
f = open('test.bin', 'ab')
f.write(s)

f.close()


with open('test.bin', 'rb') as f:
	d1 = struct.unpack('f' * 7, f.read(4 * 7))
	print d1

	f.seek(4 * 7)
	d2 = struct.unpack('f' * 7, f.read(4 * 7))
	print d2

	f.seek(4 * 7 * 2)
	d3 = struct.unpack('f' * 7, f.read(4 * 7))
	print d3