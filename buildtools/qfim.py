#!/usr/bin/env python3

import argparse
import binascii
import os
import struct
import math


def image_read(filename):
	if os.path.exists(filename):
		with open(filename, 'rb') as f:
			data = f.read()
		return bytearray(data)
	print('Error: Unable to open file ', filename)
	exit(0)


def image_write(data, filename):
	with open(filename, 'wb') as f:
		f.write(data)


# Determines the offsets of records in data
def get_record_offsets(data):
	offset = 0,
	while offset[-1] < len(data):
		r = struct.unpack('H H I', data[offset[-1]:(offset[-1] + 8)])
		offset += (offset[-1] + r[2] + 8),
	if not offset[-1] == len(data):
		print('Error, malformatted firmware image')
		exit(0)
	return offset[0:-1]


def recordtype_to_str(r):
	if r == 1:
		return 'COMM'
	if r == 2:
		return 'DATA'
	if r == 3:
		return 'FILL'
	if r == 4:
		return 'ACTN'
	if r == 5:
		return 'VERI'
	if r == 6:
		return 'HEAD'
	if r == 7:
		return 'DWRI'
	if r == 8:
		return 'GDAT'
	if r == 9:
		return 'GDA4'
	return None


def get_record_head(data, roffsets, n):
	ro = roffsets[n]
	s = struct.unpack('H H I', data[ro:(ro + 8)])
	rstr = recordtype_to_str(s[0])
	if not rstr:
		print('Error: malformatted firmware image, unknown record type')
		exit(0)

	str = ' %2d: 0x%06x %1d %4s %04x %6d '\
		% (n, ro, s[0], rstr, s[1], s[2])

	# Append comment by Partition Type
	if s[0] == 2:
		raddr = struct.unpack('I', data[(ro + 8):(ro + 12)])[0]
		str += 'Write [%06x : %06x]' % (raddr, raddr + s[2] - 4)
	elif s[0] == 6:
		cmt = struct.unpack('4s I I I I 32s', data[(ro + 8):(ro + 60)])
		if not cmt[0] == b'0126':
			print('Error: malformatted firmware image, magic failed')
		str += 'CRC: 0x%08x V: %1d Len: %6d %s' %\
			(cmt[2], cmt[3], cmt[4], cmt[5])
	return str


def print_record_table(data, roffsets):
	print('  R  Offset   Type   Flgs Size   Contents')
	for n in range(0, len(roffsets)):
		print(get_record_head(data, roffsets, n))


def update_headerlength(data):
	# Obtain the length of the data array
	l_data = len(data)

	# Obtain the length of all data records
	l_records = 0
	ro = get_record_offsets(data)
	for n in range(0, len(ro)):
		s = struct.unpack('I', data[(ro[n] + 4):(ro[n] + 8)])
		l_records += s[0] + 8

	if l_data == l_records:
		# Data size matches, write to header field
		data[24:28] = struct.pack('I', l_data)
	else:
		print('Error: malformatted firmware image, wrong length')
		exit(0)


def update_crc(data, crc_new=None):
	# Read the old CRC value
	# crc_old = struct.unpack('I', data[16:20])[0]

	if crc_new is None:
		# Compute a new CRC value
		# Set crc to zero (temporary) and calculate crc
		data[16:20] = struct.pack('I', 0)
		crc_new = binascii.crc32(data)

	# print('Updating CRC: 0x%08x -> 0x%08x' % (crc_old, crc_new))

	# Update the CRC field
	data[16:20] = struct.pack('I', crc_new)


def cmd_set_data(imgdata, payload, ro):

	s = struct.unpack('H H I I', imgdata[ro:(ro + 12)])

	if not s[0] == 2:
		print('Error, setting data requires a data type record')
		exit(0)

	# Overwrite the Payload
	imgdata[(ro + 12):(ro + s[2] + 8)] = payload

	# Update the Record Length
	imgdata[(ro + 4):(ro + 8)] = struct.pack('I', len(payload) + 4)


def cmd_get_data(imgdata, ro):

	s = struct.unpack('H H I I', imgdata[ro:(ro + 12)])

	if not s[0] == 2:
		print('Error, getting data requires a data type record')
		exit(0)

	# Overwrite the Payload
	return imgdata[(ro + 12):(ro + s[2] + 8)]


def cmd_print_dwrite_data(data, ro):
	h = struct.unpack('H H I', imgdata[ro:(ro + 8)])
	entries = list()
	r = ro + 8

	while r < (ro + 8 + h[2]):
		entry = struct.unpack('I I I', imgdata[r:(r + 12)])
		entries.append(entry)
		r += 12

	print('ADDR\tVALUE\tMASK')
	for e in entries:
		str = '0x%08x\t0x%08x\t0x%08x\t' % (e[0], e[1], e[2])

		nmask = ~e[2] & 0xffffffff
		rmb = int(math.log2(nmask ^ (nmask & (nmask - 1))) + 1)

		value = (e[1] & nmask) >> (rmb - 1)

		blen = int(math.log2((nmask >> (rmb - 1)) + 1))

		str += '%2dBit Value: %8x (%d)\t' % (blen, value, value)
		print(str)


def cmd_get_fw_capabilities(data, ro):
	# Look for a comment record
	for r in ro:
		h = struct.unpack('H H I', imgdata[r:(r + 8)])
		if h[0] == 1:
			magic = struct.unpack('I', imgdata[(r + 8):(r + 12)])
			if magic[0] == 0xabcddcba:

				# Found a capabilities entry
				cap = struct.unpack('I', imgdata[(r + 12):(r + 16)])
				return cap
	return 0


def cmd_set_fw_capabilities(data, ro, capabilities):
	# Look if a capabiltiies record is already present
	for r in ro:
		h = struct.unpack('H H I', imgdata[r:(r + 8)])
		if h[0] == 1:
			magic = struct.unpack('I', imgdata[(r + 8):(r + 12)])
			if magic[0] == 0xabcddcba:
				# Found record, just change the capabilites
				cap = struct.unpack('I', imgdata[(r + 12):(r + 16)])[0]
				imgdata[(r + 12):(r + 16)] = struct.pack('I', int(capabilities) | cap)
				return

	print('Adding new reccord')
	print(capabilities)
	# If not append a new capability record
	record = bytearray(
		b'\x01\x00\x00\x00\x08\x00\x00\x00\xba\xdc\xcd\xab\x00\x00\x00\x00')
	record[12:16] = struct.pack('I', int(capabilities))
	print(len(record))
	data += record
	return


# def cmd_get(fwfile, ofile, record, rawdump):
# 	if record is None:
# 		print('No record id specified')
# 		exit(0)
# 	data = image_read(fwfile)
# 	ro = get_record_offset(data, record)

# 	# Extract the record data
# 	rdata = data[ro[-2]:ro[-1]]

# 	print('Extracting record %d [%08x:%08x]' % (record, ro[-2], ro[-1]))

# 	if rawdump:
# 		# Dump the raw record data
# 		image_write(rdata, ofile)

# 	else:
# 		# Parse only the payload of the file
# 		rr = struct.unpack('H H I', rdata[0:8])
# 		rtype = rr[0]
# 		# rsize = rr[2]

# 		if rtype == 2:
# 			# Parse data record
# 			rr = struct.unpack('I', rdata[8:12])
# 			raddress = rr[0]
# 			rpayload = rdata[12:len(rdata)]

# 			print('Destination address: 0x%08x' % raddress)

# 			image_write(rpayload, ofile)

# 		else:
# 			# Parse different record
# 			print('not supported yet, use rawdump flag instead')
# 			exit(0)


# def cmd_set(fwfile, ofile, record, rtype, rawdump, raddr):
# 	if record is None:
# 		print('No record id specified')
# 		exit(0)
# 	data = image_read(fwfile)
# 	ro = get_record_offset(data, record)

# 	# Load the record data
# 	rpayload = image_read(ofile)

# 	print('Overwriting record %d [%08x:%08x]' % (record, ro[-2], ro[-1]))

# 	if rawdump:
# 		# Directly overwrite the data
# 		data[ro[-2]:ro[-1]] = rpayload

# 	else:
# 		# The object file just containts payload, add header arround
# 		if rtype == 2:
# 			rsize = len(rpayload) + 4
# 			rheader = struct.pack('H H I I', rtype, 0, rsize, raddr)
# 		else:
# 			print('not supported yet, use rawdump flag instead')
# 			exit(0)

# 		data[ro[-2]:ro[-1]] = rheader + rpayload

# 	update_headerlength(data)
# 	update_crc(data)

# 	image_write(data, fwfile)


# def cmd_delete(fwfile, record):
# 	if record is None:
# 		print('No record id specified')
# 		exit(0)
# 	data = image_read(fwfile)
# 	ro = get_record_offset(data, record)

# 	# Overwrite record with empty byte array
# 	data[ro[-2]:ro[-1]] = bytearray(0)

# 	update_headerlength(data)
# 	update_crc(data)

# 	print('Removing record %d [%08x:%08x]' % (record, ro[-2], ro[-1]))
# 	image_write(data, fwfile)


# def cmd_insert(fwfile, ofile, record):
# 	if record is None:
# 		print('No record id specified')
# 		exit(0)
# 	data = image_read(fwfile)
# 	ro = get_record_offset(data, record)

# 	# Load the record data
# 	rdata = image_read(ofile)

# 	data[ro[-2]:ro[-2]] = rdata

# 	update_headerlength(data)
# 	update_crc(data)

# 	print('Inserting record %d [%08x:%08x]' % (record, ro[-2], ro[-1]))

# 	image_write(data, fwfile)


# def cmd_crc(fwfile, crc):
# 	data = image_read(fwfile)
# 	update_crc(data, crc)
# 	image_write(data, fwfile)


if __name__ == "__main__":
	parser = argparse.ArgumentParser(
		prog='QFIM',
		description='QCA9500 Firmware Image Manager \
		changes records in wil6210 firmware files.')

	parser.add_argument('cmd', nargs='*', type=str, default=['list'])
	parser.add_argument('--filein', default='wil6210.fw')
	parser.add_argument('--fileout', default='wil6210_repacked.fw')
	parser.add_argument('-r', '--recordid', type=int)
	parser.add_argument('-d', '--datafile', type=str, default='wildata')
	parser.add_argument('--dsize', type=int, default='-1')
	parser.add_argument('-c', '--capabilities', default=0)
	args = parser.parse_args()
	# print(args)

	fw_write = False

	# Read the firmware image
	imgdata = image_read(args.filein)

	# Obtain the record offsets
	records = get_record_offsets(imgdata)

	if args.cmd[0] == 'list':
		print_record_table(imgdata, records)

	elif args.cmd[0] == 'set':
		if args.cmd[1] == 'data':
			# SET DATA COMMAND
			payload = image_read(args.datafile)
			if args.dsize is not -1:
				if len(payload) > args.dsize:
					print('Error, data too big')
					exit(0)
				else:
					padding = bytearray(args.dsize - len(payload))
					payload += padding
			cmd_set_data(imgdata, payload, records[args.recordid])
			fw_write = True

		elif args.cmd[1] == 'capabilities':
			cmd_set_fw_capabilities(imgdata, records, args.capabilities)
			fw_write = True

	elif args.cmd[0] == 'get':
		if args.cmd[1] == 'data':
			# GET DATA COMMAND
			payload = cmd_get_data(imgdata, records[args.recordid])
			image_write(payload, args.datafile)

		elif args.cmd[1] == 'dwrite':
			# Get dwrite Data
			cmd_print_dwrite_data(imgdata, records[args.recordid])

		elif args.cmd[1] == 'capabilities':
			cap = cmd_get_fw_capabilities(imgdata, records)
			print('0x%04x' % cap)

	# Write the firmware image to file, if necessary
	if fw_write:
		update_headerlength(imgdata)
		update_crc(imgdata)
		image_write(imgdata, args.fileout)
