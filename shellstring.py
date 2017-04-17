#!/usr/bin/python
from sys import platform, argv, version_info
from binascii import hexlify, unhexlify

class nixColor:
	rst		= str('\033[0m')
	red		= str('\033[0;31m')
	green	= str('\033[0;32m')
	cyan	= str('\033[0;36m')
	yellow	= str('\033[1;33m')
	blue	= str('\033[0;34m')

class winColor:
	from colorama import init, Fore, Style
	rst		= Style.RESET_ALL
	red		= Fore.RED
	green	= Fore.GREEN
	cyan	= Fore.CYAN
	yellow	= Fore.YELLOW
	blue	= Fore.BLUE

color = nixColor if not platform == "win32" else winColor
m = ', '

class c:
	cmt		= staticmethod(lambda s: color.green + unhexlify(s).decode('utf-8') + color.rst)
	num		= staticmethod(lambda s: color.red + str(s) + color.rst)

for a in ["mov", "push", "shl", "shr", "xor", "_or"]: #.or is not valid. Had to use _or
	setattr(c, a, color.cyan + a.replace('_', '') + ' ' + color.rst)

for a in [	"rax", "eax", "ax", "ah", "al", "rbx", "ebx", "bx", "bh", "bl",
			"rcx", "ecx", "cx", "ch", "cl",	"rdx", "edx", "dx", "dh", "dl",
			"r8", "r8d", "r8w", "r8b", "r9", "r9d", "r9w", "r9b"]:
	setattr(c, a, color.green + a.replace('_', '') + color.rst)

def foo(s, bits):
	if bits == 32:
		if len(s) == 2:
			print('\t' + c.mov + c.dl + m + c.num('0x%s' % s) + '\t; ' + c.cmt(s))
		if len(s) == 4:
			print('\t' + c.mov + c.dx + m + c.num('0x%s' % s) + '\t; ' + c.cmt(s))
		if len(s) == 6:
			print('\t' + c.mov + c.dh + m + c.num('0x%s' % s[:2]) + '\t; ' + c.cmt(s[:2]))
			print('\t' + c.shl + c.edx + m + c.num(str(8)))
			print('\t' + c.mov + c.dx + m + c.num('0x%s' % s[2:]) + '\t; ' + c.cmt(s[2:]))
		print('\t' + c.push + c.edx)
	if bits == 64:
		if len(s) == 2:
			print('\t' + c.mov + c.r8b + m + c.num("0x%s" % s) + '\t; ' + c.cmt(s))
		if len(s) == 4:
			print('\t' + c.mov + c.r8w + m + c.num("0x%s" % s) + '\t; ' + c.cmt(s))
		if len(s) == 6:
			print('\t' + c.mov + c.r8b + m + c.num("0x%s" % s[:2]) + '\t; ' + c.cmt(s[:2]))
			print('\t' + c.shl + c.r8 + m + '16')
			print('\t' + c.mov + c.r9w + m + c.num("0x%s" % s[2:]) + '\t; ' + c.cmt(s[2:]))
			print('\t' + c._or + c.r8 + m + c.r9)
		if len(s) == 8:
			print('\t' + c.mov + c.r8d + m + c.num("0x%s" % s) + '\t; ' + c.cmt(s))
		if len(s) == 10:
			print('\t' + c.mov + c.r8b + m + c.num("0x%s" % s[:2]) + '\t; ' + c.cmt(s[:2]))
			print('\t' + c.shl + c.r8 + m + '32')
			print('\t' + c.mov + c.r9d + m + c.num("0x%s" % s[2:]) + '\t; ' + c.cmt(s[2:]))
			print('\t' + c._or + c.r8 + m + c.r9)
		if len(s) == 12:
			print('\t' + c.mov + c.r8w + m + c.num("0x%s" % s[:4]) + '\t; ' + c.cmt(s[:4]))
			print('\t' + c.shl + c.r8 + m + '32')
			print('\t' + c.mov + c.r9d + m + c.num("0x%s" % s[4:]) + '\t; ' + c.cmt(s[4:]))
			print('\t' + c._or + c.r8 + m + c.r9)
		if len(s) == 14:
			print('\t' + c.mov + c.r8b + m + c.num("0x%s" % s[:2]) + '\t; ' + c.cmt(s[:2]))
			print('\t' + c.shl + c.r8 + m + '16')
			print('\t' + c.mov + c.r9w + m + c.num("0x%s" % s[2:6]) + '\t; ' + c.cmt(s[2:6]))
			print('\t' + c._or + c.r8 + m + c.r9)
			print('\t' + c.shl + c.r8 + m + '32')
			print('\t' + c.shl + c.r9 + m + '16')
			print('\t' + c.mov + c.r9d + m + c.num("0x%s" % s[6:]) + '\t; ' + c.cmt(s[6:]))
			print('\t' + c._or + c.r8 + m + c.r9)
		print('\t' + c.push + c.r8)

def main():
	if len(argv) > 1:
		bits = 32
		if len (argv) > 2:
			arch = argv[2]
			if arch in ['x86', '32', 'i386']:
				bits = 32
			elif arch in ['x64', '64']:
				bits = 64
			else:
				print("Defaulting to 32-bit NASM")
		mod = bits/4
		if version_info.major == 2:
			s	= str(hexlify(argv[1].decode("string_escape")))
			s	= ''.join(reversed([s[int(i):int(i+2)] for i in range(int(0), int(len(s)), int(2))]))
		elif version_info.major == 3:
			s	= hexlify(bytes(argv[1], 'utf-8')).decode('utf-8')
			s	= ''.join(reversed([s[int(i):int(i+2)] for i in range(int(0), int(len(s)), int(2))]))
		else:
			raise(StandardException("Version not supported. Seriously, upgrade."))
		size	= len(s)//2
		if bits == 32:
			print('\t' + c.xor + c.edx + ", " + c.edx)
		elif bits == 64:
			print('\t' + c.xor + c.r8 + m + c.r8)
		tmp = len(s) % mod
		if tmp != 0:
			if bits == 64:
				print('\t' + c.xor + c.r9 + m + c.r9)
			s	= str(s)
			a,s = s[:int(tmp)], s[int(tmp):]
			foo(a, bits)
		else:
			if bits == 32:
				print('\t' + c.push + c.edx)
			elif bits == 64:
				print('\t' + c.push + c.r8)
		for i in range(int(0), int(len(s)), int(mod)):
			if bits == 32:
				print('\t' + c.push + c.num('0x%s' % s[int(i):int(i+mod)]) + '\t; ' + c.cmt(s[int(i):int(i+mod)]))
			if bits == 64:
				print('\t' + c.mov + c.r8 + m + c.num("0x%s" % s[int(i):int(i+mod)]) + '\t; ' + c.cmt(s[int(i):int(i+mod)]))
		print("\nSize: %i (%s)" % (size, hex(size)))
	else:
		print('Usage:\n\t%s "String To Reverse" [32/64]' % argv[0])

if __name__=="__main__":
	main()
