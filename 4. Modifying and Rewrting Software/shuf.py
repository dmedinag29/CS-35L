import argparse
import random
import sys

class shuf:
	def __init__(self, filename, input_range, head_count, repeat):
		self.filename = filename
		self.input_range = input_range
		self.head_count = head_count
		self.repeat = repeat

		if (self.input_range is not None) and (self.filename is not None):
			sys.exit("Error: We cannot have both a file and an input range!")

	def shuffle_out(self):
		if (self.filename) and (self.filename != "-"):
			self.line_list = []
			f = open(self.filename, 'r')
			self.line_list = f.readlines()
			f.close()

		elif (self.input_range is not None):
			self.line_list = []
			list_range = self.input_range.split("-")
			if (len(list_range) > 2):
				sys.exit("Error: This range is invalid, should contain only upper and lower bound!")
			if ((int(list_range[0]) < 0) or (int(list_range[1]) < int(list_range[0])) or (int(list_range[1]) < 0)):
				sys.exit("Error: This range is invalid")
			self.size = int(list_range[1]) - int(list_range[0]) + 1
			for i in range(int(list_range[0]), int(list_range[1])+1):
				self.line_list.append(str(i) + "\n")
			#self.line_list = list(range(int(list_range[0]), int(list_range[1]) + 1))
		else:
			self.line_list = sys.stdin.readlines()

		if self.repeat:
			size = len(self.line_list)
			self.shuffledl = random.choices(self.line_list, k=size)
			if self.head_count is None:
				hold = 0
				while hold < 1:
					self.shuffledl = random.choices(self.line_list, k=size)
					print(self.shuffledl, sep = '', end = '')
			else:
				hold = 0
				while hold < self.head_count:
					self.shuffledl = random.choices(self.line_list, k=size)
					print(self.shuffledl[0], sep = '', end = '')
					hold += 1
		else:
			size = len(self.line_list)
			self.shuffledl = random.sample(self.line_list, k=size)

		if self.head_count is not None and self.repeat is not True:
			size = len(self.line_list)
			hold = 0
			while hold < self.head_count:
				print(self.shuffledl[hold], sep = '', end = '')
				hold += 1
		elif (self.repeat is not True):
			print(self.shuffledl, sep = '', end = '')


def main():
	parser = argparse.ArgumentParser()
	parser.add_argument("file", nargs="?")
	parser.add_argument("-n","--head-count", type=int, help="Will only output certain head-count of lines")
	parser.add_argument("-i","--input-range", type=str, help='Will output numbers in input-range')
	parser.add_argument("-r","--repeat", action="store_true", help="Will allows repetition, and will run indefinitely unless used with headcount")
	args = parser.parse_args()

	try:
		generator = shuf(args.file, args.input_range, args.head_count, args.repeat)
	except Exception as e:
		parser.error(e)

	generator.shuffle_out()

if __name__ == '__main__':
	main()
