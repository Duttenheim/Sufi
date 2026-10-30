sufi_target(memallocsample SUFI_EXECUTABLE)
sufi_files(memallocsample.cc)
sufi_add_dependencies(SUFI_TRANSIENT sufi)

sufi_target(arraysample SUFI_EXECUTABLE)
sufi_files(arraysample.cc)
sufi_add_dependencies(SUFI_TRANSIENT sufi)