openocd \
    -f interface/stlink.cfg \
    -f target/stm32l4x.cfg \
    -c "program build/Debug/unflappy.elf verify reset exit"
