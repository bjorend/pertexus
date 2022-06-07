#!/usr/bin/env python3

import sys
import csv
import argparse

def main(argv):
    parser = argparse.ArgumentParser(
        description='Generate a GPIO definition table')
    parser.add_argument('ifile', metavar='input-file',
                        type=argparse.FileType('r'),
                        help='CSV input file')
    parseargs = parser.parse_args()

    csvreader = csv.DictReader(parseargs.ifile, dialect='excel')

    allvals = [ item for item in csvreader if item['My Design'] != '' ]

    namelen = 4*((max([len(item['My Design']) for item in allvals])+3)//4)

    for item in allvals:
        pertecport = 0
        pertecpin = 0

        try:
            try:
                gpioset = int(item['GPIO Group'])
                gpiocard = int(item['Number'])
                teensyconn = item['Connector']
                teensypin = int(item['Pin #'])
                arduinopin = int(item['Arduino Pin'])
                isgpio = True
            except:
                exppin = int(item['Pin #'])
                gpioset = exppin // 8
                gpiocard = exppin % 8
                teensyconn = ''
                teensypin = 0
                arduinopin = -1
                isgpio = False

            # Figure out the group number
            if item['PertecGroup'].startswith('IN'):
                # It's a controller input (group A)
                groupnum = 0 if isgpio else 1
            elif item['PertecGroup'].startswith('OUT'):
                # It's a controller output (group B)
                groupnum = 2 if isgpio else 3
            elif item['PertecGroup'].startswith('UDF'):
                # It's more complicated (group C)
                groupnum = 4 if isgpio else 5
            else:
                groupnum = 6

            if item['PertecPin'].startswith('P'):
                portpin = item['PertecPin'][1:].split(':')
                pertecport = int(portpin[0])
                pertecpin = int(portpin[1])
            comment = None
            if item['Pinname (Pad)'] != '':
                comment = item['Pinname (Pad)']
            elif not isgpio:
                comment = 'IOExpander:{}'.format(item['Pin #'])
                
            print(
   '#define GP_{:{}s} ({:d}, {:d}, {:2d}, {:2d}, {:>4s}, {:2d}, {:d}, {:2d})'
                  .format(item['My Design'], namelen,
                          groupnum, gpioset, gpiocard, arduinopin,
                          ('"' + teensyconn + '"') if teensyconn != ''
                          else 'NULL', teensypin,
                          pertecport, pertecpin)
                  + ('' if comment is None else ('     /* ' + comment + ' */')))
        except:
            print('Died on record: {}'.format(item))
            raise
            
                

if __name__ == '__main__':
    main(sys.argv)

