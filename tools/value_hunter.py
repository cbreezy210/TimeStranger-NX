#!/usr/bin/env python3
"""
Value Hunter: Read-only save dump diffing tool.
Finds candidate u32 little-endian offsets matching a known delta between two dumps.
"""
import sys
import struct

def find_u32_le_candidates(dump1: bytes, dump2: bytes, expected_delta: int) -> list[tuple[int, int, int]]:
    """
    Scans two byte dumps for u32 LE values that changed by exactly `expected_delta`.
    Returns a list of tuples: (offset, val1, val2)
    """
    candidates = []
    min_len = min(len(dump1), len(dump2))
    
    # Step by 4 bytes for u32
    for offset in range(0, min_len - 3, 4):
        val1 = struct.unpack_from('<I', dump1, offset)[0]
        val2 = struct.unpack_from('<I', dump2, offset)[0]
        
        if abs(val2 - val1) == expected_delta:
            candidates.append((offset, val1, val2))
            
    return candidates

def main():
    if len(sys.argv) != 4:
        print("Usage: python value_hunter.py <dump1.bin> <dump2.bin> <expected_delta>")
        sys.exit(1)
        
    file1, file2, delta_str = sys.argv[1], sys.argv[2], sys.argv[3]
    
    try:
        expected_delta = int(delta_str)
    except ValueError:
        print("Error: Delta must be an integer.")
        sys.exit(1)

    print(f"[*] Reading {file1} and {file2}...")
    with open(file1, 'rb') as f1, open(file2, 'rb') as f2:
        dump1 = f1.read()
        dump2 = f2.read()

    print(f"[*] Scanning for u32 LE changes of exactly {expected_delta}...")
    candidates = find_u32_le_candidates(dump1, dump2, expected_delta)
    
    print(f"[+] Found {len(candidates)} candidate offset(s):")
    for offset, val1, val2 in candidates:
        print(f"    0x{offset:08X} : {val1} -> {val2} (Delta: {val2 - val1:+d})")

if __name__ == "__main__":
    main()
