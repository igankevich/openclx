(use-package-modules
  base build-tools check
  cmake commencement gcc
  ninja opencl pkg-config)

(use-modules
  (guix packages)
  (guix git-download))

(define-public pocl-1.3
  (package
    (inherit pocl)
    (name "pocl")
    (version "1.3")
    (source
     (origin
       (method git-fetch)
       (uri (git-reference
             (url "https://github.com/pocl/pocl.git")
             (commit (string-append "v" version))))
       (sha256
        (base32 "176qcnc4jp1xq6s6mv4aa0r84pldka54bl042xgz433s029a04hw"))
       (file-name (git-file-name name version))))))

(packages->manifest
  (append
    (list
      pocl-1.3 ;; for unit tests
      opencl-headers
      ocl-icd
      (list gcc-9 "lib")
      gcc-toolchain-9
      pkg-config
      ninja
      meson
      googletest)))
