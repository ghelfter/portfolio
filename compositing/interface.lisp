;;;; *********************************************************
;;;;                       Galen Helfter
;;;;                         DPA 8150
;;;;                         Project 1
;;;;                       interface.lisp
;;;; *********************************************************

(in-package :composite)

;; Hardcoded image locations
(defparameter *fn-img-1* "./assets/blindguardian.png")
(defparameter *fn-img-2* "./assets/gammaray.png")
(defparameter *fn-matte* "./assets/matte.png")
(defparameter *write-image-loc* "./assets/output.png")

;; Lookup table for base64 encoding
(defparameter *base64-lookup* (concatenate 'string "ABCDEFGHIJKLMNOPQRSTUVWXY"
                                                   "Zabcdefghijklmnopqrstuvwx"
                                                   "yz0123456789+/="))
(defparameter *test* (concatenate 'string "iVBORw0KGgoAAAANSUhEUgAAACgAAAAoCA"
                                          "IAAAADnC86AAAACXBIWXMAAAsTAAALEwEA"
                                          "mpwYAAAAB3RJTUUH4QoeEQodb7cXngAAAB"
                                          "l0RVh0Q29tbWVudABDcmVhdGVkIHdpdGgg"
                                          "R0lNUFeBDhcAAAAuSURBVFjD7c0BDQAACA"
                                          "Oga//OGsPNQQEqmVzoHBGLxWKxWCwWi8Vi"
                                          "sVgsFn+MF9p+AU8THgYkAAAAAElFTkSuQm"
                                          "CC"))

;; Working image buffer for us to write results to
(defparameter *img* nil)
(defparameter *img-2* nil)
(defparameter *matte* nil)
(defparameter *wimg* nil)

(defun to-8bit (value)
  (declare (single-float value))
  (max 0 (min 255 (floor (* value 255.0)))))

(defun ppm-encode (img)
  (let ((result (make-array (image-size img)
                            :element-type 'character
                            :fill-pointer 0
                            :adjustable t)))
    ;; Write header
    (vector-push-extend #\P result)
    (vector-push-extend #\3 result)
    (vector-push-extend #\Newline result)
    ;; Write width and height
    (let* ((tw (write-to-string (width img)))
           (th (write-to-string (height img))))
           (loop for i from 0 to (- (length tw) 1) do
             (vector-push-extend (char tw i) result))
           (vector-push-extend #\Space result)
           (loop for i from 0 to (- (length th) 1) do
             (vector-push-extend (char th i) result))
           (vector-push-extend #\Newline result))
    ;; Write 255
    (vector-push-extend #\2 result)
    (vector-push-extend #\5 result)
    (vector-push-extend #\5 result)
    (vector-push-extend #\Newline result)
    ;; Write the pixels now
    (loop for y from 0 to (- (height img) 1) do
      (loop for x from 0 to (- (width img) 1) do
        (loop for ch from 0 to (- (channels img) 1) do
          (let ((temp (write-to-string (to-8bit (access-pixel img x y ch)))))
            (loop for i from 0 to (- (length temp) 1) do
              (vector-push-extend (char temp i) result)))
          (vector-push-extend #\Space result)))
      (vector-push-extend #\Newline result))
    result))


(defun bytes-to-base-64-str (v1 v2 v3)
  (let* ((result (make-array 4 :element-type 'character))
         (chunk-1 (ash (logand v1 #xFC) -2))
         (chunk-2 (logior (ash (logand v1 #x03) 4) (ash (logand v2 #xF0) -4)))
         (chunk-3 (logior (ash (logand v2 #x0F) 2) (ash (logand v3 #xC0) -6)))
         (chunk-4 (logand v3 #x3F))) ; ash is an arithmetic bitshift
    (setf (elt result 0) (elt *base64-lookup* chunk-1)) ; Use lookup table
    (setf (elt result 1) (elt *base64-lookup* chunk-2))
    (setf (elt result 2) (elt *base64-lookup* chunk-3))
    (setf (elt result 3) (elt *base64-lookup* chunk-4))
    result))

(defun base-64-space-cost (original)
  (* (/ original 3) 4))

;; This function assumes a three channel image. It would be more difficult to
;; write a general one, and the likelihood is that to do so, we'd have to
;; just condense the image down to three channels, since that is what the
;; hex encoding is going to want.
(defun encode-64-image (img)
  (let ((encoded (make-array (base-64-space-cost (image-size img))
                             :element-type 'character)))
     (loop for i from 0 to (- (image-dim img) 1) do
       (let ((temp (bytes-to-base-64-str
                     (to-8bit (access-pixels-sdim img i 0))
                     (to-8bit (access-pixels-sdim img i 1))
                     (to-8bit (access-pixels-sdim img i 2)))))
         (loop for j from 0 to 3 do
           (setf (char encoded (+ (* i 4) j)) (elt temp j)))))
     encoded))

(defun image-to-hex-string (img)
  (declare (optimize (speed 3) (safety 0)) (type image img))
  (let* ((rval (make-array (+ (* 2 (image-size img))
                              (image-dim img) (image-dim img)
                              (* 4 (height img)))
                           :element-type 'character
                           :fill-pointer 0)))
    (loop for y of-type fixnum from 0 to (- (height img) 1) do
      (vector-push #\{ rval)
      (vector-push #\Space rval)
      (loop for x of-type fixnum from 0 to (- (width img) 1) do
        (let* ((r (to-8bit (access-pixel img x y 0)))
               (g (to-8bit (access-pixel img x y 1)))
               (b (to-8bit (access-pixel img x y 2)))
               (r-str (write-to-string r :base 16))
               (g-str (write-to-string g :base 16))
               (b-str (write-to-string b :base 16)))
          (declare (type simple-string r-str g-str b-str))
          (vector-push #\# rval)
          (cond ((< r 127)
                 (vector-push #\0 rval)
                 (vector-push (char r-str 0) rval))
                (t
                 (vector-push (char r-str 0) rval)
                 (vector-push (char r-str 1) rval)))
          (cond ((< g 127)
                 (vector-push #\0 rval)
                 (vector-push (char g-str 0) rval))
                (t
                 (vector-push (char g-str 0) rval)
                 (vector-push (char g-str 1) rval)))
          (cond ((< b 127)
                 (vector-push #\0 rval)
                 (vector-push (char b-str 0) rval))
                (t
                 (vector-push (char b-str 0) rval)
                 (vector-push (char b-str 1) rval))))
        (vector-push #\Space rval))
      (vector-push #\} rval)
      (vector-push #\Space rval))
    rval))

(defun to-char-buffer (img)
  (let ((rval (make-array (image-size img) :element-type 'character
                                           :fill-pointer 0)))
    (loop for i from 0 to (- (image-size img) 1) do
      (vector-push (code-char (to-8bit (elt (data img) i))) rval))
    rval))

;; Main function
(defun comp-main ()
  (push :inline-generic-function *features*)

  (if (> (list-length sb-ext:*posix-argv*) 1)
    (setf *fn-img-1* (cadr sb-ext:*posix-argv*)))
  (if (> (list-length sb-ext:*posix-argv*) 2)
    (setf *fn-img-2* (caddr sb-ext:*posix-argv*)))
  (if (> (list-length sb-ext:*posix-argv*) 3)
    (setf *matte* (cadddr sb-ext:*posix-argv*)))
  (setf *img* (make-instance 'image :width 0 :height 0 :channels 0))
  (setf *img-2* (make-instance 'image :width 0 :height 0 :channels 0))
  (setf *matte* (make-instance 'image :width 0 :height 0 :channels 0))
  (read-image *fn-img-1* *img*)
  (read-image *fn-img-2* *img-2*)
  (read-image *fn-matte* *matte*)
  (if (= (channels *img*) 4) (premult *img* *img*))
  (if (= (channels *img-2*) 4) (premult *img-2* *img-2*))
  (if (= (channels *matte*) 4) (premult *matte* *matte*))
  (setf *wimg* (make-instance 'image :width (width *img*)
                                     :height (height *img*)
                                     :channels (channels *img*)))
  (ltk:with-ltk ()
    (let* ((tl (make-instance 'ltk:toplevel :width 900 :height 500))
           (tk-img (make-instance 'ltk:photo-image
                                                   :name "disp-image"
                                                   :width  (width *img*)
                                                   :height (height *img*)))
           ;; (format-wish
           (f1 (make-instance 'ltk:frame :master tl)) ; column 1
           (f2 (make-instance 'ltk:frame :master tl)) ; column 2
           (l-basic (make-instance 'ltk:label :master f1
                                              :text "Basic"
                                              :foreground :darkred))
           (f3 (make-instance 'ltk:frame :master tl))
           (c (make-instance 'ltk:canvas :master f3 :width (width *img*)
                                         :height (height *img*)))
                                         ;;:image tk-img))
           (b-gam (make-instance 'ltk:button :master f1
                     :text "Gamma"
                     :command (lambda ()
                       (progn
                         (gamma-correct *img* *wimg* 1.45)
                         (ltk:image-writedata tk-img
                           (image-to-hex-string *wimg*))))))
           (b-contr (make-instance 'ltk:button :master f1
                      :text "Contrast"
                      :command (lambda ()
                      (progn
                        (simple-contrast *img* *wimg*)
                        (ltk:image-writedata tk-img
                          (image-to-hex-string *wimg*))))))
           (b-inv (make-instance 'ltk:button :master f1
                    :text "Invert"
                    :command (lambda ()
                    (progn
                      (invert *img* *wimg*)
                      (ltk:image-writedata tk-img
                        (image-to-hex-string *wimg*))))))
           (b-mono (make-instance 'ltk:button :master f1
                     :text "Monochrome"
                     :command (lambda ()
                     (progn
                       (monochrome *img* *wimg*)
                       (ltk:image-writedata tk-img
                         (image-to-hex-string *wimg*))))))
           ;; Second set of buttons
           (l-spatial (make-instance 'ltk:label :master f2
                                                :text "Spatial"
                                                :foreground :darkred))
           (b-edge (make-instance 'ltk:button :master f2
                     :text "Edge-detect"
                     :command (lambda ()
                       (progn
                         (convolve-3 *img* *wimg* *kernel-edge*)
                         (ltk:image-writedata tk-img
                           (image-to-hex-string *wimg*))))))
           (b-blur (make-instance 'ltk:button :master f2
                     :text "Blur"
                     :command (lambda ()
                       (progn
                         (convolve-3 *img* *wimg* *kernel-blur*)
                         (ltk:image-writedata tk-img
                           (image-to-hex-string *wimg*))))))
           (b-sharpen (make-instance 'ltk:button :master f2
                        :text "Sharpen"
                        :command (lambda ()
                          (progn
                            (convolve-3 *img* *wimg* *kernel-sharpen*)
                            (ltk:image-writedata tk-img
                              (image-to-hex-string *wimg*))))))
           (b-median (make-instance 'ltk:button :master f2
                        :text "Median"
                        :command (lambda ()
                          (progn
                            (median-3 *img* *wimg*)
                            (ltk:image-writedata tk-img
                              (image-to-hex-string *wimg*))))))
           (blk4 (make-instance 'ltk:label :master f2))
           (l-keying (make-instance 'ltk:label :master f2
                                               :text "Keying"
                                               :foreground :darkred))
           (b-luma (make-instance 'ltk:button :master f2
                      :text "Luma Key"
                      :command (lambda ()
                        (progn
                          (luma-key *img* *wimg*)
                          (ltk:image-writedata tk-img
                            (image-to-hex-string *wimg*))))))
           (b-chroma (make-instance 'ltk:button :master f2
                       :text "Chroma Key"
                       :command (lambda ()
                         (progn
                           (chroma-key *img* *wimg* 0.0)
                           (ltk:image-writedata tk-img
                             (image-to-hex-string *wimg*))))))
           (b-diff (make-instance 'ltk:button :master f2
                      :text "Difference"
                      :command (lambda ()
                        (progn
                          (color-difference-blue *img* *wimg*)
                          (ltk:image-writedata tk-img
                            (image-to-hex-string *wimg*))))))
           (b-diffm (make-instance 'ltk:button :master f2
                       :text "Difference Matte"
                       :command (lambda ()
                         (progn
                           (color-difference-blue-matte *img* *wimg*)
                           (ltk:image-writedata tk-img
                             (image-to-hex-string *wimg*))))))

           (blk1 (make-instance 'ltk:label :master f1))
           (lshow (make-instance 'ltk:label :master f1 :text "Show"
                                            :foreground :darkred))
           (bimg-1 (make-instance 'ltk:button :master f1 :text "Image 1"
                     :command (lambda () (ltk:image-writedata tk-img
                                           (image-to-hex-string *img*)))))
           (bimg-2 (make-instance 'ltk:button :master f1 :text "Image 2"
                     :command (lambda () (ltk:image-writedata tk-img
                                           (image-to-hex-string *img-2*)))))
           (blk2 (make-instance 'ltk:label :master f1))
           (l3 (make-instance 'ltk:label :master f1
                                          :text "Comp"
                                          :foreground :darkred))
           (b-add (make-instance 'ltk:button :master f1
                    :text "Add"
                    :command (lambda ()
                    (progn
                      (add-images *img* *img-2* *wimg*)
                      (ltk:image-writedata tk-img
                        (image-to-hex-string *wimg*))))))
           (b-sub (make-instance 'ltk:button :master f1
                    :text "Subtract"
                    :command (lambda ()
                    (progn
                      (sub-images *img* *img-2* *wimg*)
                      (ltk:image-writedata tk-img
                        (image-to-hex-string *wimg*))))))
           (b-mix (make-instance 'ltk:button :master f1
                    :text "Mix"
                    :command (lambda ()
                    (progn
                      (mix-image *img* *img-2* *wimg* 0.5)
                      (ltk:image-writedata tk-img
                        (image-to-hex-string *wimg*))))))
           (b-key-mix (make-instance 'ltk:button :master f1
                        :text "Key Mix"
                        :command (lambda ()
                        (progn
                          (keymix *img* *img-2* *wimg* *matte*)
                          (ltk:image-writedata tk-img
                            (image-to-hex-string *wimg*))))))
           (b-over (make-instance 'ltk:button :master f1
                     :text "Over"
                     :command (lambda ()
                     (progn
                       (over *img* *img-2* *wimg*)
                       (ltk:image-writedata tk-img
                         (image-to-hex-string *wimg*))))))
           (blk3 (make-instance 'ltk:label :master f1))
           (l5 (make-instance 'ltk:label :master f1
                                          :text "File"
                                          :foreground :darkred))
           (b8 (make-instance 'ltk:button :master f1
                 :text "Write"
                 :command (lambda ()
                   (write-image *write-image-loc* *wimg*)))))

           ;; Close the default toplevel
           (ltk:withdraw ltk:*tk*)
           ;; Set our toplevel to close lisp when it exits
           (ltk:on-close tl 'sb-ext:exit)
           ;; Set toplevel position to the center of the screen
           (ltk:set-geometry-xy tl (/ (ltk:screen-width) 2)
                                   (/ (ltk:screen-height) 2))
           ;; Set title of the toplevel
           (ltk:wm-title tl "8150 Compositing")
           (ltk:image-writedata tk-img (image-to-hex-string *img*))

           (ltk:pack f1 :side :left :fill :y)
           (ltk:pack l-basic :side :top)
           (ltk:pack b-gam :side :top :fill :x)
           (ltk:pack b-contr :side :top :fill :x)
           (ltk:pack b-inv :side :top :fill :x)
           (ltk:pack b-mono :side :top :fill :x)
           (ltk:pack blk1 :side :top)
           (ltk:pack l3 :side :top)
           (ltk:pack b-add :side :top :fill :x)
           (ltk:pack b-sub :side :top :fill :x)
           (ltk:pack b-mix :side :top :fill :x)
           (ltk:pack b-key-mix :side :top :fill :x)
           (ltk:pack b-over :side :top :fill :x)
           (ltk:pack blk3 :side :top)
           (ltk:pack l5 :side :top)
           (ltk:pack b8 :side :top :fill :x)

           (if (< (channels *img*) 4) (ltk:configure b-over :state :disabled))

           (ltk:pack f2 :side :left :fill :y)
           (ltk:pack l-spatial :side :top)
           (ltk:pack b-edge :side :top :fill :x)
           (ltk:pack b-blur :side :top :fill :x)
           (ltk:pack b-sharpen :side :top :fill :x)
           (ltk:pack b-median :side :top :fill :x)
           (ltk:pack blk4 :side :top)
           (ltk:pack l-keying :side :top)
           (ltk:pack b-luma :side :top :fill :x)
           (ltk:pack b-chroma :side :top :fill :x)
           (ltk:pack b-diff :side :top :fill :x)
           (ltk:pack b-diffm :side :top :fill :x)

           (ltk:pack blk2 :side :top)
           (ltk:pack lshow :side :top)
           (ltk:pack bimg-1 :side :top :fill :x)
           (ltk:pack bimg-2 :side :top :fill :x)

           (let ((c-img (ltk:create-image c 0 0 :image tk-img)))
             (ltk:itemconfigure c c-img "anchor" "nw"))

           (ltk:pack f3 :side :left)
           (ltk:pack c :side :top)
           (ltk:configure f1 :borderwidth 3)
           (ltk:configure f1 :relief :sunken)
           (ltk:configure f2 :borderwidth 3)
           (ltk:configure f2 :relief :sunken))))
