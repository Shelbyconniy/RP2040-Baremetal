import sys
import struct

def crc32(data: bytes) -> int:
    
    reg = 0xFFFFFFFF
    pol = 0x04C11DB7

    for byte in data:

        reg ^= (byte << 24) & 0xFFFFFFFF 

        for _ in range(8):
            if (reg & 0x80000000):
                reg = ((reg << 1) ^ pol) & 0xFFFFFFFF
            else:
                reg = (reg << 1) & 0xFFFFFFFF
    
    return reg

def main():
    if len(sys.argv) < 2:
        print("Usage: crc32_rp2040.py [path-to-file]")
        sys.exit(1)
    
    path = sys.argv[1]

    try:
        with open(path, "r+b") as f:
            data = f.read(252)

            if len(data) < 252:
                print("File is too short")
                sys.exit(1)
            
            checksum = crc32(data)
            print(f"Checksum: 0x{checksum:08X}")

            f.seek(252)
            f.write(struct.pack('<I', checksum))

            f.flush()
            print("CRC32 was written")
    except FileNotFoundError:
        print("Invalid path")
        sys.exit(1)
    except IOError as e:
        print(f"IO error: {e}")
        sys.exit(1)

if __name__ == '__main__':
    main()

