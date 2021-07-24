gcc -Ignu-efi-dir/inc -fpic -ffreestanding -fno-stack-protector -fno-stack-check -fshort-wchar -mno-red-zone -maccumulate-outgoing-args -c main.c -o main.o

ld main.o                              /home/a/Documents/os/gnu-efi-dir/x86_64/gnuefi/crt0-efi-x86_64.o          -nostdlib                           -znocombreloc                       -T /home/a/Documents/os/gnu-efi-dir/gnuefi/elf_x86_64_efi.lds      -shared                             -Bsymbolic                          -L /home/a/Documents/os/gnu-efi-dir/x86_64/lib                    -l:libgnuefi.a                      -l:libefi.a                         -o main.so

objcopy -j .text                \
          -j .sdata               \
          -j .data                \
          -j .dynamic             \
          -j .dynsym              \
          -j .rel                 \
          -j .rela                \
          -j .reloc               \
          --target=efi-app-x86_64 \
          main.so                 \
          BOOTX64.EFI

dd if=/dev/zero of=fat.img bs=1k count=1440
mformat -i fat.img -f 1440 ::
mmd -i fat.img ::/EFI
mmd -i fat.img ::/EFI/BOOT
mcopy -i fat.img BOOTX64.EFI ::/EFI/BOOT
mv fat.img ./iso/
xorriso -as mkisofs -R -f -e fat.img -no-emul-boot -o cdimage.iso iso
qemu-system-x86_64 -pflash OVMF.fd -cdrom cdimage.iso -net none
