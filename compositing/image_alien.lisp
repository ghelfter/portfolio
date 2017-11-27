;;;; *********************************************************
;;;;                       Galen Helfter
;;;;                         DPA 8150
;;;;                         Project 1
;;;;                      image_alien.lisp
;;;; *********************************************************

;;; Package definition
;;(defpackage :composite
;;  (:use :common-lisp))
(in-package :composite)

;;; Alien types definitions
(sb-alien:define-alien-type nil
  (sb-alien:struct ImageStore
    (data (* (sb-alien:single-float)))
    (w sb-alien:unsigned-int)
    (h sb-alien:unsigned-int)
    (channels sb-alien:unsigned-int)))

;;; Foreign function definitions
(sb-alien:define-alien-routine image_load sb-alien:integer
  (img (* (sb-alien:struct ImageStore)))
  (filename sb-alien:c-string))

(sb-alien:define-alien-routine image_write sb-alien:integer
  (img (* (sb-alien:struct ImageStore)))
  (filename sb-alien:c-string))

(sb-alien:define-alien-routine image_cleanup sb-alien:void
  (img (* (sb-alien:struct ImageStore))))

(defmacro with-image-read (var fname &body body)
  `(let ((,var (sb-alien:make-alien (sb-alien:struct ImageStore))))
     (image_load ,var ,fname)
     (unwind-protect (progn ,@body)
       (image_cleanup ,var)
       (sb-alien:free-alien ,var))))

(defun convert-to-c-image (img)
  (declare (optimize (speed 3) (safety 0)))
  (declare (type image img))
  (let ((istruct (sb-alien:make-alien (sb-alien:struct ImageStore))))
    (setf (sb-alien:slot istruct 'data) (sb-alien:make-alien
                                           sb-alien:single-float
                                           (image-size img)))
    (setf (sb-alien:slot istruct 'w) (width img))
    (setf (sb-alien:slot istruct 'h) (height img))
    (setf (sb-alien:slot istruct 'channels) (channels img))
    (loop for i from 0 to (- (image-size img) 1) do
      (setf (sb-alien:deref (sb-alien:slot istruct 'data) i)
            (elt (data img) i)))
    istruct))

(defmacro with-image-write (var fname img &body body)
  `(let ((,var (convert-to-c-image ,img)))
     (image_write ,var ,fname)
     (unwind-protect (progn ,@body)
       (image_cleanup ,var)
       (sb-alien:free-alien ,var))))

(defun convert-from-c-image (istruct img)
  (declare ((sb-alien:alien (sb-alien:struct ImageStore)) istruct))
  (declare (type image img))
  (declare (optimize (speed 3) (safety 0)))
  (setf (slot-value img 'width) (sb-alien:slot istruct 'w))
  (setf (slot-value img 'height) (sb-alien:slot istruct 'h))
  (setf (slot-value img 'channels) (sb-alien:slot istruct 'channels))
  (setf (slot-value img 'data) (make-array (* (width img)
                                              (height img)
                                              (channels img))
                                           :element-type 'single-float
                                           :initial-element 0f0))
  (loop for i from 0 to (- (image-size img) 1) do
    (setf (elt (slot-value img 'data) i) (sb-alien:deref
                                 (sb-alien:slot istruct 'data) i))))

(defun read-image (filename img)
  (declare (optimize (speed 3) (safety 0)))
  (declare (simple-string filename))
  (declare (type image img))
  (with-image-read istruct filename
    (convert-from-c-image istruct img)))

(defun write-image (filename img)
  (declare (simple-string filename))
  (declare (type image img))
  (if (> (channels img) 3) (clear-channel img 3 1.0))
  (with-image-write istruct filename img ()))
