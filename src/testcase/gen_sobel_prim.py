import sys
import struct
import random


# specify the number of primary inputs and input groups
size = 12
prim_in = size*size
group = 1000

# specify the testbench name
name = "sobel_" + str(size) + "x" + str(size)

# specify the primary input data range
data_min = -2
data_max = 2


######## fixed primary input values
weights = [1, 0, -1, 2, 0, -2, 1, 0, -1, 1, 2, 1, 0, 0, 0, -1, -2, -1]




def main():
	data = []
	print "Generating input data as " + name + ".bin"
	print "Number of inputs: %d, number of groups: %d, data range (%d, %d)" % (prim_in, group, data_min, data_max)

	for g in range(0, group):
		for i in range(0, prim_in):
			data.append( random.random() * (data_max - data_min) + data_min )

	#print len(data)

	f = open(name + '.bin', 'wb')

	### first write fixed primary input values
	s = struct.pack('f'*len(weights), *weights)
	f.write(s)

	### then write variable primary input values
	s = struct.pack('f'*len(data), *data)
	f.write(s)

	f.close()

  
if __name__== "__main__":
	main()

