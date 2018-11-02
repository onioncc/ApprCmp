
import sys

### generate DFG for sobelFilter

def main(argv):
	if len(argv) == 1:
		print "please specify the graph size"
		return

	size = int(argv[1]) * int(argv[1]) + 1
	

	### 1 represents accurate ADD
	### 2 represents accurate MUL
	### 3 represents accurate ROOT


	nodes = []
	edges = []

	nd = 1
	for pixel in range(1, size):

		### left conv part 
		for i in range(0, 9):
			nodes.append(str(nd + i) + " " + "2")
		
		### addition tree
		nodes.append(str(nd +  9) + " " + "1")
		edges.append(str(nd     ) + " " + str(nd + 9))
		edges.append(str(nd +  1) + " " + str(nd + 9))
		
		nodes.append(str(nd + 10) + " " + "1")
		edges.append(str(nd +  2) + " " + str(nd + 10))
		edges.append(str(nd +  3) + " " + str(nd + 10))

		nodes.append(str(nd + 11) + " " + "1")
		edges.append(str(nd +  4) + " " + str(nd + 11))
		edges.append(str(nd +  5) + " " + str(nd + 11))

		nodes.append(str(nd + 12) + " " + "1")
		edges.append(str(nd +  6) + " " + str(nd + 12))
		edges.append(str(nd +  7) + " " + str(nd + 12))

		nodes.append(str(nd + 13) + " " + "1")
		edges.append(str(nd +  9) + " " + str(nd + 13))
		edges.append(str(nd + 10) + " " + str(nd + 13))

		nodes.append(str(nd + 14) + " " + "1")
		edges.append(str(nd + 11) + " " + str(nd + 14))
		edges.append(str(nd + 12) + " " + str(nd + 14))

		nodes.append(str(nd + 15) + " " + "1")
		edges.append(str(nd +  8) + " " + str(nd + 15))
		edges.append(str(nd + 14) + " " + str(nd + 15))

		nodes.append(str(nd + 16) + " " + "1")
		edges.append(str(nd + 13) + " " + str(nd + 16))
		edges.append(str(nd + 15) + " " + str(nd + 16))

		left_add = nd + 16
		nd = nd + 17

		### right conv part 
		for i in range(0, 9):
			nodes.append(str(nd + i) + " " + "2")
		
		### addition tree
		nodes.append(str(nd +  9) + " " + "1")
		edges.append(str(nd     ) + " " + str(nd + 9))
		edges.append(str(nd +  1) + " " + str(nd + 9))
		
		nodes.append(str(nd + 10) + " " + "1")
		edges.append(str(nd +  2) + " " + str(nd + 10))
		edges.append(str(nd +  3) + " " + str(nd + 10))

		nodes.append(str(nd + 11) + " " + "1")
		edges.append(str(nd +  4) + " " + str(nd + 11))
		edges.append(str(nd +  5) + " " + str(nd + 11))

		nodes.append(str(nd + 12) + " " + "1")
		edges.append(str(nd +  6) + " " + str(nd + 12))
		edges.append(str(nd +  7) + " " + str(nd + 12))

		nodes.append(str(nd + 13) + " " + "1")
		edges.append(str(nd +  9) + " " + str(nd + 13))
		edges.append(str(nd + 10) + " " + str(nd + 13))

		nodes.append(str(nd + 14) + " " + "1")
		edges.append(str(nd + 11) + " " + str(nd + 14))
		edges.append(str(nd + 12) + " " + str(nd + 14))

		nodes.append(str(nd + 15) + " " + "1")
		edges.append(str(nd +  8) + " " + str(nd + 15))
		edges.append(str(nd + 14) + " " + str(nd + 15))

		nodes.append(str(nd + 16) + " " + "1")
		edges.append(str(nd + 13) + " " + str(nd + 16))
		edges.append(str(nd + 15) + " " + str(nd + 16))

		right_add = nd + 16
		nd = nd + 17

		
		### square part
		nodes.append(str(nd) + " " + "2")
		edges.append(str(left_add) + " " + str(nd))
		edges.append(str(left_add) + " " + str(nd))
		
		nd = nd + 1
		nodes.append(str(nd) + " " + "2")
		edges.append(str(right_add) + " " + str(nd))
		edges.append(str(right_add) + " " + str(nd))

		nd = nd + 1
		### addition and root
		nodes.append(str(nd) + " " + "1")
		edges.append(str(nd - 2) + " " + str(nd))
		edges.append(str(nd - 1) + " " + str(nd))
		nd = nd + 1
		nodes.append(str(nd) + " " + "3")
		edges.append(str(nd - 1) + " " + str(nd))

		nd = nd + 1
	
	#print nodes
	#print edges

	
	file_name = 'sobel_' + argv[1] + "x" + argv[1] + '.dat'
	fo = open(file_name, 'w')
	for nd in nodes:
		fo.write(str(nd)+"\n")
	fo.write("-1\n\n")

	for ed in edges:
		fo.write(str(ed)+"\n")
	fo.write("-1\n\n")

	print file_name + " is generated for image size " + argv[1] + "x" + argv[1]


if __name__ == "__main__":
	main(sys.argv)
