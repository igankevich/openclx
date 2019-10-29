(use-package-modules
  base
  build-tools
  check
  cmake
  commencement
  gcc
  gl
  maths
  ninja
  opencl
  pkg-config)

(use-modules
  (stables packages opencascade)
  (stables packages nvidia)
  (stables packages mesa))

(define (has-nvidia-gpu?) (file-exists? "/dev/nvidia0"))
(define (has-dri-gpu?) (file-exists? "/dev/dri/card0"))

(define (has-nvidia-driver version)
  (use-modules (rnrs io ports)
	       (ice-9 rdelim))
  (string-contains (read-line (open-input-file "/proc/driver/nvidia/version")) version))

(packages->manifest
  (append
    (cond
      ((has-nvidia-gpu?) (list nvidia-driver))
      ((has-dri-gpu?) (list mesa-opencl-icd-absolute))
      (#t (list pocl-1.3)))
    (list
      opencl-headers
      opencl-clhpp
      ocl-icd
      (list gcc-9 "lib")
      gcc-toolchain-9
      pkg-config
      ninja
      meson
      googletest)))
