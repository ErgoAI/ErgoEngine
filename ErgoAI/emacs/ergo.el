;;; ergo.el --- a major mode for editing and running Ergo programs

;; Authors:
;; Heinz Uphoff (uphoff@informatik.uni-freiburg.de)
;; Christian Schlepphorst (schlepph@informatik.uni-freiburg.de)
;; Michael Kifer (kifer@cs.stonybrook.edu)

;;; Commentary:

;; This package provides a major mode for editing Ergo programs.
;; It knows about Ergo syntax and comments, and can send
;; regions, buffers, and files to an inferior interpreter process.

;; This package was adapted from flp.el (a major mode for FLIP) and expanded
;; by Michael Kifer.
;; In turn, flp.el, a major mode for FLORID, was adapted from prolog.el by
;; Heinz Uphoff and Christian Schlepphorst.

;;
;; SETUP:
;;
;;  ** For Ergo:
;;   (setq auto-mode-alist (cons '("\\(\\.flr$\\|\\.ergo$\\|\\.ergotxt$\\)" . ergo-mode) auto-mode-alist))
;;   (autoload 'ergo-mode "ergo" "Major mode for editing and running Ergo programs." t)
;;   (setq ergo-program-path "path-to-your-runergo")

;;; Code:

(require 'comint)

;; Is it XEmacs? NOT TESTED FOR XEMACS!!!
(defconst ergo-xemacs-p (featurep 'xemacs))
;; Is it Emacs?
(defconst ergo-emacs-p (not ergo-xemacs-p))

(defun ergo-buffer-live-p (buf)
  (and buf (get-buffer  buf) (buffer-name (get-buffer buf))))

(defconst ergo-temp-file-prefix
  (cond (ergo-emacs-p temporary-file-directory)
	((fboundp 'temp-directory) (temp-directory))
	(t "/tmp/"))
  "*Directory for temporary files.")

;; This path has to be set at the installation time of the F-Logic-System!!!
(defvar ergo-program-path "~/ERGOAI/ErgoEngine/ErgoAI/runergo"
  "*Program name for invoking an inferior Ergo with `run-ergo'.")
(defvar ergo-program-name nil
  "*Program name for invoking an inferior process with `run-ergo'. Internal.")

(defvar ergo-command-line nil
  "*Ergo command to execute at startup.")

(defvar ergo-mode-syntax-table nil)
(defvar ergo-mode-abbrev-table nil)
(defvar ergo-mode-map nil)

(defvar ergo-module-load-history nil)

(defvar ergo-forget-string "\\halt.\n"
  "*Reinitialise  system")

(defconst ergo-paren-shift 2
  "Amount of space to add when aligning with an open parenthesis.")
(defconst ergo-ergotext-shift 3
  "Amount of space to add when aligning with an open ErgoText parenthesis.")
(defconst ergo-ctrl-stmt-shift 3
  "Amount of space to add when aligning with \\then, \\else, \\until, \\do, \\while.")
(defconst ergo-comment-shift 3
  "Amount of space to add inside a multiline comment.")
(defconst ergo-infix-connective-left-shift 2
  "Amount of space by which to shift leftward the connectives ;, ==>, etc., if
  preceeded entirely by whitespace.")

(defconst ergo-other-statement-shift 3
  "Amount of space to add for all kinds of there statements like =, !=")

(defconst ergo-process-buffer-const "*ergo*"
  "Name of the Ergo buffer.")
(defconst ergo-process-name-const "ergo"
  "Name of the Ergo process.")
(defvar ergo-process-buffer nil
  "Name of the actual process buffer. Set at runtime.")
(defvar ergo-process-name nil
  "Name of the actual process. Set at runtime.")

(defvar ergo-offer-save t
  "*If non-nil, ask about saving modified buffers before 
\\[ergo-load-file] is run.")


(defvar ergo-indent-mline-comments-flag t
  "*Non-nil means automatically align comments when indenting.")

(defconst ergo-quoted-atom-regexp
  "'\\([^']\\|''\\)*'"
  "Regexp matching a quoted atom.")
(defconst ergo-unquoted-atom-regexp
  "\\([:.,()*&^$#@]\\|[A-Za-z0-9_]+\\)"
  "Regexp matching an unquoted atom.")
(defconst ergo-atom-regexp
  (format "\\(%s\\|%s\))" ergo-quoted-atom-regexp ergo-unquoted-atom-regexp)
  "Regexp matching an atom.")
(defconst ergo-string-regexp
  (format "\\(\"\\([^\n\"]\\|\"\"\\)*\"\\|%s\\)" ergo-quoted-atom-regexp)
  "Regexp matching a string (things inside double or single quotes).")
(defconst ergo-bracketed-object "\\[.*\\]"
  "Like list. Used to prevent recursion in ergo-list-regexp.")
(defconst ergo-list-regexp
  (format "\\[\\([^\]\[]*\\|%s\\)\\]" ergo-bracketed-object)
  "Regexp for matching a list.")
(defconst ergo-oid-regexp
  (format "\\(%s\\|%s\\|%s\\|%s\\|[A-Za-z0-9]+\\)"
	  ergo-bracketed-object ergo-list-regexp 
	  ergo-string-regexp ergo-atom-regexp)
  "Regexp to recognize oid.")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Do fontifification of F-logic-syntax with font-lock.
;; If font-lock is not installed, there should be no problem

(make-face 'ergo-font-lock-system-face)
(copy-face 'default 'ergo-font-lock-system-face)
(make-face-bold 'ergo-font-lock-system-face)
(set-face-foreground 'ergo-font-lock-system-face "violet")
;;------------------
(make-face 'ergo-font-lock-arrow-face)
(copy-face 'default 'ergo-font-lock-arrow-face)
(set-face-foreground 'ergo-font-lock-arrow-face "Maroon")
(make-face-bold 'ergo-font-lock-arrow-face)
;;--------------------------
(make-face 'ergo-font-lock-ergotext-face)
(set-face-foreground 'ergo-font-lock-ergotext-face "cyan")
(make-face-bold 'ergo-font-lock-ergotext-face)
;;-----------------------------------------
(make-face 'ergo-font-lock-signature-face)
;;(copy-face 'default 'ergo-font-lock-signature-face)
(set-face-foreground 'ergo-font-lock-signature-face "green")
(make-face-bold 'ergo-font-lock-signature-face)
;;-----------------------------------------
(make-face 'ergo-font-lock-bold-keyword-face)
(copy-face 'font-lock-keyword-face 'ergo-font-lock-bold-keyword-face)
(make-face-bold 'ergo-font-lock-bold-keyword-face)
;;-----------------------------------------
(make-face 'ergo-font-lock-query-face)
(set-face-foreground 'ergo-font-lock-query-face "darkgreen")
(make-face 'ergo-font-lock-transaction-face)
(set-face-foreground 'ergo-font-lock-transaction-face "DarkSlateBlue")
(make-face 'ergo-font-lock-preprocessor-face)
(set-face-foreground 'ergo-font-lock-preprocessor-face "MediumAquamarine")
(make-face 'ergo-font-lock-constant-face)
(set-face-foreground 'ergo-font-lock-constant-face "DarkGoldenrod4")

(defconst ergo-directives-regexp
  "index\\|semantics\\|ignoredeps\\|setruntime\\|setsemantics\\|symbol_context\\|compiler_options"
  "Ergo compiler directives without the \\( and \\).")

(defconst ergo-font-lock-keywords
   (list
    '("\\(\\(flora\\)? +\\?-\\|ergo>\\|[:!?]-\\|-:\\|\\.[ \t\n]*$\\)"
      1 'ergo-font-lock-query-face)
    ;; for objects
    '("\\([A-Za-z0-9_][A-Za-z0-9_!.]*\\) *\\["
      1 'font-lock-type-face)
    ;; for constants
    '("[^A-Za-z0-9_]\\([0-9]+\\(.[0-9]+\\|[eE][-+]?[0-9]+\\)?\\)[^A-Za-z0-9_]"
      1 'font-lock-constant-face)
    ;; for variables
    '("\\(\\?\\([A-Za-z_]+[A-Za-z0-9_]*\\)?\\)"
      1 'font-lock-variable-name-face)
    '("\\b\\(\\\+\\|true\\|false\\|undefined\\|avg\\|max\\|min\\|sum\\|count\\|avgdistinct\\|sumdistinct\\|countdistinct\\|setof\\|bagof\\|load\\|add\\|insert\\|delete\\|t_?insert\\|t_?delete\\|insertall\\|t_?insertall\\|deleteall\\|t_?deleteall\\|erase\\|eraseall\\|t_?erase\\|t_?eraseall\\|insertrule_?[az]?\\|deleterule\\|\\[a-z]+\\|p2h\\|semantics\\|setsemantics\\|caller\\|encoding\\|newoid\\|test\\|catch\\|clause\\|newmodule\\|erasemodule\\|t?enable\\|t?disable\\|is[a-z]+\\)\\b[ \t]*{"
      1 'font-lock-keyword-face)
    '("\\b\\(must\\|wish\\|exists?\\|forall\\|any\\|all\\|some\\)\\b[ \t]*("
      1 'font-lock-keyword-face)
    '("\\(\\\\[A-Za-z#]+\\)"
      1 'font-lock-keyword-face)
    '("\\(@!\\|@\\|@@\\)"
      1 'ergo-font-lock-bold-keyword-face)
    '("\\(->\\|=>\\|->->\\|[^>]==>\\|<==[^>]\\|<==>\\|[^>]~~>\\|<~~[^>]\\|<~~>\\|-->>\\)"
      1 'ergo-font-lock-arrow-face)
    '("\\(:\\|[^[(]|[^])]\\)" 
      1 'font-lock-type-face)
    '("\\(\\[|\\||\\]\\|(|\\||)\\)"
      1 'ergo-font-lock-signature-face)
    '("\\(\\\\\\[\\|\\\\\\]\\|\\\\(\\|\\\\)\\)"
      1 'ergo-font-lock-ergotext-face)
    '("\\(\\[\\|\\]\\|{\\|}\\)"
      1 'bold)
    (list (format "\\b\\(%s\\|^#[a-z]\\)\\b" ergo-directives-regexp)
	  1 '(quote ergo-font-lock-system-face))
    '("\\(\\b[A-Z0-9_]\\{3,\\}\\b\\)[^([]" 1 'ergo-font-lock-preprocessor-face)
    ;;'("\\(\\b[A-Za-z0-9_]+\\b\\)[ \t]*\\(([^)]+)\\)[ \t\n]*\\((.*)[ \t\C-m]*\\)?\\([---=]>\\)?"
    '("\\(\\b[A-Za-z0-9_]+\\b\\)[ \t]*?\\((\\|[---=]>\\)"
      1 'font-lock-function-name-face)
    ;; obj[boolprop,...]
    '("[[:alnum:]][[]|?[^[]*?\\(\\b[A-Za-z0-9_]+\\b\\)"
      1 'font-lock-function-name-face)
    ;; Next one be careful: can make font-lock UNUSABLY slow, if changed. Always
    ;; check indent2-test.flr
    ;; ppp, lll. or ppp :- . This does only shallow job.
    '("[ \t]*\\(\\(\\b[A-Za-z0-9_]+\\b[ \t]*\\)+\\)[ \t]*\\([.;]\\|:-\\)"
      1 'font-lock-function-name-face)
    ;; :- ppp, lll
    '("[?!:]-[[:space:]]*\\(\\(\\b[A-Za-z0-9_]+\\b[ \t]*[,;]?[ \t]*\\)+\\)"
      1 'font-lock-function-name-face)
    ;; \naf or \neg ppp
    ;;'("\\\\n[ea][fg][[:space:]]*\\(\\(\\b[A-Za-z0-9_]+\\b[ \t]*[,;]?[ \t]*?\\)+\\)"
    '("\\\\n[ea][fg]\\(\\([\t ]*\\b[A-Za-z0-9_]+\\b[ \t]*[,;]?\\)+?\\)"
      1 'font-lock-function-name-face)
    ;;'("\\(\\b%[A-Za-z0-9_]+\\b\\)[ \t]*\\((\\b[^)]+\\b)\\)?[ \t\n]*\\((.*)\\)?"
    '("\\(\\b%[A-Za-z0-9_]+\\b\\)[ \t]*(?"
      1 'ergo-font-lock-transaction-face)
    '("\\(\\b[A-Za-z0-9_]+\\b\\)"
      1 'ergo-font-lock-constant-face)
    )
  "Additional expressions to highlight in Ergo mode.")

;; matches ?...\(?...?\)+ where ? is [,(,{
(defconst indent-lparen-base
  ;;"\\(\\[[^][]*\\(\\[[^][]*\\][^][]*\\)+\\|([^()]*\\(([^()]*)[^()]*\\)+\\|{[^{}]*\\({[^{}]*}[^{}]*\\)+\\)"
  "[({[][^]{}()[]*\\([{([][^](){}[]*[]})][^](){}[]*\\)+"
  )
;; matches ?...\(?...?\)* where ? is [,(,{
(defconst indent-lparen-base2
  ;;"\\(\\[[^][]*\\(\\[[^][]*\\][^][]*\\)*\\|([^()]*\\(([^()]*)[^()]*\\)*\\|{[^{}]*\\({[^{}]*}[^{}]*\\)*\\)"
  "[({[][^]{}()[]*\\([{([][^](){}[]*[]})][^](){}[]*\\)*"
  )

;; matches closest previous {,(,[, possibly preceded with a \ 
(defconst nearest-unclosed-paren
  "\\\\?[{([][^](){}[]*"
  )

;; matches closest previous ),],}, possibly preceded with a \ 
(defconst nearest-closing-paren
  "\\\\?[]})][^](){}[]*"
  )

(defconst ergo-infix-connective
  "\\(;\\|\\\\or\\|\\\\and\\|,\\|<==>?\\|==>\\|<~~>?\\|~~>\\)"
  "Regexp matching a binary Ergo connective.")


(defvar ergo-mode-menu
  '(:visible (eq major-mode 'ergo-mode)
    ["Load Ergo file"    ergo-load-file-to-module   t]
    ["Load Ergo buffer"  ergo-load-buffer-to-module t]
    ["Load Ergo region"  ergo-load-region-to-module t]
    "---"
    ["Add Ergo file"    ergo-add-file-to-module   t]
    ["Add Ergo buffer"  ergo-add-buffer-to-module t]
    ["Add Ergo region"  ergo-add-region-to-module t]
    "---"
    ["Execute region as Ergo query"  ergo-send-region-as-query t]
    "---"
    ["Start Ergo process"     run-ergo     	    t]
    ["Restart Ergo process"   ergo-restart	    t]
    "---"
    ["Interrupt Ergo process" ergo-interrupt	    t]
    ["Quit Ergo process"      ergo-quit    	    t]
    ))
    
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(if ergo-mode-syntax-table
    ()
  (let ((table (make-syntax-table)))
    (modify-syntax-entry ?_    "_"      table)
    (modify-syntax-entry ?\\   "\\"     table)
    (modify-syntax-entry ?+    "."      table)
    (modify-syntax-entry ?-    "."      table)
    (modify-syntax-entry ?=    "."      table)
    ;(modify-syntax-entry ?%    "<"  table)
    (modify-syntax-entry ?\n   ">"    table)
    (modify-syntax-entry ?\C-m ">"      table)
    (modify-syntax-entry ?<    "."      table)
    (modify-syntax-entry ?>    "."      table)
    (modify-syntax-entry ?\'   "\""     table)
    (modify-syntax-entry ?/    ". 124" table)
    (modify-syntax-entry ?*    ". 23b"   table)
    (setq ergo-mode-syntax-table table)
    ))


(define-abbrev-table 'ergo-mode-abbrev-table ())

(defun ergo-mode-variables ()
  (set-syntax-table ergo-mode-syntax-table)
  (setq local-abbrev-table ergo-mode-abbrev-table)
  (make-local-variable 'paragraph-start)
  (setq paragraph-start (concat "\\(^[ \t]*$\\|" page-delimiter "\\|^[ \t]*/\*\\)"))
  ;;(make-local-variable 'paragraph-separate)
  ;;(setq paragraph-separate paragraph-start)
  (make-local-variable 'paragraph-ignore-fill-prefix)
  (setq paragraph-ignore-fill-prefix t)
  (make-local-variable 'indent-line-function)
  (setq indent-line-function 'ergo-indent-line)
  (make-local-variable 'comment-start)
  (setq comment-start "//")
  (make-local-variable 'comment-start-skip)
  (make-local-variable 'comment-end-skip)
  ;; This complex regexp makes sure that comments cannot start
  ;; inside quoted atoms or strings
  ;; If point before start of a comment, skips into the comment
  ;; (past //_? and /*_?). 
  (setq comment-start-skip 
	(format "^\\(%s\\|[^\n'\"/]\\)*\\(/\\*+ *\\|//+ *\\|//\\)"
		ergo-string-regexp))
  ;; Not used
  (setq comment-end-skip 
	(format " *\\*+/\\(%s\\|[^\n'\"/]\\)*$"
		ergo-string-regexp))
  (make-local-variable 'comment-end)
  (setq comment-end "")
  (make-local-variable 'comment-column)
  (setq comment-column 48)
  (make-local-variable 'comment-indent-function)
  (setq comment-indent-function 'ergo-comment-indent)
  (make-local-variable 'font-lock-defaults)
  (setq font-lock-defaults
	'(ergo-font-lock-keywords nil nil ((?_ . "w"))))
)

(defun ergo-mode-commands (map)
  (define-key map "\t"       'ergo-indent-line)
  (define-key map "\C-c\C-l" 'ergo-switch-to-ergo-buffer)
  (define-key map "\C-c\C-b" 'ergo-load-buffer-to-module)
  (define-key map "\C-c\C-r" 'ergo-load-region-to-module)
  (define-key map "\C-c\C-f" 'ergo-load-file-to-module)
  (define-key map "\C-c\C-s" 'ergo-restart)
  (define-key map "\C-c\C-c" 'ergo-interrupt)
  (define-key map "\C-c\C-d" 'ergo-quit)
  (define-key map "\C-c\C-q" 'ergo-send-region-as-query)
  (define-key map "\C-c%"    'ergo-paren-match)
  (define-key map "*"	     'ergo-electric-star)
  (define-key map "/"	     'ergo-electric-slash)
  (define-key map ")"	     'ergo-electric-rparen)
  (define-key map "]"	     'ergo-electric-rparen)
  (define-key map "}"	     'ergo-electric-rparen)
  (define-key map ";"	     'ergo-electric-punctuation)
  (define-key map "\M-q"     'ergo-fill-paragraph)
  )


;; Set up Ergo keymap
(if ergo-mode-map
    nil
  (setq ergo-mode-map (make-sparse-keymap))
  (ergo-mode-commands ergo-mode-map))


;;;###autoload
(defun ergo-mode ()
  "Major mode for editing F-Logic code.
Blank lines and `//...' separate paragraphs.

Commands:
\\{ergo-mode-map}
Entry to this mode calls the value of `ergo-mode-hook'
if that value is non-nil."
  (interactive)
  (kill-all-local-variables)
  (use-local-map ergo-mode-map)
  (setq major-mode 'ergo-mode)
  (setq mode-name "Ergo")
  (ergo-mode-variables)
  ;;(setq comint-prompt-regexp "ergo> +")
  (setq ergo-process-buffer ergo-process-buffer-const
        ergo-process-name   ergo-process-name-const
        ergo-program-name   ergo-program-path)
  ;; Set up Ergo menus
  (if window-system
      (easy-menu-define ergo-menubar ergo-mode-map "Ergo Commands"
        (cons "Ergo" ergo-mode-menu))
    )
  (run-hooks 'ergo-mode-hook))

(defun ergo-indent-line (&optional whole-exp)
  "Indent current line as Ergo code.
With argument, indent any additional lines of the same clause
rigidly along with this one (not yet)."
  (interactive "p")
  (let ((pos (- (point-max) (point)))
        (indent 0)
        beg first_line_char)
    (beginning-of-line)
    (setq beg (point))
    (skip-chars-forward " \t")

    ;; simulate electric insertion of character at point so if it is electric
    ;; then we get the right indentation
    (setq first_line_char (char-after))
    (setq last-command-event first_line_char)
    (setq indent (ergo-indent-level))

    (if (zerop (- indent (current-column)))
	nil
      (delete-region beg (point))
      (indent-to indent))
    (if (> (- (point-max) pos) (point))
	(goto-char (- (point-max) pos)))
    ))


;; with number, get indent level of current line + number
(defun ergo-indentation-level-of-line (&optional entry-point number)
  "Return the indentation level of the current line."
  (if (not entry-point)
      (setq entry-point (point)))
  (save-excursion
    (goto-char entry-point)
    (if (integerp number)
        (forward-line number))
    (beginning-of-line)
    (skip-chars-forward " \t")
    (current-column)))


(defun ergo-find-start-of-mline-comment (&optional with-skip-forward)
  "Return the start column of a /* */ comment.
This assumes that the point is inside a comment."
  (if (re-search-backward "/\\*" (point-min) t)
      (progn
        ;; make indent inside comment depend on first line
        (if with-skip-forward
            (progn
              (skip-chars-forward " \t" (ergo-get-eol))
              (+ ergo-comment-shift (current-column))
              )
          (current-column))
        )
    (error "Not inside a comment")
    ))

(defun ergo-in-mline-comment (&optional entry-point)
  "Check if point is inside multiline comment."
  (if (not entry-point)
      (setq entry-point (point)))

  (let ((pt entry-point))
    (save-excursion
      (goto-char entry-point)
      (if (re-search-backward "/\\*" (point-min) t)
	  ;; If after searching backward and finding /* we search forward
	  ;; and find no */ between */ and the point then we are inside
          ;; multiline comment
	  (not (re-search-forward "\\*/" pt t))
	))
    ))

(defun ergo-in-rest-of-line-comment (&optional entry-point)
  (if (not entry-point)
      (setq entry-point (point)))
  (save-excursion
    (goto-char entry-point)
    (re-search-backward "[^'\"]*//"  (ergo-get-bol) t)
    )
  )


;; this does not include the ergotext delimiters \( and \)
(defun ergo-inside-ergotext (&optional entry-point)
  (if (not entry-point)
      (setq entry-point (point)))
  ;; Bottleneck: this is called too often, and the expensive part is
  ;; calling ergo-get-beginning-of-clause-pos each time
  (save-excursion
    (goto-char entry-point)
    (and
     (re-search-backward
      "\\\\("  (ergo-get-beginning-of-clause-pos entry-point)  t)
     (not (re-search-forward "\\\\)"  (+ 2 entry-point) t)))
    )
  )

(defun ergo-in-comment-ergotext-string-or-qatom (&optional pos)
  (or pos (setq pos (point)))

  (or (ergo-in-mline-comment pos)
      (ergo-in-rest-of-line-comment pos)
      (ergo-sitting-in ergo-string-regexp pos)
      (ergo-sitting-in ergo-quoted-atom-regexp pos)
      (ergo-inside-ergotext pos)
      ))

(defun ergo-in-comment (&optional pos)
  (or pos (setq pos (point)))

  (or (ergo-in-mline-comment pos)
      (ergo-in-rest-of-line-comment pos)
      ))


(defun ergo-indent-level ()
  "Compute Ergo indentation level."
  (save-excursion
    (beginning-of-line)
    (skip-chars-forward " \t")
    (cond
     ((looking-at "///") (current-column))   ;; /// - leave comment where it is
     ;;End of /* */ comment
     ((or (looking-at "\\*/") (looking-at "\\*\\*"))
      (save-excursion
	(ergo-find-start-of-mline-comment)
	;;(skip-chars-backward " \t")
        ;; don't shift inside comment if */ or **
	;;(- (current-column) ergo-comment-shift))
        (current-column))
      )
     ;; Here we check if the current line is within a /* */ pair
     ((and (looking-at "[^*/]")
	   (ergo-in-mline-comment))
      (if (and ergo-indent-mline-comments-flag
               ;; things that start with \ are not indented.
               ;; Also, o, -, number followed by a dot.
               ;; This allows for simple formatting of comments
               (not (looking-at "\\(\\\\\\|[-*o:>] \\|[0-9]+\.\\)"))
               )
	  (ergo-find-start-of-mline-comment 'skip-forward)
	;; leave as before
	(ergo-indentation-level-of-line))
      )
     ((bobp) 0)				;Beginning of buffer
     (t
      (let ((ind 0)
            (ind1 0)
            (ind2 0)
            (prev-point 0)
            (entry-point (point))
            temp-indentation
            unmatched-paren
            search-limit)
	;; See previous indentation
        ;; skip empty space up to the last clause
        (ergo-skip-empty-backwards 'skip-line-up)
 	(if (bobp)
 	    (setq ind 0)		;Beginning of buffer
	  (setq ind (current-column)))	;Beginning of clause
	;; See its beginning
	(if (looking-at "//")
	    ind
	  (end-of-ergo-code-line)
	  (or (bobp) (ergo-backward-char 1))

          (save-excursion
            (setq prev-point (point)
                  search-limit (nearest-preceding-ergo-clause-delimiter
                                prev-point))
            (forward-char 1)
            (setq unmatched-paren
                  (ergo-nearest-unclosed-open-paren
                   entry-point
                   (nearest-preceding-ergo-clause-delimiter prev-point)))
            (cond ((setq temp-indentation
                         ;; indentation for control statements
                         (ergo-get-ctrl-stmt-indentation
                          "\\(\\\\then\\|\\\\else\\|\\\\do\\|\\\\until\\)"
                          ;;"\\(\\\\if\\|\\\\while\\|\\\\unless\\|\\\\do\\)"
                          ;; \do is a special context string - treated inside
                          ;; ergo-get-ctrl-stmt-indentation
                          "\\(\\\\if\\|\\\\while\\|\\\\unless\\)"
                          "[ \t]*\\(\\\\else\\|\\\\until\\)"
                          entry-point
                          search-limit))
                   (setq ind1 temp-indentation))
;;;                ((setq temp-indentation
;;;                       ;; indentation for -->>
;;;                       (ergo-get-ctrl-stmt-indentation
;;;                        "-->>"
;;;                        "("
;;;                        "[ \t]*;"
;;;                        entry-point
;;;                        search-limit))
;;;                 ;; adding +1 here since ";" is later always shifted -1 left
;;;                 (setq ind1 (1+ temp-indentation)))

                  ;; sitting inside .). .\). .]. .\]. .}. or .\}.
                  ((ergo-sitting-in ".?\\([\\]?[]})]\\).?" entry-point)
                   (save-excursion
                     (goto-char entry-point)
                     (ergo-paren-match)
                     ;; probably looking at \( of ergotext
                     (if (looking-at "\\\\")
                         (setq ind1 (current-column))
                       (setq ind1 (1+ (current-column)))
                       )))
                  (unmatched-paren
                   ;; found open, unclosed parenthesis: use it for indentation
                   (save-excursion
                     (goto-char unmatched-paren)
                     (if (looking-at "\\\\")
                         (setq ind1 (+ ergo-ergotext-shift (current-column)))
                       (setq ind1 (+ ergo-paren-shift (current-column))))
                     ))
                  ) ; cond
            )

          (save-excursion
            (goto-char entry-point)
            (if (and
                 (re-search-backward
                  "[:!?]-[.\n]*" (nearest-preceding-ergo-clause-delimiter) t)
                 (not (ergo-in-comment)))
                (setq ind2 tab-width)
              )
            )

          (if (ergo-sitting-in "@[!@]?{[^{}]*}.*[[:space:]\n]*" entry-point)
              (setq ind 0) ;; rule id/tag/etc
            (setq ind (if (> ind1 0) ind1 (max ind1 ind2)))
            )
          ;;(message "indentations: %S %S  %S %d"  ind1 ind2 ind (point))

	  (cond
           ((is-ergo-electric-rparen-char)
            (1- ind))
           ;; indenting \or \and ==> <== <==> <~~ ~~> ;
           ((ergo-indenting-connective entry-point)
            (- ind ergo-infix-connective-left-shift))
           ((and (looking-at "[,;({[]") (> ind 0))
            ind)
           ((looking-at "=")
            (+ ind ergo-other-statement-shift))
           ((ergo-sitting-in
             (format ":- *\\(%s\\)" ergo-directives-regexp))
            tab-width)
           ((looking-at "[^.]")
            ind)
           (t
            0))			;No indentation
	  )))
     )))

;; Returns t, if the string before point matches the regexp STR.
;; If search-limit is nil, search only to the beginning of line;
;;                  otherwise, search back till the limit
;; entry-point: search from this points; otherwise, from point.
(defun ergo-looking-back (str &optional search-limit entry-point)
  (let ((limit (or search-limit
                   (ergo-get-bol))))
    (save-excursion
      (if entry-point
          (goto-char entry-point))
      (setq entry-point (point))
      (looking-back str limit 'greedy)
      )
    )
  )

(defun expensive-looking-back (pattern)
  (looking-back pattern nil))

(defun ergo-seeing-open-lparen (&optional entry-point)
  (or entry-point (setq entry-point (point)))
  (ergo-nearest-unclosed-open-paren
   entry-point
   (nearest-preceding-ergo-clause-delimiter entry-point)))

;; should probably be replaced with ergo-get-end-of-clause or something
(defun ergo-right-at-dot (entry-point)
  (save-excursion
    (or
     (ergo-looking-back "[.][\n\t ]*" nil entry-point)
     (looking-at "[.][\n\t ]")
     )
    )
  )


;; does not check for comments etc
(defun ergo-nearest-possible-end-of-clause (entry-point)
  (save-excursion
    (let ((search-limit (ergo-get-beginning-of-clause-pos entry-point)))
      (if (re-search-backward
           "\\([.][ \t]+$\\|[.]$\\)"
           search-limit
           t)
          (match-beginning 0))
      )
    ))

;; Find the nearest preceding end of clause or nil
;; This one continues search if the previous end-of-clause is inside comments
;; or strings.
(defun ergo-get-end-of-clause (&optional entry-point)
  (or entry-point) (setq entry-point (point))

  (save-excursion
    (let ((pos entry-point)
          (continue t)
          (oldpos (point-max)))
      (while (and continue (not (bobp)) (< pos oldpos))
        ;;(message "endclause: %S"  (point))
        (goto-char pos)
        (setq oldpos pos)
        (setq pos (ergo-nearest-possible-end-of-clause oldpos))
        (if (and pos (not (ergo-in-comment-ergotext-string-or-qatom pos)))
            (setq continue nil)  ;; then we are done
          (or pos (setq pos (ergo-previous-line-of-pos oldpos))))
        ) ;; while
      (if (and (< pos entry-point) (ergo-right-at-dot pos))
          pos)
      ;; else nil
      )))

(defun nearest-preceding-ergo-clause-delimiter (&optional entry-point)
  (if (not entry-point)
      (setq entry-point (point)))
  (save-excursion
    (goto-char entry-point)
    (let ((end (ergo-get-end-of-clause entry-point))
          (beg (ergo-get-beginning-of-clause-pos entry-point)))
      (if (expensive-looking-back "^.*[?!:]-[^.]*")  ;; rule, query, latent query
          (setq beg (match-beginning 0)))
      (cond ((and end beg) (max end beg))
            (beg beg)
            (end end)
            (t (ergo-indentation-level-of-line entry-point -1))
        ))
    ))


;; Returns t, if the string surrounding point matches the regexp STR.
;; Left-limit - search only from that pos (on the same line). If nil - from
;; beginning of line.
(defun ergo-sitting-in (str &optional pos left-limit)
  ;;(message "sitin: %S %S "  pos str)
  (or pos (setq pos (point)))
  (or left-limit (setq left-limit 0))
  (save-excursion
    (goto-char pos) ;; to get to the right line
    (setq left-limit (max left-limit (ergo-get-bol)))
    (goto-char left-limit)
    (if (re-search-forward str (ergo-get-eol) t)
        (or
         (and (>= pos (match-beginning 0)) (< pos (match-end 0)))
         (ergo-sitting-in str pos (match-end 0)))
      )
    )
  )

;; changes point
(defun ergo-skip-comments-and-whitespace-backwards (&optional pos)
  (or pos (setq pos (point)))
  (goto-char pos)
  (while (and (not (bobp))
              (or (ergo-in-comment) (expensive-looking-back "[\n\t ]")))
    (if (ergo-in-mline-comment)
        (re-search-backward "/\\*" (point-min) t))
    (cond ((and (ergo-in-comment) (expensive-looking-back "/[*/]"))
           (ergo-backward-char 3))
          ((ergo-in-rest-of-line-comment)
           (re-search-backward "//" (ergo-get-bol) t)
           ;; stay within the comment
           (forward-char 2))
          (t (ergo-backward-char 1))
          )
    (skip-chars-backward "[\n\t ]")
    )
  (if (< (point) pos)
      (forward-char 1))
  )

;; Skip comments and whitespace backwards.
;; Then match the preceeding text with the pattern. If match then return the
;; current point. Else, return nil and go to the initial point.
(defun ergo-skip-comments-and-whitespace-backwards-if-match (pattern limit &optional pos)
  (or pos (setq pos (point)))
  (let (pos-after-skip)
    (save-excursion
      (goto-char pos)
      (while (and (not (bobp))
                  (or (ergo-in-comment) (looking-at-p "[\n\t ]")))
        (cond ((and (ergo-in-comment) (expensive-looking-back "/[*/]"))
               (ergo-backward-char 3))
              ((ergo-in-rest-of-line-comment)
               (re-search-backward "//" (ergo-get-bol) t)
               ;; stay within the comment
               (forward-char 2))
              (t (ergo-backward-char 1))
              )
        )
      (if (< (point) pos)
          (forward-char 1))
      (if (looking-back pattern limit 'greedy)
          (setq pos-after-skip (point)))
      )
    (if pos-after-skip
        (goto-char pos-after-skip))
  ))

;; Search backwards for pattern. If pattern is inside a comment, keep searching
;; If found, return the point. Else, return nil and go to the initial point.
;; At this point, if search is inside a comment, we don't loop but just 
;; do the search the second time.
;; Could be generalized, but what we have seems good enough.
(defun ergo-search-backwards-skip-comments-and-whitespace (pattern limit &optional pos)
  (or pos (setq pos (point)))
  (let (pos-after-skip)
    (save-excursion
      (goto-char pos)
      (re-search-backward pattern limit t)
      (if (ergo-in-comment)
          (progn
            (ergo-skip-comments-and-whitespace-backwards)
            (re-search-backward pattern limit t)
            )
        )
      (if (looking-at-p pattern)
          (setq pos-after-skip (point)))
      )
    (if pos-after-skip
        (goto-char pos-after-skip))
  ))


;; return beginning of line pos
(defun ergo-get-bol ()
  (save-excursion
    (beginning-of-line)
    (point)
    ))
;; return end of line pos
(defun ergo-get-eol ()
  (save-excursion
    (end-of-line)
    (point)
    ))


(defun ergo-electric-star (arg)
  "Insert a star character.
If the star is the second character of a C style comment introducing
construct, and we are on a comment-only-line, indent line as comment.
If numeric ARG is supplied or point is inside a literal, indentation
is inhibited."
  (interactive "*P")
  ;; if we are not in a comment, or if arg is given do not re-indent the
  ;; current line, unless this star introduces a comment-only line.
  (let ((indentp (and (not arg)
                      (or (expensive-looking-back "^[ \t]*/")
                          (and
                           (ergo-in-mline-comment)
                           (eq (char-before) ?*)
                           (save-excursion
                             (ergo-backward-char 1)
                             (skip-chars-backward "*")
                             (if (eq (char-before) ?/)
                                 (ergo-backward-char 1))
                             (skip-chars-backward " \t")
                             (bolp)))))
                 ))
    (self-insert-command (prefix-numeric-value arg))
    (if indentp
	(ergo-indent-line))
    ))

(defun ergo-electric-rparen (arg)
  "Insert a parentheses, bracket, brace character.
If this char is indented with white space then move 1 position to the left."
  (interactive "*P")
  (self-insert-command (prefix-numeric-value arg))
  (let ((indentp (and (not arg)
                      (is-ergo-electric-rparen-char)
                      (ergo-looking-back "^[ \t\\]*[])}]"))
                 )
        )
    (if indentp
	(ergo-indent-line)
      )
    ))

(defun is-ergo-electric-rparen-char ()
  (or (eq last-command-event ?\))
      (eq last-command-event ?\])
      (eq last-command-event ?\})))

(defun ergo-looking-at-electric-rparen-char ()
  (looking-at "})]"))

(defun ergo-electric-slash (arg)
  "Insert a slash character.

Indent the line as a comment, if:
The slash is part of a `*/' token that closes a block oriented comment.

If numeric ARG is supplied or point is inside a literal, indentation
is inhibited."
  (interactive "*P")
  (let* ((ch (char-before))
	 (indentp (and (not arg)
		       (eq last-command-event ?/)
		       (or 
			;;(and (eq ch ?/)
			;;     (not (ergo-in-literal)))
			(and (eq ch ?*)
			     (ergo-in-mline-comment)))
		       ))
	 )
    (self-insert-command (prefix-numeric-value arg))
    (if indentp
	(ergo-indent-line))))


(defun ergo-electric-punctuation (arg)
  "Insert a semicolon or other included punctuation.
If this char is indented with white space then move 1 position to the left."
  (interactive "*P")
  (self-insert-command (prefix-numeric-value arg))
  (let ((indentp (and (not arg)
                      (ergo-is-electric-punctuation-char)
                      (ergo-looking-back "^[ \t]*[;]"))
                 )
        )
    (if indentp
	(ergo-indent-line)
      )
    ))
(defun ergo-is-electric-punctuation-char ()
  (or (eq last-command-event ?\;)
      )
  )


(defun ergo-in-literal ()
  ;; to be worked out
  nil)



;; If can skip into a comment then goes into the comment and then backs off out
;; of that comment -- just to whatever the end of code on that line is.
;; Otherwise, to end of line.
;; Seems not to understand when some-code1 /*...*/ some-code2 -- will go to the
;; end of some-code1.  Should be changed to work from end of line backwards.
(defun end-of-ergo-code-line ()
  "Go to end of statement in this line."
  (beginning-of-line 1)
  (let* ((eolpos (ergo-get-eol)))
    (if (re-search-forward comment-start-skip eolpos 'move)
        (goto-char (match-end 0)))
    (skip-chars-backward " \t/*")))

(defun ergo-comment-indent ()
  "Compute Ergo style comment indentation."
  (cond ((looking-at "///") 0)
	((looking-at "//") (ergo-indent-level))
	(t
	 (save-excursion
	       (skip-chars-backward " \t")
	       ;; Insert one space at least, except at left margin.
	       (max (+ (current-column) (if (bolp) 0 1))
		    comment-column)))
	))

;; This may be a bit off, but does not matter. May give, say, the 2nd line in
;; the below situation:
;; @!{ruleid}
;; p(...) :- ...
;; `inpos' is the position from which to start the search
(defun ergo-get-beginning-of-clause-pos (&optional inpos)
  (let ((continue t)
        (result 1))
    (if (not inpos)
        (setq inpos (point)))
    ;; Bottleneck: calling this from within ergo-inside-ergotext makes this
    ;; mode slow
    (save-excursion
      (goto-char inpos)
      (ergo-skip-empty-backwards)
      (while (and continue (not (bobp)))
        (if (> (current-column) 0)
              (beginning-of-line)
          (if (or (ergo-in-comment)
                  (expensive-looking-back "[\n\t ]"))
              (ergo-skip-comments-and-whitespace-backwards))
          (if (expensive-looking-back "[\n\t ]")
              (skip-chars-backward "[\n\t ]"))
          (cond ((not (ergo-right-at-dot (point)))
                 (ergo-backward-char 1))
                (t (setq result (point))
                   (setq continue nil)
                   ))
          )
        )
      (setq result (point))
      )
    result
    ))

;; skip empty space up to the last clause
(defun ergo-skip-empty-backwards (&optional skip-line-up)
  (let ((empty t)
        prev-pos)
    (while (and empty (not (eq prev-pos (point))))
      (setq prev-pos (point))
      (if skip-line-up
          (ergo-backward-line 1))
      ;;(beginning-of-line)
      (if (bobp)
          (setq empty nil)
        (skip-chars-forward " \t")
        (if (not (or (looking-at "//") (looking-at "\n")))
            (setq empty nil))
        ))
    ))

(defun ergo-get-back-match-beginning-keep-point (str &optional limit)
  (if (not limit)
      (setq limit (ergo-get-beginning-of-clause-pos)))
  (save-excursion
    (if (re-search-backward str limit t)
        (match-beginning 0))))

(defun ergo-previous-line-of-pos (pos)
  (save-excursion
    (goto-char pos)
    (ergo-backward-line 1)
    (end-of-line)
    (point)))

;; cut down version of viper-paren-match, but is it sees ergotext then it
;; matches another ergotext delimiter, disregarding anything inside.
(defun ergo-paren-match ()
  "Go to the matching parenthesis."
  (interactive)
  (let ((parse-sexp-ignore-comments t)
	anchor-point)
    (let (beg-lim end-lim)
      (if (and (eolp) (not (bolp)))
          (ergo-backward-char 1))
      (if (not (looking-at "[][(){}]"))
          (setq anchor-point (point)))
      (setq beg-lim (line-beginning-position)
            end-lim (line-end-position))
      (cond ((re-search-forward "\\(\\\\(\\|\\\\)\\)" end-lim t)
             (backward-char 2))
            ((save-excursion
               (forward-char 1)
               (re-search-backward "\\(\\\\(\\|\\\\)\\)" beg-lim t))
             (forward-char 1)
             (re-search-backward "\\(\\\\(\\|\\\\)\\)" beg-lim t))
            ((re-search-forward "[][(){}]" end-lim t)
             (backward-char) )
            ((re-search-backward "[][(){}]" beg-lim t))
            (t
             ;;(error "No matching character on line"))))
             nil)))
    (cond ((or (looking-at "\\\\(")
               (and (looking-at "(") (expensive-looking-back "\\\\")))
           (re-search-forward "\\\\)" nil t)
           (backward-char 2))
          ((or (looking-at "\\\\)")
               (and (looking-at ")") (expensive-looking-back "\\\\")))
           (forward-char 2)
           (re-search-backward "\\\\(" nil t)
           )
          ((looking-at "[\(\[{]")
           (forward-sexp 1)
           (backward-char))
          ((looking-at "[])}]")
           (forward-char)
           (backward-sexp 1))
          (t
           ;;(error "Bell"))
           nil)
          )))


;; returns the position of the nearest preceding unclosed open parenthesis
(defun ergo-nearest-unclosed-open-paren (entry-point limit)
  (or entry-point (setq entry-point (point)))
  (save-excursion
    (goto-char entry-point)
    (ergo-nearest-unclosed-open-paren-aux 0 limit)))

;; `count' counts the number of closed parentheses to match with the open ones
(defun ergo-nearest-unclosed-open-paren-aux (count limit)
  ;;(message "count: %S point: %S char: %c"  count (point) (char-after))
  (let (pos)
    (cond ((ergo-looking-back nearest-unclosed-paren limit)
           (setq pos (match-beginning 0))
           (goto-char pos)
           (cond ((ergo-in-comment-ergotext-string-or-qatom pos)
                  (ergo-nearest-unclosed-open-paren-aux count limit))
                 ((= count 0) pos)
                 (t
                  (ergo-nearest-unclosed-open-paren-aux (1- count) limit)))
           )
          ((ergo-looking-back nearest-closing-paren limit)
           (setq pos (match-beginning 0))
           (goto-char pos)
           (cond ((ergo-in-comment-ergotext-string-or-qatom pos)
                  (ergo-nearest-unclosed-open-paren-aux count limit))
                 (t
                  (ergo-nearest-unclosed-open-paren-aux (1+ count) limit)))
          )
    )))


;; Intended to search for things like beginning of \if \then \else where
;; \if is a context string and \then or \else are trigger strings.
;; Similarly with \while-\do, \unless-\do, and \do-\until.
;; `end-str' here is a match for the ending of control statements of the form
;; ) \else and ) \until.
(defun ergo-get-ctrl-stmt-indentation (trigger-str context-str end-str &optional pos limit)
  (or pos (setq pos (point)))
  (or limit (setq limit (nearest-preceding-ergo-clause-delimiter pos)))

  (let (newpos newpos2 match-pos)
    (save-excursion
      (goto-char pos)
      (beginning-of-line)
      (cond (;; no extra indent if " \else" or " \until".
             ;; Also for (...-->> ... ;...)
             (looking-at-p end-str)
             (if (or (and (looking-at-p "[ \t]*\\\\else")
                          (ergo-search-backwards-skip-comments-and-whitespace
                           "\\\\if" limit)
                          (setq match-pos (point)))
                     (and (looking-at-p "[ \t]*\\\\until")
                          (ergo-search-backwards-skip-comments-and-whitespace
                           "\\\\do" limit)
                          (setq match-pos (point)))
                     (and (ergo-search-backwards-skip-comments-and-whitespace
                           context-str limit)
                          (setq match-pos (point)))
                     (and (ergo-search-backwards-skip-comments-and-whitespace
                           "\\\\do" limit)
                          (setq match-pos (point))))
                 (goto-char match-pos))
             (if (and (= (char-after pos) ?\))
                      (ergo-looking-back "^[ \t]*" nil pos))
                 ;; looking at spaces then the )
                 (1+ (current-column))
               (current-column)))
            (;; the trigger string must be not on the same line
             (re-search-forward trigger-str pos t)
             nil)
            ((ergo-skip-comments-and-whitespace-backwards-if-match
              (concat trigger-str "[ \t\n]*") limit)
             ;; pos is after trigger-str + space
             (or (ergo-search-backwards-skip-comments-and-whitespace
                  context-str limit)
                 ;; \do is an exceptional context.
                 ;; We first search for the context of
                 ;; \if, \while, \unless and only IF THAT FAILS we assume that
                 ;; we are dealing with a \do-\while or \do-\until
                 (ergo-search-backwards-skip-comments-and-whitespace
                  "\\\\do" limit))
             (+ ergo-ctrl-stmt-shift (current-column)))
            ((and (setq newpos (ergo-nearest-unclosed-open-paren pos limit))
                  (ergo-sitting-in
                   "^[ \t]*)?[ \t]*\\\\else[ \t]*(?[ \t]*$" newpos))
             ;; we are inside the else-part of the statement
             (if (and (= (char-after pos) ?\))
                      (ergo-looking-back "^[ \t]*" nil pos))
                 ;; looks like closing paren of \else(
                 (1+ (ergo-indentation-level-of-line newpos))
               ;; probably inside \else (.....), not closing paren
               (+ ergo-ctrl-stmt-shift 
                  (ergo-indentation-level-of-line newpos)))
             )
            ((and ;; taking care of parentheses
              (setq newpos (ergo-nearest-unclosed-open-paren pos limit))
              (goto-char newpos)
              (looking-at-p ".[ \t]*$")
              (looking-back (concat trigger-str "[ \t\n]*") limit 'greedy))
             (or (and
                  (not (ergo-sitting-in "^[ \t]*)[ \t]*\\\\until" pos))
                  (re-search-backward context-str limit t)
                  )
                 ;; \do is an exceptional context.
                 ;; We first search for the context of
                 ;; \if, \while, \unless and only IF THAT FAILS we assume that
                 ;; we are dealing with a \do-\while or \do-\until
                 (re-search-backward "\\\\do"))
             ;; if looking at a closing ")" of control statement then do not
             ;; shift to the right by ergo-ctrl-stmt-shift
             (if (and (= (char-after pos) ?\))
                      (ergo-looking-back "^[ \t]*" nil pos))
                 (1+ (current-column))
               (+ ergo-ctrl-stmt-shift (current-column)))
             )) ; cond
      ) ; save
    ) ; let
  )
         
(defun ergo-indenting-connective (pos)
  (save-excursion
    (goto-char pos)
    (beginning-of-line)
    (looking-at-p (concat "[ \t]*" ergo-infix-connective))))

(defun ergo-backward-char (num)
  (setq num (min (1- (point)) num))
  (or (bobp)
      (forward-char (- num))))

(defun ergo-backward-line (num)
  (or (bobp)
      (forward-line (- num))))

(defun ergo-fill-paragraph (&optional arg)
  (interactive "P")
  (if (ergo-in-mline-comment)
      (fill-paragraph arg)))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;
;;; Inferior Ergo mode
;;;
(defvar inferior-ergo-mode-map nil)

(defun inferior-ergo-mode ()
  "Major mode for interacting with an inferior Ergo process.

The following commands are available:
\\{inferior-ergo-mode-map}

Entry to this mode calls the value of `inferior-ergo-mode-hook' with no
arguments, if that value is non-nil.  Likewise with the value of
`comint-mode-hook'. 
`inferior-ergo-mode-hook' is called after `comint-mode-hook'.

You can send text to the inferior flora from other buffers
using the commands \\[ergo-load-buffer] \\[ergo-load-file], and
\\[ergo-load-region]. 

Return at end of buffer sends line as input.
Return not at end copies rest of line to end and sends it.
\\[comint-kill-input] and \\[backward-kill-word] are kill commands, imitating normal Unix input editing.
\\[comint-interrupt-subjob] interrupts the shell or its current subjob if any.
\\[comint-stop-subjob] stops. \\[comint-quit-subjob] sends quit signal."
  (interactive)
  (require 'comint)
  (comint-mode)
  (ergo-mode-variables)
  ;;(setq major-mode 'inferior-ergo-mode
  (setq major-mode 'comint-mode
        mode-name  "Ergo process"
        comint-prompt-regexp "^ergo> *"
        comint-input-ring-file-name (expand-file-name "~/.ergo-history"))
  (if inferior-ergo-mode-map 
      nil
    (setq inferior-ergo-mode-map (copy-keymap comint-mode-map))
    (ergo-mode-commands inferior-ergo-mode-map)
    (define-key inferior-ergo-mode-map "\M-\t" 'comint-dynamic-complete))
  (use-local-map inferior-ergo-mode-map)
  (run-hooks 'inferior-ergo-mode-hook)
  (or (file-exists-p comint-input-ring-file-name)
      (write-region 1 1 comint-input-ring-file-name))
  (comint-read-input-ring)
)

(defun run-ergo-background ()
  "Run an Ergo process.
Input and output via buffer *ergo*."
  (if (not (get-process ergo-process-name))
      (with-current-buffer (if ergo-command-line
   			       (make-comint ergo-process-name
					    ergo-program-name
					    nil
					    "-e"
					    ergo-command-line)
			     (make-comint
			      ergo-process-name ergo-program-name))
	(inferior-ergo-mode))))



;;;###autoload
(defun run-ergo ()
  "Run an Ergo process. Input and output via buffer *ergo*, and
switch to the buffer."
  (interactive)
  (run-ergo-background)
  (show-ergo-buffer 'switch))


(defun ergo-load-region (&optional beg end module)
  "Send the region to the Ergo process.
The region must be created in advance."
  (interactive)
  (or (and beg end)
      (if (not (mark t))
	  (error "No region specified"))
      (setq beg (min (point) (mark t))
	    end (max (point) (mark t))))
  (let ((tmpfile-name (ergo-make-temp-file beg end))
        command)
    (setq command 
          (format "load{'%s' >> %s}.\n" tmpfile-name (or module "main")))
    (run-ergo-background)
    (save-excursion
      (process-send-string
       ergo-process-name 
       command
       ))
    (show-ergo-buffer)
    ))

(defun ergo-send-region-as-query (&optional beg end)
  "Send the region to the Ergo process as a query.
The region must be a valid query terminated with a period."
  (interactive "r")
  (run-ergo-background)
  (let ((query (buffer-substring-no-properties beg end)))
    (save-excursion
      (process-send-string ergo-process-name query)
      (process-send-string ergo-process-name "\n"))
    )
  (show-ergo-buffer))

(defun ergo-load-buffer (&optional module)
  "Send the current buffer to the Ergo process.
Does not offer to save files."
  (interactive)
  (let ((file (file-name-nondirectory (buffer-file-name)))
	ergo-offer-save)
    (if file
	(progn
	  (setq file (concat ergo-temp-file-prefix file))
	  (write-region (point-min) (point-max) file)
	  (ergo-load-file file module))
      (ergo-load-region 
       (point-min-marker) (point-max-marker) module))
    ))


(defun ergo-load-file (&optional file module add)
  "Prompt for a file, offer to save all buffers, then run Ergo
on the file."
  (interactive "P")
  (let ((default-file (or (buffer-file-name) "none")))
    (if (not (stringp file))
	(setq file
	      (read-file-name
	       (format "File name to load (%s): "
		       (file-name-nondirectory default-file))
	       nil default-file)))
    (if ergo-offer-save
	(save-some-buffers))
    (run-ergo-background)
    (if add
        (process-send-string
         ergo-process-name
         (format "add{'%s' >> %s}.\n" file (or module "main")))
      (process-send-string
       ergo-process-name
       (format "load{'%s' >> %s}.\n" file (or module "main"))))
    (show-ergo-buffer)))

(defun ergo-load-file-to-module ()
  "Prompt for a module into which to load the file. Then prompt for file."
  (interactive)
  (let (module)
    (setq module (ergo-ask-module))
    (ergo-load-file nil module)))

(defun ergo-load-buffer-to-module ()
  "Prompt for a module into which to load the file."
  (interactive)
  (let (module)
    (setq module (ergo-ask-module))
    (ergo-load-buffer module)))

(defun ergo-load-region-to-module ()
  "Prompt for a module into which to load the region."
  (interactive)
  (let (module)
    (setq module (ergo-ask-module))
    (ergo-load-region nil nil module)))

(defun ergo-add-file-to-module ()
  "Prompt for a module to which to add the file. Then prompt for file."
  (interactive)
  (let (module)
    (setq module (ergo-ask-module-add))
    (ergo-load-file nil module)))

(defun ergo-add-buffer-to-module ()
  "Prompt for a module to which to add the file."
  (interactive)
  (let (module)
    (setq module (ergo-ask-module-add))
    (ergo-load-buffer module)))

(defun ergo-add-region-to-module ()
  "Prompt for a module to which to add the region."
  (interactive)
  (let (module)
    (setq module (ergo-ask-module-add))
    (ergo-load-region nil nil module)))

(defun ergo-ask-module ()
  (read-string "Module to load to (main): " nil
               'ergo-module-load-history "main"))
(defun ergo-ask-module-add ()
  (read-string "Module to add to (main): " nil
               'ergo-module-load-history "main"))

(defun ergo-interrupt()
  (interactive)
  (interrupt-process ergo-process-name))

(defun ergo-quit()
  (interactive)
  (quit-process ergo-process-name))

(defun ergo-restart ()
  (interactive)
  (run-ergo-background)
  (process-send-string ergo-process-name ergo-forget-string)
  (sit-for 2)
  (run-ergo)
  (sit-for 0))  ;; synchronize

(defun ergo-switch-to-ergo-buffer ()
  (interactive)
  (run-ergo-background)
  (pop-to-buffer ergo-process-buffer))

;; SWITCH means switch to the inferior Ergo buffer
(defun show-ergo-buffer (&optional switch)
  (let ((wind (selected-window)))
    (with-temp-buffer
      (sit-for 1))
      (set-buffer ergo-process-buffer)
      (or (ergo-get-visible-buffer-window ergo-process-buffer)
	  (progn
	    (display-buffer ergo-process-buffer)
	    (switch-to-buffer-other-window ergo-process-buffer)))
      ;; time is needed for XSB to return. otherwise, the point will be off
      (goto-char (point-max))
      (or switch
	  (select-window wind))))

(defun ergo-get-visible-buffer-window (buff)
  (if (ergo-buffer-live-p buff)
      (if ergo-xemacs-p
	  (get-buffer-window buff t)
	(get-buffer-window buff 'visible))))


(defun ergo-make-temp-file (start end)
  (let* ((f (make-temp-name (concat ergo-temp-file-prefix "ergo"))))
    ;; create the file
    (write-region start end
		  (concat f ".ergo")
		  nil          ; don't append---erase
		  'no-message) 
    (expand-file-name f)))


(provide 'ergo)


;;; ergo.el ends here

