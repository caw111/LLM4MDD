// AVR 1.8.6's default yield hook is empty and may be overridden by a sketch.
// Link this as an archive member: use it only when the sketch has no definition.
// This also works with MinGW, whose weak-symbol fallback differs from AVR/ELF.
extern "C" void yield() {}
