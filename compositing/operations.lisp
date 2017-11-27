;;;; *********************************************************
;;;;                       Galen Helfter
;;;;                         DPA 8150
;;;;                         Project 1
;;;;                      operations.lisp
;;;; *********************************************************

;;; Package definition
;;(defpackage :composite
;;  (:use :common-lisp))
(in-package :composite)

;;; Generics
(defgeneric premult (img out)
  (:documentation "Premultiplies an image by the alpha channel"))

(defgeneric invert (img out)
  (:documentation "Given an input image, inverts it and stores in the
                   output image, assuming the output image already has the
                   memory needed."))

(defgeneric brightness-inplace (img val)
  (:documentation "Scales the image by a given brightness value. Does not
                   alter the alpha channel."))

(defgeneric brightness (img out val)
  (:documentation "Scales the image by a given brightness value. Does not
                   alter the alpha channel."))

(defgeneric simple-contrast (img out)
  (:documentation "Implements a simple contrast operation, storing the reuslt
                   in the output variable."))

(defgeneric contrast (img out val)
  (:documentation "Implements a sigmoid contrast operation, storing the
                   result in the output image."))

(defgeneric gamma-correct (img out gamma)
  (:documentation "Provides a gamma correction operation, storing the result
                   in the given output image variable."))

(defgeneric monochrome (img out)
  (:documentation "Converts the given image to a monochrome image."))

(defgeneric mix-image (img-1 img-2 out mv)
  (:documentation "Given a mix value and two images, it mixes between them,
                   weighted by the mix value."))

(defgeneric over (img-1 img-2 out)
  (:documentation "Given a set of premultiplied images, it applies the inverse
                   of the alpha channel of the first image to the second."))

(defgeneric convolve-3 (img out filter)
  (:documentation "Given an image and a 3x3 filter, it convolves the
                   image and stores it destructively in the out image."))

(defgeneric median-3 (img out)
  (:documentation "With a 3x3 median filter, it performs a median
                   operation."))

(defgeneric color-difference-blue (img out)
  (:documentation "Uses the color difference operation to create a matte from
                   a blue background. Only the first stage."))

(defgeneric color-difference-blue-matte (img out)
  (:documentation "Second stage of the color difference operation, actually
                   creates the matte."))

(defgeneric chroma-key (img out hue)
  (:documentation "Performs a chroma keying operation, given the hue provided.
                   It does nothing fancier at the moment."))

;; Parameter definitions
(defparameter *kernel-blur* (the (simple-array single-float)
                                 (make-array 9
                                   :element-type 'single-float
                                   :initial-contents
                                   '( 0.11111111f0 0.11111111f0
                                      0.11111111f0 0.11111111f0
                                      0.11111111f0 0.11111111f0
                                      0.11111111f0 0.11111111f0
                                      0.11111111f0))))

(defparameter *kernel-sharpen* (the (simple-array single-float)
                                    (make-array 9
                                      :element-type 'single-float
                                      :initial-contents
                                      '( -1f0 -1f0 -1f0 -1f0 9f0
                                         -1f0 -1f0 -1f0 -1f0))))

(defparameter *kernel-edge* (the (simple-array single-float)
                              (make-array 9
                                :element-type 'single-float
                                :initial-contents
                                '( -1f0 -1f0 -1f0 -1f0 8f0
                                   -1f0 -1f0 -1f0 -1f0))))

(defparameter *hue-eps* 0.003f0)
(defparameter *sat-eps* 0.003f0)
(defparameter *val-eps* 0.003f0)

;; Abstracted the operations out to a generalized macro
(defmacro operation (img out ch i &body body)
  `(loop for ,i of-type fixnum from 0 to (- (image-dim ,img) 1) do
     (if (= (mod (channels ,img) 2) 0) ; Account for alpha channel
       (loop for ,ch from 0 to (- (channels ,img) 2) do
         (setf (aref (data ,out)
                    (+ (the fixnum (* ,i (channels ,out))) ,ch))
           (progn ,@body)))
       (loop for ,ch from 0 to (- (channels ,img) 1) do
         (setf (aref (data ,out)
                    (+ (the fixnum (* ,i (channels ,out))) ,ch))
          (progn ,@body))))))

;; Abstracted macro for 
(defmacro operation-xy (img out ch op-x op-y &body body)
  `(loop for ,op-y of-type fixnum from 0 to (- (height ,img) 1) do
     (loop for ,op-x of-type fixnum from 0 to (- (width ,img) 1) do
       (if (= (mod (channels ,img) 2) 0) ; Account for alpha channel
         (loop for ,ch from 0 to (- (channels ,img) 2) do
           (setf (aref (data ,out)
                       (+ (the fixnum
                            (* (+ ,op-x (* ,op-y (width ,img)))
                               (channels ,out))) ,ch))
             (progn ,@body)))
         (loop for ,ch from 0 to (- (channels ,img) 1) do
           (setf (aref (data ,out)
                       (+ (the fixnum
                            (* (+ ,op-x (* ,op-y (width ,img)))
                               (channels ,out))) ,ch))
             (progn ,@body)))))))


;; Applies a mask to a given image. If the mask has a value of 1.0 (white),
;; then the image will stay the same. As it gets lower, it will scale the
;; image appropriately.
(defun apply-matte (img out matte)
  (declare (type image img out))
  (operation img out ch i (* (access-pixels-sdim img i ch)
                             (access-pixels-sdim matte i ch))))

(defun add-images (img-1 img-2 out)
  (declare (type image img-1 img-2 out))
  (operation img-1 out ch i (+ (access-pixels-sdim img-1 i ch)
                               (access-pixels-sdim img-2 i ch))))

(defun sub-images (img-1 img-2 out)
  (declare (type image img-1 img-2 out))
  (operation img-1 out ch i (- (access-pixels-sdim img-1 i ch)
                               (access-pixels-sdim img-2 i ch))))

(defun rgb-to-hue (r g b)
  (declare (type single-float r g b))
  (let ((ret 0.0))
    (let* ((max-c (max r g b))
           (min-c (min r g b))
           (delta (- max-c min-c)))
      (if (> max-c *hue-eps*)
        (progn
          (cond ((<= delta 0.0)
                 (setf ret 0.0f0))
                ((< (abs (- max-c r)) *hue-eps*)
                 (setf ret (/ (- g b) delta)))
                ((< (abs (- max-c g)) *hue-eps*)
                 (setf ret (+ 2.0 (/ (- b r) delta))))
                ((< (abs (- max-c b)) *hue-eps*)
                 (setf ret (+ 4.0 (/ (- r g) delta))))
                (t (setf ret 0.0f0)))
          (setf ret (* ret 60.0))
          (if (< ret 0.0) (setf ret (+ ret 360.0)))))
      ret)))

(defun rgb-to-sat (r g b)
  (declare (type single-float r g b))
  (let ((ret 0.0))
    (let* ((max-c (max r g b))
           (min-c (min r g b))
           (delta (- max-c min-c)))
      (if (> max-c *hue-eps*) (setf ret (/ delta max-c))))))

(defun rgb-to-val (r g b)
  (declare (type single-float r g b))
  (max r g b))

;; Binary functions like this make more sense as functions than methods
(defun keymix (img-1 img-2 out matte)
  (declare (type image img-1 img-2 out))
  (let* ((temp-1 (make-instance 'image :width (width img-1)
                                       :height (height img-1)
                                       :channels (channels img-1)))
         (temp-2 (make-instance 'image :width (width img-1)
                                       :height (height img-1)
                                       :channels (channels img-1))))
         (invert matte temp-1) ; Perform operation with inverse first
         (apply-matte img-2 temp-2 temp-1)
         (apply-matte img-1 temp-1 matte)
         (add-images temp-1 temp-2 out)))

;; Stub for a binary operation macro
;;(defmacro binary-operation (img-1 img-2 out ch &body body))

;;; Methods

;; Assumes an even number of channels, so that it must have an alpha
(defmethod premult ((img image) (out image))
  (operation img out ch i (* (access-pixels-sdim img i ch)
                             (access-pixels-sdim img i
                               (- (channels img) 1)))))

(defmethod invert ((img image) (out image))
  (declare (optimize (speed 3) (safety 0)))
  (operation img out ch i (- 1f0 (the single-float
                                   (access-pixels-sdim img i ch)))))

(defmethod brightness-inplace ((img image) val)
  (declare (type single-float val))
  (operation img img ch i (* (access-pixels-sdim img i ch) val)))

(defmethod brightness ((img image) (out image) val)
  (declare (optimize (speed 3) (safety 0))
           (type single-float val))
  (operation img out ch i (* (access-pixels-sdim img i ch) val)))

(defmethod simple-contrast ((img image) (out image))
  (declare (optimize (speed 3) (safety 0)))
  (operation img out ch i (* (the single-float
                             (- (access-pixels-sdim img i ch)
                                0.33f0)) 3f0)))

(defmethod contrast ((img image) (out image) val)
  (declare (optimize (speed 3) (safety 0))
           (type single-float val))
  (operation img out ch i (* (the single-float
                             (- (access-pixels-sdim img i ch)
                                 0.33f0)) 3f0)))

(defmethod gamma-correct ((img image) (out image) gamma)
  (declare (optimize (speed 3) (safety 0))
           (type single-float gamma))
  (operation img out ch i (expt
                              (access-pixels-sdim img i ch)
                            (the single-float (/ 1f0 gamma)))))

(defmethod monochrome ((img image) (out image))
  (declare (optimize (speed 3) (safety 0)))
  (operation img out ch i
    (+ (the single-float (* (access-pixels-sdim img i 0) 0.309f0))
       (the single-float (+ (the single-float
                              (* (access-pixels-sdim img i 1) 0.609f0))
                            (the single-float
                              (* (access-pixels-sdim img i 2)
                                 0.082f0)))))))

;; Takes a mix value and weights the pixels of each image based on that
;; value and its inverse, then sums the two values together for the final
;; mixed image.
(defmethod mix-image ((img-1 image) (img-2 image) (out image) mv)
  (declare (optimize (speed 3) (safety 0))
           (type single-float mv)
           (type image img-1 img-2 out) (inline + *))
  (operation img-1 out ch i (+ (the single-float
                               (* (access-pixels-sdim img-1 i ch) mv))
                             (the single-float 
                               (* (access-pixels-sdim img-2 i ch)
                                 (- 1f0 mv))))))

(defmethod over ((img-1 image) (img-2 image) (out image))
  (declare (optimize (speed 3) (safety 0)))
  (operation img-1 out ch i (+ (access-pixels-sdim img-1 i ch)
                               (* (- 1f0 (access-pixels-sdim img-1 i 3))
                                  (access-pixels-sdim img-2 i ch)))))

;; This neighbor function appropriately implements mirroring
(defun convolve-neighbor-dim (d md dind)
  (declare (type fixnum d md dind))
  (cond 
    ((>= (+ d dind) (- md 1))
     (- (- md 1) (- (+ d dind) (- md 1))))
    ((< (+ d dind) 0)
     (abs (+ d dind)))
    (t (+ d dind))))

;; Given a 3x3 filter, an input image and an output image, it destructively
;; stores the applied filter into the output image. The filter is assumed
;; to be a 3x3 element (or a single-dimensional array of size 9), which makes
;; for better efficiency and speed. It uses the reduce function to sum the
;; final values of the convolution, which are temporarily stored in the
;; working array.
(defmethod convolve-3 ((img image) (out image) filter)
  (declare (type (simple-array single-float (9)) filter))
  ;; Working array is declared outside the loop and altered destructively
  (let ((working (the (simple-array single-float)
                   (make-array 9
                     :element-type 'single-float
                     :initial-element 0f0))))
    (operation-xy img out ch op-x op-y
      (loop for y of-type fixnum from -1 to 1 do
          (loop for x of-type fixnum from -1 to 1 do
            (setf (aref working (+ (+ x 1) (* (+ y 1) 3)))
                  (* (aref filter (+ (+ x 1) (* (+ y 1) 3)))
                     (access-pixel img
                       (convolve-neighbor-dim op-x (width img) x)
                       (convolve-neighbor-dim op-y (height img) y)
                       ch)))))
      (reduce #'+ working))))

;; This function is more complicated, due to the median filter requiring a
;; sorting operation of brightness values to acquire the target. This is
;; set up to go through the image x and y, then acquire the brightness of
;; each pixel in the filter area. It also saves the index of the brightness
;; into a separate array, since a reference to that will be needed. The
;; pairlis function is then leveraged, along with the key parameter to the
;; sort function to sort by brightness. Then, the index paired with it can
;; be taken, then accessed to write the final value out. Lists are used
;; instead of arrays, which I typically use, since pairlis can't be used
;; with arrays. The operation macro is also inappropriate, as the channel
;; loop should be brought below the operation loops, since the logic is by
;; pixel, and not by each channel in the pixel like most operations.
(defmethod median-3 ((img image) (out image))
  (let ((working (list nil nil)))
    (loop for y of-type fixnum from 0 to 2 do
      (loop for x of-type fixnum from 0 to 2 do
        (push 0.0 (car working)) (push 0 (cadr working))))
    (loop for op-y of-type fixnum from 0 to (- (height img) 1) do
      (loop for op-x of-type fixnum from 0 to (- (width img) 1) do
        ;; Instead of appplying a filter, this will construct a set of
        ;; lists, one with the brightness and the other with the index.
        (loop for y of-type fixnum from -1 to 1 do
            (loop for x of-type fixnum from -1 to 1 do
              (setf (elt (car working) (+ (+ x 1) (* (+ y 1) 3)))
                (get-value img
                  (convolve-neighbor-dim op-x (width img) x)
                  (convolve-neighbor-dim op-y (height img) y)))
              (setf (elt (cadr working) (+ (+ x 1) (* (+ y 1) 3)))
                (+ (convolve-neighbor-dim op-x (width img) x)
                   (* (width img)
                      (convolve-neighbor-dim op-y (height img) y))))))
        ;; Now, out can have the element at index four (the middle) of the
        ;; sorted combination of the two lists, which can be paired with
        ;; the pairlis function. cdr is used over cadr, since pairlis stores
        ;; a pair of cons cells, not an actual list, so cdr gives us the
        ;; element, not the tail list like it does for a list.
        (let ((ind (cdr (elt (sort (pairlis (car working) (cadr working))
                                       #'< :key #'car) 4))))
          (loop for ch of-type fixnum from 0 to (- (channels img) 1) do
            (setf (aref (data out)
                    (+ ch (+ (* (channels out)
                      (+ op-x (* (width img) op-y))))))
                  (access-pixels-sdim img ind ch))))))))

;; Simple - convert to monochrome, then bump the contrast
(defmethod luma-key ((img image) (out image))
  (monochrome img out)
  (simple-contrast out out))

(defmethod chroma-key ((img image) (out image) hue)
  (declare (type single-float hue))
  (loop for i of-type fixnum from 0 to (- (image-dim img) 1) do
     (let* ((r (access-pixels-sdim img i 0))
            (g (access-pixels-sdim img i 1))
            (b (access-pixels-sdim img i 2)))
       (cond ((< (abs (- (rgb-to-hue r g b) hue)) *hue-eps*)
              (setf (aref (data out) (+ 0 (* i (channels img)))) 0.0)
              (setf (aref (data out) (+ 1 (* i (channels img)))) 0.0)
              (setf (aref (data out) (+ 2 (* i (channels img)))) 0.0))
             (t (loop for ch of-type fixnum from 0 to (- (channels img) 1) do
                  (setf (aref (data out) (+ ch (* i (channels img))))
                        (aref (data img) (+ ch (* i (channels img)))))))))))

(defmethod color-difference-blue ((img image) (out image))
  (loop for i of-type fixnum from 0 to (- (image-dim img) 1) do
   (let* ((r (access-pixels-sdim img i 0))
          (g (access-pixels-sdim img i 1))
          (b (access-pixels-sdim img i 2)))
     (if (> b g) (setf b g))
     (loop for ch of-type fixnum from 0 to (- (channels img) 1) do
       (setf (aref (data out) (+ ch (* i (channels img))))
             (aref (data img) (+ ch (* i (channels img))))))
     (setf (aref (data out) (+ 2 (* i (channels img)))) b))))

(defmethod color-difference-blue-matte ((img image) (out image))
  (loop for i of-type fixnum from 0 to (- (image-dim img) 1) do
    (let* ((r (access-pixels-sdim img i 0))
           (g (access-pixels-sdim img i 1))
           (b (access-pixels-sdim img i 2)))
      (setf b (- b (max r g)))
      (setf (aref (data out) (+ 0 (* i (channels img)))) b)
      (setf (aref (data out) (+ 1 (* i (channels img)))) b)
      (setf (aref (data out) (+ 2 (* i (channels img)))) b))))
