#!/usr/bin/env python3
"""Convert binary SPIR-V .spv files to C hex array format for #include."""

import sys
import pathlib

def convert_spv_to_hex(input_path, output_path):
    """Read binary SPV and write as C uint32_t array."""
    with open(input_path, 'rb') as f:
        data = f.read()

    # SPIR-V is uint32_t array
    if len(data) % 4 != 0:
        raise ValueError(f"SPV file size {len(data)} is not multiple of 4")

    with open(output_path, 'w') as f:
        # Write as comma-separated hex values
        for i in range(0, len(data), 4):
            uint32_val = int.from_bytes(data[i:i+4], byteorder='little')
            f.write(f"0x{uint32_val:08x},")
            if (i // 4 + 1) % 8 == 0:
                f.write("\n")
            else:
                f.write(" ")
        f.write("\n")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <input.spv> <output.spv>")
        sys.exit(1)

    convert_spv_to_hex(sys.argv[1], sys.argv[2])
    print(f"Converted {sys.argv[1]} -> {sys.argv[2]}")
