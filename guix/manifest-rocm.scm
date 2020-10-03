(packages->manifest
  (list
    ;;(@ (gnu packages rocm) roc-opencl-runtime)
    (@ (gnu packages opencl) ocl-icd)
    (@ (gnu packages opencl) opencl-headers)
    (list (@ (gnu packages gcc) gcc) "lib")
    (@ (gnu packages commencement) gcc-toolchain)
    (@ (gnu packages pkg-config) pkg-config)
    (@ (gnu packages check) googletest)
    (@ (gnu packages ninja) ninja)
    (@ (gnu packages build-tools) meson)))
