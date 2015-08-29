#! /usr/bin/python


import argparse
import png
import os

def createAssetCode(args):
	hxxFileName = getOutputFileName(args, "hxx")
	hxxFile = open(hxxFileName, 'w')
	cxxFileName = getOutputFileName(args, "cxx")
	cxxFile = open(cxxFileName, 'w')

	includeGuard = getIncludeGuard(hxxFile)
	writeHxxPrologue(hxxFile, args, includeGuard)
	writeCxxPrologue(cxxFile, args, os.path.basename(hxxFile.name))

	for image in args.images:
		processImage(image, hxxFile, cxxFile, args)

	writeHxxEpilogue(hxxFile, includeGuard)
	hxxFile.close()
	writeCxxEpilogue(cxxFile)
	cxxFile.close()



def getOutputFileName(args, extension):
	outFileName = args.namespace + "." + extension
	if args.output:
		outFileName = args.output + "." + extension

	return outFileName


def getIncludeGuard(hxxFile):
	baseName = os.path.basename(hxxFile.name)
	includeGuard = baseName.upper().replace('-', '_').replace('+', '_').replace(' ', '_').replace('.', '_')
	return "__" + includeGuard + "__"


def writeHxxPrologue(hxxFile, args, includeGuard):
	hxxFile.write("// This file is auto-generated, please don't edit it.\n")
	hxxFile.write("\n")
	hxxFile.write("#ifndef " + includeGuard + "\n")
	hxxFile.write("#define " + includeGuard + "\n")
	hxxFile.write("\n")
	hxxFile.write("#include \"stddef.h\"\n")
	hxxFile.write("\n")
	hxxFile.write("namespace " + args.namespace + "\n")
	hxxFile.write("{\n")


def writeCxxPrologue(cxxFile, args, hxxFileBaseName):
	cxxFile.write("// This file is auto-generated, please don't edit it.\n")
	cxxFile.write("\n")
	cxxFile.write("#include \"" + hxxFileBaseName + "\"")
	cxxFile.write("\n")
	cxxFile.write("\n")


def processImage(image, hxxFile, cxxFile, args):
	imageSymbolRoot = getImageSymbolRoot(image)
	imageData = getImageData(image)
	writeHeaderEntry(imageData, imageSymbolRoot, hxxFile)
	writeDataEntry(imageData, imageSymbolRoot, cxxFile, args)


def getImageSymbolRoot(image):
	imageSymbolRoot = os.path.basename(image.name).replace('-', '').replace('+', '').replace(' ', '').replace('.', '')
	return imageSymbolRoot


def getImageData(image):
	imageReader = png.Reader(image)
	imageData = imageReader.read_flat()
	return imageData


def writeHeaderEntry(imageData, imageSymbolRoot, hxxFile):
	hxxFile.write("\textern const char* " + imageSymbolRoot + "Data;\n")
	hxxFile.write("\tconst unsigned int " + imageSymbolRoot + "DataSize = " + str(len(imageData[2]))  + ";\n")
	hxxFile.write("\tconst unsigned int " + imageSymbolRoot + "Width = " + str(imageData[0]) + ";\n")
	hxxFile.write("\tconst unsigned int " + imageSymbolRoot + "Height = " + str(imageData[1]) + ";\n\n")


def writeDataEntry(imageData, imageSymbolRoot, cxxFile, args):
	symbolName = args.namespace + "::" + imageSymbolRoot + "Data"
	tempSymbolName = "temp_" + imageSymbolRoot + "Data"
	cxxFile.write("\tstatic const unsigned char " + tempSymbolName +"[] = \n\t{\n")
	for y in range(imageData[1]):
		cxxFile.write("\t\t")
		for x in range(imageData[0]):
			index = y * imageData[0] + x
			cxxFile.write(str(imageData[2][index]))
			if index < (imageData[0] * imageData[1] - 1):
				cxxFile.write(", ")
		cxxFile.write("\n")
	cxxFile.write("\t};\n")
	cxxFile.write("\tconst char* " + symbolName + " = (const char*)" + tempSymbolName + ";\n\n")



def writeHxxEpilogue(hxxFile, includeGuard):
	hxxFile.write("}\n")
	hxxFile.write("#endif // " + includeGuard + "\n")


def writeCxxEpilogue(cxxFile):
	cxxFile.write("\n")





def main():
	parser = argparse.ArgumentParser()
	parser.add_argument("namespace", type=str, help='The namespace to output the images into')
	parser.add_argument("images", metavar='I', type=file, nargs='+', help='The images to process in to source')
	parser.add_argument("--output", type=str, help='The base name of the cxx/hxx files to output')
	args = parser.parse_args()
	createAssetCode(args)



main()
