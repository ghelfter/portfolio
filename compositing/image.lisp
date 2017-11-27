;;;; *********************************************************
;;;;                       Galen Helfter
;;;;                         DPA 8150
;;;;                         Project 1
;;;;                        image.lisp
;;;; *********************************************************

;;; Package definition
;;(defpackage :composite
;;  (:use :common-lisp))
(in-package :composite)

;;; Generics
(defgeneric access-pixel (img x y channel)
  (:documentation "Given an x and y, accesses the image at that
                  plus the given channel."))

(defgeneric access-pixels-sdim (img i channel)
  (:documentation "Accesses a pixel at a given, single dimensional index."))

(defgeneric get-value (img x y)
  (:documentation "Given an xy index, returns the brightness of a pixel"))

(defgeneric image-size (img)
  (:documentation "Given an image, calculates its total size."))

(defgeneric image-dim (img)
  (:documentation "Given an image, calculates the width by the height."))

(defgeneric clear-channel (img channel value)
  (:documentation "Given an image and a channel, clears all of the channel
                  to the given value."))

;;; Class definition
(defclass image ()
  ((width
     :initarg :width
     :initform (error "Must specify a width")
     :accessor width
     :type fixnum
     :documentation "Width of the image.")
   (height
     :initarg :height
     :initform (error "Must specify a height")
     :accessor height
     :type fixnum
     :documentation "Height of the image.")
   (channels
     :initarg :channels
     :initform (error "Must specify channels")
     :accessor channels
     :type fixnum
     :documentation "Channels of the image.")
   (data
     :initform nil
     :accessor data
     :type (simple-array single-float)
     :documentation "Data where the image is stored.")))

;;; Methods
(defmethod initialize-instance :after ((img image) &key)
  (setf (slot-value img 'data) (make-array
                                 (* (width img)
                                    (the fixnum (* (height img)
                                    (channels img))))
                                 :element-type 'single-float
                                 :initial-element 0f0)))

(defmethod access-pixel ((img image) x y channel)
  (declare (type fixnum x y channel))
  (aref (slot-value img 'data) (+ channel
                                 ( + (the fixnum (* x (channels img)))
                                     (the fixnum
                                       (the fixnum (* (the fixnum (* y (width img)))
                                                      (channels img)))))))) 
(defmethod access-pixels-sdim ((img image) i channel)
  (declare (type fixnum i channel))
  (aref (slot-value img 'data)
       (+ channel (the fixnum(* i (channels img))))))

(defmethod image-size ((img image))
  (* (channels img) (the fixnum (* (width img) (height img)))))

(defmethod image-dim ((img image))
  (* (width img) (height img)))

(defmethod clear-channel ((img image) channel value)
  (declare (type fixnum channel) (type single-float value))
  (loop for i of-type fixnum from 0 to (- (image-dim img) 1) do
    (setf (aref (slot-value img 'data)
               (+ (* i (channels img)) channel)) value)))

(defmethod get-value (img x y)
  (+ (the single-float (* (the single-float
                            (access-pixel img x y 0)) 0.309f0))
     (the single-float (+ (the single-float
                            (* (the single-float
                               (access-pixel img x y 1)) 0.609f0))
                           (the single-float
                             (* (the single-float
                                  (access-pixel img x y 2))
                                0.082f0))))))

;; Type declarations for optimization
(declaim (ftype (function (image) fixnum) width))
(declaim (ftype (function (image) fixnum) height))
(declaim (ftype (function (image) fixnum) channels))
(declaim (ftype (function (&rest t) single-float) access-pixel))
(declaim (ftype (function (&rest t) single-float) access-pixels-sdim))
(declaim (ftype (function (&rest t) (simple-array single-float)) data))
(declaim (ftype (function (image) fixnum) image-size))
(declaim (ftype (function (image) fixnum) image-dim))
