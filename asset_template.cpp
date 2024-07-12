#include <{{target}}/f2_assets.hpp>

extern char data_start[];
extern char data_end[];

asm(
  ".section \".rodata\", \"a\", @progbits\n"
  "data_start:\n"
    ".incbin \"{{path}}\"\n"
  "data_end:\n"
    ".previous\n"
);

const char* f2::asset<"{{name}}">::data{ data_start };
const size_t f2::asset<"{{name}}">::size{ static_cast<size_t>(data_end - data_start) };
