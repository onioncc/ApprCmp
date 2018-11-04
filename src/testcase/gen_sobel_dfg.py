
import sys

### generate DFG for sobelFilter

def main(argv):
	if len(argv) == 1:
		print "please specify the graph size"
		return


	size = int(argv[1])
	# +2 is for padding
	

	### 1 represents accurate ADD
	### 2 represents accurate MUL
	### 3 represents accurate ROOT


	nodes = []
	edges = []
	prim_in = []
	prim_in_fix = []
	prim_out = []



	nd = 1
	po = 1
	for Row in range(1, size + 1 - 2):
		for Col in range(1, size + 1 - 2):

			### left conv part 
			for i in range(0, 9):
				nodes.append(str(nd + i) + " " + "2")

			### primary inputs
			### variables
			prim_in.append( str( 18 + (Row-1) * size + Col  ) + " " + str(nd))
			prim_in.append( str( 18 + (Row-1) * size + Col+1) + " " + str(nd+1))
			prim_in.append( str( 18 + (Row-1) * size + Col+2) + " " + str(nd+2))
			prim_in.append( str( 18 + (Row  ) * size + Col  ) + " " + str(nd+3))
			prim_in.append( str( 18 + (Row  ) * size + Col+1) + " " + str(nd+4))
			prim_in.append( str( 18 + (Row  ) * size + Col+2) + " " + str(nd+5))
			prim_in.append( str( 18 + (Row+1) * size + Col  ) + " " + str(nd+6))
			prim_in.append( str( 18 + (Row+1) * size + Col+1) + " " + str(nd+7))
			prim_in.append( str( 18 + (Row+1) * size + Col+2) + " " + str(nd+8))

			
			### fixed		
			prim_in_fix.append( "1" + " " + str(nd))
			prim_in_fix.append( "2" + " " + str(nd+1))
			prim_in_fix.append( "3" + " " + str(nd+2))
			prim_in_fix.append( "4" + " " + str(nd+3))
			prim_in_fix.append( "5" + " " + str(nd+4))
			prim_in_fix.append( "6" + " " + str(nd+5))
			prim_in_fix.append( "7" + " " + str(nd+6))
			prim_in_fix.append( "8" + " " + str(nd+7))
			prim_in_fix.append( "9" + " " + str(nd+8))

			
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

			### primary inputs
			### variables
			prim_in.append( str( 18 + (Row-1) * size + Col   ) + " " + str(nd))
			prim_in.append( str( 18 + (Row-1) * size + Col+1 ) + " " + str(nd+1))
			prim_in.append( str( 18 + (Row-1) * size + Col+2 ) + " " + str(nd+2))
			prim_in.append( str( 18 + (Row  ) * size + Col   ) + " " + str(nd+3))
			prim_in.append( str( 18 + (Row  ) * size + Col+1 ) + " " + str(nd+4))
			prim_in.append( str( 18 + (Row  ) * size + Col+2 ) + " " + str(nd+5))
			prim_in.append( str( 18 + (Row+1) * size + Col   ) + " " + str(nd+6))
			prim_in.append( str( 18 + (Row+1) * size + Col+1 ) + " " + str(nd+7))
			prim_in.append( str( 18 + (Row+1) * size + Col+2 ) + " " + str(nd+8))
			
			### fixed		
			prim_in_fix.append( "10" + " " + str(nd))
			prim_in_fix.append( "11" + " " + str(nd+1))
			prim_in_fix.append( "12" + " " + str(nd+2))
			prim_in_fix.append( "13" + " " + str(nd+3))
			prim_in_fix.append( "14" + " " + str(nd+4))
			prim_in_fix.append( "15" + " " + str(nd+5))
			prim_in_fix.append( "16" + " " + str(nd+6))
			prim_in_fix.append( "17" + " " + str(nd+7))
			prim_in_fix.append( "18" + " " + str(nd+8))


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

			### primary out
			prim_out.append(str(nd) + " " + str(po))
			po = po + 1

			nd = nd + 1
	
	#print nodes
	#print edges

	print prim_in
	print prim_out

	
	file_name = 'sobel_' + str(size) + "x" + str(size) + '.dat'
	fo = open(file_name, 'w')

	### write nodes
	fo.write("** Nodes\n")
	for nd in nodes:
		fo.write(str(nd)+"\n")
	fo.write("-1\n\n")

	### write edges
	fo.write("** Edges\n")
	for ed in edges:
		fo.write(str(ed)+"\n")
	fo.write("-1\n\n")

	### write primary inputs 
	fo.write("** Primary Inputs \n")
	fo.write("** PI_id ---> Node_id\n")
	fo.write("** Fixed values\n")
	for pi in prim_in_fix:
		fo.write(str(pi)+"\n")
	fo.write("-1\n\n")

	fo.write("** Variables\n")
	for pi in prim_in:
		fo.write(str(pi)+"\n")
	fo.write("-1\n\n")

	### write primary outputs
	fo.write("** Primary Outputs\n")
	fo.write("** Node_id ---> PO_id\n")
	for po in prim_out:
		fo.write(str(po)+"\n")
	fo.write("-1\n\n")


	print file_name + " is generated for image size " + str(size) + "x" + str(size)


if __name__ == "__main__":
	main(sys.argv)
