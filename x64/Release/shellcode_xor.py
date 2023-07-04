##!/usr/bin/python3
# -*- coding:utf-8 -*-
# author(cn):大剑
# author(en):cl4ym0re

import argparse


def bannerPrint():
    banner = r'''
██╗  ██╗ ██████╗ ██████╗ 
╚██╗██╔╝██╔═══██╗██╔══██╗
 ╚███╔╝ ██║   ██║██████╔╝
 ██╔██╗ ██║   ██║██╔══██╗
██╔╝ ██╗╚██████╔╝██║  ██║
╚═╝  ╚═╝ ╚═════╝ ╚═╝  ╚═╝
    why not xor it                                  
    '''
    print(banner)

def xor(num):
    key = num;
    input_file = "payload.bin"
    output_file = "favicon.lco"

    with open(input_file, "rb") as f:
        payload = f.read()
        encrypted_payload = bytearray(len(payload))

    for i in range(len(payload)):
        encrypted_payload[i] = payload[i] ^ num

    with open(output_file, "wb") as f:
        f.write(encrypted_payload)
    print("[*]Saved as favicon.lco")
if __name__ == "__main__":
    bannerPrint()
    parser = argparse.ArgumentParser()
    parser.add_argument("-n","--num",help="Xor key",type=int)
    args = parser.parse_args()
    if args.num != None:
        xor(args.num)
    else:
        print("[-]XOR key is not provided.")
        print("[?]usage: python3 shellcode_xor.py -n 99")