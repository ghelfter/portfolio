;;;; *********************************************************
;;;;                       Galen Helfter
;;;;                         DPA 8150
;;;;                         Project 1
;;;;                      composite.lisp
;;;; *********************************************************

;;; Contains all of the package information
(defpackage :composite
  (:use :common-lisp)
  (:export #:image
           #:access-pixel
           #:access-pixels-sdim
           #:get-brightness
           #:image-size
           #:image-dim
           #:clear-channel
           #:width
           #:height
           #:channels
           #:data
           #:invert
           #:brightness-inplace
           #:brightness
           #:simple-contrast
           #:gamma-correct
           #:monochrome
           #:apply-matte
           #:add-images
           #:sub-images
           #:mix-image
           #:keymix
           #:over
           #:convolve-3
           #:median-3
           #:color-difference-blue
           #:color-difference-blue-matte
           #:read-image
           #:write-image
           #:comp-main
           #:to-hex-string
           #:encode-64-image
           #:bytes-to-base-64-str
           #:ppm-encode
           #:to-char-buffer))
