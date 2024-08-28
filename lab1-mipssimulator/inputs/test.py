import sys


file_name = sys.argv[1]

f = open(file_name, "r")
contents = f.read()

f1 = open("test1.txt", "a")
f1.write(contents)
