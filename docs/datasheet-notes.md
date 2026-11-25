# BCM2711 datasheet notes

Source: "BCM2711 ARM Peripherals" (Raspberry Pi Ltd).

## address map figure 1 page 6
| View | GPIO base |
| --- | ---|  
| Legacy bus address (datasheet) | 0x7E200000 |
| ARM physical, low peripheral mode | 0xFE200000 |

why different: the BCM2711 has several bus masters and each one sees the peripheral through its own address mapping. The datasheet uses the legacy bus view, where peripherals start at 0x7C00000. The ARM CPU in Low peripheral mode sees the same block starting at 0xFC000000. The offset inside the block is the same (GPIO = base + 0x02200000), so 0x7E200000(bus) = 0xFE200000 (ARM physical). 

## Registers (offset from GPIO base)
| Register | Offset | Type | Use for GPIO17 |
|---|---|---|---|
| GPFSEL1 | 0x04 | RW | FSEL17 = bits 23:21 (table 65 page 67) |
| GPSET0 | 0x1C | WO | bit 17 (table 70) |
| GPCLR0 |0x28 | WO | bit 17 (table 72) |
| GPLEV0 | 0x34 | RO | bit 17 = current level |

## Function select codes (Table 65)
000 input · 001 output · 100/101/110/111/011/010 = ALT0..ALT5 (not sequential)
Reset value 0 → all pins are inputs at power-on.

## Notes
- GPFSEL: read-modify-write (one register holds 10 pins)
- GPSET/GPCLR: write-only, writing 0 has no effect → no read-modify-write needed
- Set/clear is latched while pin is input → clear first, then switch to output
- GPIO17 alt functions: SD9, DPI_D13, RTS0, SPI1_CE1_N, RTS1
- GPIO17 = BCM numbering; physical header pin 11

