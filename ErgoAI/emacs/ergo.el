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
(defconst flora-xemacs-p (featurep 'xemacs))
;; Is it Emacs?
(defconst flora-emacs-p (not flora-xemacs-p))

(defun flora-buffer-live-p (buf)
  (and buf (get-buffer  buf) (buffer-name (get-buffer buf))))

(defconst flora-temp-file-prefix
  (cond (flora-emacs-p temporary-file-directory)
	((fboundp 'temp-directory) (temp-directory))
	(t "/tmp/"))
  "*Directory for temporary files.")

;; This path has to be set at the installation time of the F-Logic-System!!!
(defconst ergo-program-path "~/ERGOAI/ErgoAI/runergo"
  "*Program name for invoking an inferior Ergo with `run-ergo'.")
(defvar ergo-program-name nil
  "*Program name for invoking an inferior process with `run-ergo'. Internal.")

(defvar ergo-command-line nil
  "*Ergo command to execute at startup.")

(defvar flora-mode-syntax-table nil)
(defvar flora-mode-abbrev-table nil)
(defvar flora-mode-map nil)

(defvar flora-module-load-history nil)

(defvar flora-forget-string "\\halt.\n"
  "*Reinitialise  system")

(defconst flora-paren-shift 2
  "Amount of space to add when aligning with an open parenthesis.")
(defconst flora-ergotext-shift 3
  "Amount of space to add when aligning with an open ErgoText parenthesis.")
(defconst flora-ctrl-stmt-shift 3
  "Amount of space to add when aligning with \\then, \\else, \\until, \\do, \\while.")
(defconst flora-comment-shift 3
  "Amount of space to add inside a multiline comment.")
(defconst flora-infix-connective-left-shift 2
  "Amount of space by which to shift leftward the connectives ;, ==>, etc., if
  preceeded entirely by whitespace.")

(defconst flora-other-statement-shift 3
  "Amount of space to add for all kinds of there statements like =, !=")

(defconst ergo-process-buffer-const "*ergo*"
  "Name of the Ergo buffer.")
(defconst ergo-process-name-const "ergo"
  "Name of the Ergo process.")
(defvar flora-process-buffer nil
  "Name of the actual process buffer. Set at runtime.")
(defvar flora-process-name nil
  "Name of the actual process. Set at runtime.")

(defvar flora-offer-save t
  "*If non-nil, ask about saving modified buffers before 
\\[flora-load-file] is run.")


(defvar flora-indent-mline-comments-flag t
  "*Non-nil means automatically align comments when indenting.")

(defconst flora-quoted-atom-regexp
  "'\\([^']\\|''\\)*'"
  "Regexp matching a quoted atom.")
(defconst flora-unquoted-atom-regexp
  "\\([:.,()*&^$#@]\\|[A-Za-z0-9_]+\\)"
  "Regexp matching an unquoted atom.")
(defconst flora-atom-regexp
  (format "\\(%s\\|%s\))" flora-quoted-atom-regexp flora-unquoted-atom-regexp)
  "Regexp matching an atom.")
(defconst flora-string-regexp
  (format "\\(\"\\([^\n\"]\\|\"\"\\)*\"\\|%s\\)" flora-quoted-atom-regexp)
  "Regexp matching a string (things inside double or single quotes).")
(defconst flora-bracketed-object "\\[.*\\]"
  "Like list. Used to prevent recursion in flora-list-regexp.")
(defconst flora-list-regexp
  (format "\\[\\([^\]\[]*\\|%s\\)\\]" flora-bracketed-object)
  "Regexp for matching a list.")
(defconst flora-oid-regexp
  (format "\\(%s\\|%s\\|%s\\|%s\\|[A-Za-z0-9]+\\)"
	  flora-bracketed-object flora-list-regexp 
	  flora-string-regexp flora-atom-regexp)
  "Regexp to recognize oid.")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Do fontifification of F-logic-syntax with font-lock.
;; If font-lock is not installed, there should be no problem

(make-face 'flora-font-lock-system-face)
(copy-face 'default 'flora-font-lock-system-face)
(make-face-bold 'flora-font-lock-system-face)
(set-face-foreground 'flora-font-lock-system-face "violet")
;;------------------
(make-face 'flora-font-lock-arrow-face)
(copy-face 'default 'flora-font-lock-arrow-face)
(set-face-foreground 'flora-font-lock-arrow-face "Maroon")
(make-face-bold 'flora-font-lock-arrow-face)
;;--------------------------
(make-face 'flora-font-lock-ergotext-face)
(set-face-foreground 'flora-font-lock-ergotext-face "cyan")
(make-face-bold 'flora-font-lock-ergotext-face)
;;-----------------------------------------
(make-face 'flora-font-lock-signature-face)
;;(copy-face 'default 'flora-font-lock-signature-face)
(set-face-foreground 'flora-font-lock-signature-face "green")
(make-face-bold 'flora-font-lock-signature-face)
;;-----------------------------------------
(make-face 'flora-font-lock-bold-keyword-face)
(copy-face 'font-lock-keyword-face 'flora-font-lock-bold-keyword-face)
(make-face-bold 'flora-font-lock-bold-keyword-face)
;;-----------------------------------------
(make-face 'flora-font-lock-query-face)
(set-face-foreground 'flora-font-lock-query-face "darkgreen")
(make-face 'flora-font-lock-transaction-face)
(set-face-foreground 'flora-font-lock-transaction-face "DarkSlateBlue")
(make-face 'flora-font-lock-preprocessor-face)
(set-face-foreground 'flora-font-lock-preprocessor-face "MediumAquamarine")
(make-face 'flora-font-lock-constant-face)
(set-face-foreground 'flora-font-lock-constant-face "DarkGoldenrod4")

(defconst flora-directives-regexp
  "index\\|semantics\\|ignoredeps\\|setruntime\\|setsemantics\\|symbol_context\\|compiler_options"
  "Ergo compiler directives without the \\( and \\).")

(defconst flora-font-lock-keywords
   (list
    '("\\(\\(flora\\)? +\\?-\\|ergo>\\|[:!?]-\\|-:\\|\\.[ \t\n]*$\\)"
      1 'flora-font-lock-query-face)
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
      1 'flora-font-lock-bold-keyword-face)
    '("\\(->\\|=>\\|->->\\|[^>]==>\\|<==[^>]\\|<==>\\|[^>]~~>\\|<~~[^>]\\|<~~>\\|-->>\\)"
      1 'flora-font-lock-arrow-face)
    '("\\(:\\|[^[(]|[^])]\\)" 
      1 'font-lock-type-face)
    '("\\(\\[|\\||\\]\\|(|\\||)\\)"
      1 'flora-font-lock-signature-face)
    '("\\(\\\\\\[\\|\\\\\\]\\|\\\\(\\|\\\\)\\)"
      1 'flora-font-lock-ergotext-face)
    '("\\(\\[\\|\\]\\|{\\|}\\)"
      1 'bold)
    (list (format "\\b\\(%s\\|^#[a-z]\\)\\b" flora-directives-regexp)
	  1 '(quote flora-font-lock-system-face))
    '("\\(\\b[A-Z0-9_]\\{3,\\}\\b\\)[^([]" 1 'flora-font-lock-preprocessor-face)
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
      1 'flora-font-lock-transaction-face)
    '("\\(\\b[A-Za-z0-9_]+\\b\\)"
      1 'flora-font-lock-constant-face)
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

(defconst flora-infix-connective
  "\\(;\\|\\\\or\\|\\\\and\\|,\\|<==>?\\|==>\\|<~~>?\\|~~>\\)"
  "Regexp matching a binary Ergo connective.")


(defvar ergo-mode-menu
  '(:visible (eq major-mode 'ergo-mode)
    ["Load Ergo file"    flora-load-file-to-module   t]
    ["Load Ergo buffer"  flora-load-buffer-to-module t]
    ["Load Ergo region"  flora-load-region-to-module t]
    "---"
    ["Add Ergo file"    flora-add-file-to-module   t]
    ["Add Ergo buffer"  flora-add-buffer-to-module t]
    ["Add Ergo region"  flora-add-region-to-module t]
    "---"
    ["Execute region as Ergo query"  flora-send-region-as-query t]
    "---"
    ["Start Ergo process"     run-ergo     	    t]
    ["Restart Ergo process"   ergo-restart	    t]
    "---"
    ["Interrupt Ergo process" ergo-interrupt	    t]
    ["Quit Ergo process"      ergo-quit    	    t]
    ))
    
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(if flora-mode-syntax-table
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
    (setq flora-mode-syntax-table table)
    ))


(define-abbrev-table 'flora-mode-abbrev-table ())

(defun flora-mode-variables ()
  (set-syntax-table flora-mode-syntax-table)
  (setq local-abbrev-table flora-mode-abbrev-table)
  (make-local-variable 'paragraph-start)
  (setq paragraph-start (concat "\\(^[ \t]*$\\|" page-delimiter "\\|^[ \t]*/\*\\)"))
  ;;(make-local-variable 'paragraph-separate)
  ;;(setq paragraph-separate paragraph-start)
  (make-local-variable 'paragraph-ignore-fill-prefix)
  (setq paragraph-ignore-fill-prefix t)
  (make-local-variable 'indent-line-function)
  (setq indent-line-function 'flora-indent-line)
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
		flora-string-regexp))
  ;; Not used
  (setq comment-end-skip 
	(format " *\\*+/\\(%s\\|[^\n'\"/]\\)*$"
		flora-string-regexp))
  (make-local-variable 'comment-end)
  (setq comment-end "")
  (make-local-variable 'comment-column)
  (setq comment-column 48)
  (make-local-variable 'comment-indent-function)
  (setq comment-indent-function 'flora-comment-indent)
  (make-local-variable 'font-lock-defaults)
  (setq font-lock-defaults
	'(flora-font-lock-keywords nil nil ((?_ . "w"))))
)

(defun flora-mode-commands (map)
  (define-key map "\t"       'flora-indent-line)
  (define-key map "\C-c\C-l" 'flora-switch-to-flora-buffer)
  (define-key map "\C-c\C-b" 'flora-load-buffer-to-module)
  (define-key map "\C-c\C-r" 'flora-load-region-to-module)
  (define-key map "\C-c\C-f" 'flora-load-file-to-module)
  (define-key map "\C-c\C-s" 'ergo-restart)
  (define-key map "\C-c\C-c" 'ergo-interrupt)
  (define-key map "\C-c\C-d" 'ergo-quit)
  (define-key map "\C-c\C-q" 'flora-send-region-as-query)
  (define-key map "\C-c%"    'flora-paren-match)
  (define-key map "*"	     'flora-electric-star)
  (define-key map "/"	     'flora-electric-slash)
  (define-key map ")"	     'flora-electric-rparen)
  (define-key map "]"	     'flora-electric-rparen)
  (define-key map "}"	     'flora-electric-rparen)
  (define-key map ";"	     'flora-electric-punctuation)
  (define-key map "\M-q"     'flora-fill-paragraph)
  )


;; Set up Ergo keymap
(if flora-mode-map
    nil
  (setq flora-mode-map (make-sparse-keymap))
  (flora-mode-commands flora-mode-map))


;;;###autoload
(defun ergo-mode ()
  "Major mode for editing F-Logic code.
Blank lines and `//...' separate paragraphs.

Commands:
\\{flora-mode-map}
Entry to this mode calls the value of `flora-mode-hook'
if that value is non-nil."
  (interactive)
  (kill-all-local-variables)
  (use-local-map flora-mode-map)
  (setq major-mode 'ergo-mode)
  (setq mode-name "Ergo")
  (flora-mode-variables)
  ;;(setq comint-prompt-regexp "ergo> +")
  (setq flora-process-buffer ergo-process-buffer-const
        flora-process-name   ergo-process-name-const
        ergo-program-name   ergo-program-path)
  ;; Set up Ergo menus
  (if window-system
      (easy-menu-define ergo-menubar flora-mode-map "Ergo Commands"
        (cons "Ergo" ergo-mode-menu))
    )
  (run-hooks 'flora-mode-hook))

(defun flora-indent-line (&optional whole-exp)
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
    (setq indent (flora-indent-level))

    (if (zerop (- indent (current-column)))
	nil
      (delete-region beg (point))
      (indent-to indent))
    (if (> (- (point-max) pos) (point))
	(goto-char (- (point-max) pos)))
    ))


;; with number, get indent level of current line + number
(defun flora-indentation-level-of-line (&optional entry-point number)
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


(defun flora-find-start-of-mline-comment (&optional with-skip-forward)
  "Return the start column of a /* */ comment.
This assumes that the point is inside a comment."
  (if (re-search-backward "/\\*" (point-min) t)
      (progn
        ;; make indent inside comment depend on first line
        (if with-skip-forward
            (progn
              (skip-chars-forward " \t" (flora-get-eol))
              (+ flora-comment-shift (current-column))
              )
          (current-column))
        )
    (error "Not inside a comment")
    ))

(defun flora-in-mline-comment (&optional entry-point)
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

(defun flora-in-rest-of-line-comment (&optional entry-point)
  (if (not entry-point)
      (setq entry-point (point)))
  (save-excursion
    (goto-char entry-point)
    (re-search-backward "[^'\"]*//"  (flora-get-bol) t)
    )
  )


;; this does not include the ergotext delimiters \( and \)
(defun flora-inside-ergotext (&optional entry-point)
  (if (not entry-point)
      (setq entry-point (point)))
  ;; Bottleneck: this is called too often, and the expensive part is
  ;; calling flora-get-beginning-of-clause-pos each time
  (save-excursion
    (goto-char entry-point)
    (and
     (re-search-backward
      "\\\\("  (flora-get-beginning-of-clause-pos entry-point)  t)
     (not (re-search-forward "\\\\)"  (+ 2 entry-point) t)))
    )
  )

(defun flora-in-comment-ergotext-string-or-qatom (&optional pos)
  (or pos (setq pos (point)))

  (or (flora-in-mline-comment pos)
      (flora-in-rest-of-line-comment pos)
      (flora-sitting-in flora-string-regexp pos)
      (flora-sitting-in flora-quoted-atom-regexp pos)
      (flora-inside-ergotext pos)
      ))

(defun flora-in-comment (&optional pos)
  (or pos (setq pos (point)))

  (or (flora-in-mline-comment pos)
      (flora-in-rest-of-line-comment pos)
      ))


(defun flora-indent-level ()
  "Compute Ergo indentation level."
  (save-excursion
    (beginning-of-line)
    (skip-chars-forward " \t")
    (cond
     ((looking-at "///") (current-column))   ;; /// - leave comment where it is
     ;;End of /* */ comment
     ((or (looking-at "\\*/") (looking-at "\\*\\*"))
      (save-excursion
	(flora-find-start-of-mline-comment)
	;;(skip-chars-backward " \t")
        ;; don't shift inside comment if */ or **
	;;(- (current-column) flora-comment-shift))
        (current-column))
      )
     ;; Here we check if the current line is within a /* */ pair
     ((and (looking-at "[^*/]")
	   (flora-in-mline-comment))
      (if (and flora-indent-mline-comments-flag
               ;; things that start with \ are not indented.
               ;; Also, o, -, number followed by a dot.
               ;; This allows for simple formatting of comments
               (not (looking-at "\\(\\\\\\|[-*o:>] \\|[0-9]+\.\\)"))
               )
	  (flora-find-start-of-mline-comment 'skip-forward)
	;; leave as before
	(flora-indentation-level-of-line))
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
        (flora-skip-empty-backwards 'skip-line-up)
 	(if (bobp)
 	    (setq ind 0)		;Beginning of buffer
	  (setq ind (current-column)))	;Beginning of clause
	;; See its beginning
	(if (looking-at "//")
	    ind
	  (end-of-flora-code-line)
	  (or (bobp) (flora-backward-char 1))

          (save-excursion
            (setq prev-point (point)
                  search-limit (nearest-preceding-flora-clause-delimiter
                                prev-point))
            (forward-char 1)
            (setq unmatched-paren
                  (flora-nearest-unclosed-open-paren
                   entry-point
                   (nearest-preceding-flora-clause-delimiter prev-point)))
            (cond ((setq temp-indentation
                         ;; indentation for control statements
                         (flora-get-ctrl-stmt-indentation
                          "\\(\\\\then\\|\\\\else\\|\\\\do\\|\\\\until\\)"
                          ;;"\\(\\\\if\\|\\\\while\\|\\\\unless\\|\\\\do\\)"
                          ;; \do is a special context string - treated inside
                          ;; flora-get-ctrl-stmt-indentation
                          "\\(\\\\if\\|\\\\while\\|\\\\unless\\)"
                          "[ \t]*\\(\\\\else\\|\\\\until\\)"
                          entry-point
                          search-limit))
                   (setq ind1 temp-indentation))
;;;                ((setq temp-indentation
;;;                       ;; indentation for -->>
;;;                       (flora-get-ctrl-stmt-indentation
;;;                        "-->>"
;;;                        "("
;;;                        "[ \t]*;"
;;;                        entry-point
;;;                        search-limit))
;;;                 ;; adding +1 here since ";" is later always shifted -1 left
;;;                 (setq ind1 (1+ temp-indentation)))

                  ;; sitting inside .). .\). .]. .\]. .}. or .\}.
                  ((flora-sitting-in ".?\\([\\]?[]})]\\).?" entry-point)
                   (save-excursion
                     (goto-char entry-point)
                     (flora-paren-match)
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
                         (setq ind1 (+ flora-ergotext-shift (current-column)))
                       (setq ind1 (+ flora-paren-shift (current-column))))
                     ))
                  ) ; cond
            )

          (save-excursion
            (goto-char entry-point)
            (if (and
                 (re-search-backward
                  "[:!?]-[.\n]*" (nearest-preceding-flora-clause-delimiter) t)
                 (not (flora-in-comment)))
                (setq ind2 tab-width)
              )
            )

          (if (flora-sitting-in "@[!@]?{[^{}]*}.*[[:space:]\n]*" entry-point)
              (setq ind 0) ;; rule id/tag/etc
            (setq ind (if (> ind1 0) ind1 (max ind1 ind2)))
            )
          ;;(message "indentations: %S %S  %S %d"  ind1 ind2 ind (point))

	  (cond
           ((is-flora-electric-rparen-char)
            (1- ind))
           ;; indenting \or \and ==> <== <==> <~~ ~~> ;
           ((flora-indenting-connective entry-point)
            (- ind flora-infix-connective-left-shift))
           ((and (looking-at "[,;({[]") (> ind 0))
            ind)
           ((looking-at "=")
            (+ ind flora-other-statement-shift))
           ((flora-sitting-in
             (format ":- *\\(%s\\)" flora-directives-regexp))
            tab-width)
           ((looking-at "[^.]")
            ind)
           (t
            0))			;No indentation
	  )))
     )))

;; Returns t, if the string before point matches the regexp STR.
;; If search-limit  is nil, search only to the beginning of the line;
;;                  otherwise, search back till the limit
;; entry-point: search from this points; otherwise, from point.
(defun flora-looking-back (str &optional search-limit entry-point)
  (let ((limit (or search-limit
                   (flora-get-bol))))
    (save-excursion
      (if entry-point
          (goto-char entry-point))
      (setq entry-point (point))
      (looking-back str limit 'greedy)
      )
    )
  )

(defun flora-seeing-open-lparen (&optional entry-point)
  (or entry-point (setq entry-point (point)))
  (flora-nearest-unclosed-open-paren
   entry-point
   (nearest-preceding-flora-clause-delimiter entry-point)))

;; should probably be replaced with flora-get-end-of-clause or something
(defun flora-right-at-dot (entry-point)
  (save-excursion
    (or
     (flora-looking-back "[.][\n\t ]*" nil entry-point)
     (looking-at "[.][\n\t ]")
     )
    )
  )


;; does not check for comments etc
(defun flora-nearest-possible-end-of-clause (entry-point)
  (save-excursion
    (let ((search-limit (flora-get-beginning-of-clause-pos entry-point)))
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
(defun flora-get-end-of-clause (&optional entry-point)
  (or entry-point) (setq entry-point (point))

  (save-excursion
    (let ((pos entry-point)
          (continue t)
          (oldpos (point-max)))
      (while (and continue (not (bobp)) (< pos oldpos))
        ;;(message "endclause: %S"  (point))
        (goto-char pos)
        (setq oldpos pos)
        (setq pos (flora-nearest-possible-end-of-clause oldpos))
        (if (and pos (not (flora-in-comment-ergotext-string-or-qatom pos)))
            (setq continue nil)  ;; then we are done
          (or pos (setq pos (flora-previous-line-of-pos oldpos))))
        ) ;; while
      (if (and (< pos entry-point) (flora-right-at-dot pos))
          pos)
      ;; else nil
      )))

(defun nearest-preceding-flora-clause-delimiter (&optional entry-point)
  (if (not entry-point)
      (setq entry-point (point)))
  (save-excursion
    (goto-char entry-point)
    (let ((end (flora-get-end-of-clause entry-point))
          (beg (flora-get-beginning-of-clause-pos entry-point)))
      (if (looking-back "^.*[?!:]-[^.]*")  ;; rule, query, latent query
          (setq beg (match-beginning 0)))
      (cond ((and end beg) (max end beg))
            (beg beg)
            (end end)
            (t (flora-indentation-level-of-line entry-point -1))
        ))
    ))


;; Returns t, if the string surrounding point matches the regexp STR.
;; Left-limit - search only from that pos (on the same line). If nil - from
;; beginning of line.
(defun flora-sitting-in (str &optional pos left-limit)
  ;;(message "sitin: %S %S "  pos str)
  (or pos (setq pos (point)))
  (or left-limit (setq left-limit 0))
  (save-excursion
    (goto-char pos) ;; to get to the right line
    (setq left-limit (max left-limit (flora-get-bol)))
    (goto-char left-limit)
    (if (re-search-forward str (flora-get-eol) t)
        (or
         (and (>= pos (match-beginning 0)) (< pos (match-end 0)))
         (flora-sitting-in str pos (match-end 0)))
      )
    )
  )

;; changes point
(defun flora-skip-comments-and-whitespace-backwards (&optional pos)
  (or pos (setq pos (point)))
  (goto-char pos)
  (while (and (not (bobp))
              (or (flora-in-comment) (looking-back "[\n\t ]")))
    (if (flora-in-mline-comment)
        (re-search-backward "/\\*" (point-min) t))
    (cond ((and (flora-in-comment) (looking-back "/[*/]"))
           (flora-backward-char 3))
          ((flora-in-rest-of-line-comment)
           (re-search-backward "//" (flora-get-bol) t)
           ;; stay within the comment
           (forward-char 2))
          (t (flora-backward-char 1))
          )
    (skip-chars-backward "[\n\t ]")
    )
  (if (< (point) pos)
      (forward-char 1))
  )

;; Skip comments and whitespace backwards.
;; Then match the preceeding text with the pattern. If match then return the
;; current point. Else, return nil and go to the initial point.
(defun flora-skip-comments-and-whitespace-backwards-if-match (pattern limit &optional pos)
  (or pos (setq pos (point)))
  (let (pos-after-skip)
    (save-excursion
      (goto-char pos)
      (while (and (not (bobp))
                  (or (flora-in-comment) (looking-at-p "[\n\t ]")))
        (cond ((and (flora-in-comment) (looking-back "/[*/]"))
               (flora-backward-char 3))
              ((flora-in-rest-of-line-comment)
               (re-search-backward "//" (flora-get-bol) t)
               ;; stay within the comment
               (forward-char 2))
              (t (flora-backward-char 1))
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
(defun flora-search-backwards-skip-comments-and-whitespace (pattern limit &optional pos)
  (or pos (setq pos (point)))
  (let (pos-after-skip)
    (save-excursion
      (goto-char pos)
      (re-search-backward pattern limit t)
      (if (flora-in-comment)
          (progn
            (flora-skip-comments-and-whitespace-backwards)
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
(defun flora-get-bol ()
  (save-excursion
    (beginning-of-line)
    (point)
    ))
;; return end of line pos
(defun flora-get-eol ()
  (save-excursion
    (end-of-line)
    (point)
    ))


(defun flora-electric-star (arg)
  "Insert a star character.
If the star is the second character of a C style comment introducing
construct, and we are on a comment-only-line, indent line as comment.
If numeric ARG is supplied or point is inside a literal, indentation
is inhibited."
  (interactive "*P")
  ;; if we are not in a comment, or if arg is given do not re-indent the
  ;; current line, unless this star introduces a comment-only line.
  (let ((indentp (and (not arg)
                      (or (looking-back "^[ \t]*/")
                          (and
                           (flora-in-mline-comment)
                           (eq (char-before) ?*)
                           (save-excursion
                             (flora-backward-char 1)
                             (skip-chars-backward "*")
                             (if (eq (char-before) ?/)
                                 (flora-backward-char 1))
                             (skip-chars-backward " \t")
                             (bolp)))))
                 ))
    (self-insert-command (prefix-numeric-value arg))
    (if indentp
	(flora-indent-line))
    ))

(defun flora-electric-rparen (arg)
  "Insert a parentheses, bracket, brace character.
If this char is indented with white space then move 1 position to the left."
  (interactive "*P")
  (self-insert-command (prefix-numeric-value arg))
  (let ((indentp (and (not arg)
                      (is-flora-electric-rparen-char)
                      (flora-looking-back "^[ \t\\]*[])}]"))
                 )
        )
    (if indentp
	(flora-indent-line)
      )
    ))

(defun is-flora-electric-rparen-char ()
  (or (eq last-command-event ?\))
      (eq last-command-event ?\])
      (eq last-command-event ?\})))

(defun flora-looking-at-electric-rparen-char ()
  (looking-at "})]"))

(defun flora-electric-slash (arg)
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
			;;     (not (flora-in-literal)))
			(and (eq ch ?*)
			     (flora-in-mline-comment)))
		       ))
	 )
    (self-insert-command (prefix-numeric-value arg))
    (if indentp
	(flora-indent-line))))


(defun flora-electric-punctuation (arg)
  "Insert a semicolon or other included punctuation.
If this char is indented with white space then move 1 position to the left."
  (interactive "*P")
  (self-insert-command (prefix-numeric-value arg))
  (let ((indentp (and (not arg)
                      (flora-is-electric-punctuation-char)
                      (flora-looking-back "^[ \t]*[;]"))
                 )
        )
    (if indentp
	(flora-indent-line)
      )
    ))
(defun flora-is-electric-punctuation-char ()
  (or (eq last-command-event ?\;)
      )
  )


(defun flora-in-literal ()
  ;; to be worked out
  nil)



;; If can skip into a comment then goes into the comment and then backs off out
;; of that comment -- just to whatever the end of code on that line is.
;; Otherwise, to end of line.
;; Seems not to understand when some-code1 /*...*/ some-code2 -- will go to the
;; end of some-code1.  Should be changed to work from end of line backwards.
(defun end-of-flora-code-line ()
  "Go to end of statement in this line."
  (beginning-of-line 1)
  (let* ((eolpos (flora-get-eol)))
    (if (re-search-forward comment-start-skip eolpos 'move)
        (goto-char (match-end 0)))
    (skip-chars-backward " \t/*")))

(defun flora-comment-indent ()
  "Compute Ergo style comment indentation."
  (cond ((looking-at "///") 0)
	((looking-at "//") (flora-indent-level))
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
(defun flora-get-beginning-of-clause-pos (&optional inpos)
  (let ((continue t)
        (result 1))
    (if (not inpos)
        (setq inpos (point)))
    ;; Bottleneck: calling this from within flora-inside-ergotext makes this
    ;; mode slow
    (save-excursion
      (goto-char inpos)
      (flora-skip-empty-backwards)
      (while (and continue (not (bobp)))
        (if (> (current-column) 0)
              (beginning-of-line)
          (if (or (flora-in-comment)
                  (looking-back "[\n\t ]"))
              (flora-skip-comments-and-whitespace-backwards))
          (if (looking-back "[\n\t ]")
              (skip-chars-backward "[\n\t ]"))
          (cond ((not (flora-right-at-dot (point)))
                 (flora-backward-char 1))
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
(defun flora-skip-empty-backwards (&optional skip-line-up)
  (let ((empty t)
        prev-pos)
    (while (and empty (not (eq prev-pos (point))))
      (setq prev-pos (point))
      (if skip-line-up
          (flora-backward-line 1))
      ;;(beginning-of-line)
      (if (bobp)
          (setq empty nil)
        (skip-chars-forward " \t")
        (if (not (or (looking-at "//") (looking-at "\n")))
            (setq empty nil))
        ))
    ))

(defun flora-get-back-match-beginning-keep-point (str &optional limit)
  (if (not limit)
      (setq limit (flora-get-beginning-of-clause-pos)))
  (save-excursion
    (if (re-search-backward str limit t)
        (match-beginning 0))))

(defun flora-previous-line-of-pos (pos)
  (save-excursion
    (goto-char pos)
    (flora-backward-line 1)
    (end-of-line)
    (point)))

;; cut down version of viper-paren-match, but is it sees ergotext then it
;; matches another ergotext delimiter, disregarding anything inside.
(defun flora-paren-match ()
  "Go to the matching parenthesis."
  (interactive)
  (let ((parse-sexp-ignore-comments t)
	anchor-point)
    (let (beg-lim end-lim)
      (if (and (eolp) (not (bolp)))
          (flora-backward-char 1))
      (if (not (looking-at "[][(){}]"))
          (setq anchor-point (point)))
      (setq beg-lim (point-at-bol)
            end-lim (point-at-eol))
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
               (and (looking-at "(") (looking-back "\\\\")))
           (re-search-forward "\\\\)" nil t)
           (backward-char 2))
          ((or (looking-at "\\\\)")
               (and (looking-at ")") (looking-back "\\\\")))
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
(defun flora-nearest-unclosed-open-paren (entry-point limit)
  (or entry-point (setq entry-point (point)))
  (save-excursion
    (goto-char entry-point)
    (flora-nearest-unclosed-open-paren-aux 0 limit)))

;; `count' counts the number of closed parentheses to match with the open ones
(defun flora-nearest-unclosed-open-paren-aux (count limit)
  ;;(message "count: %S point: %S char: %c"  count (point) (char-after))
  (let (pos)
    (cond ((flora-looking-back nearest-unclosed-paren limit)
           (setq pos (match-beginning 0))
           (goto-char pos)
           (cond ((flora-in-comment-ergotext-string-or-qatom pos)
                  (flora-nearest-unclosed-open-paren-aux count limit))
                 ((= count 0) pos)
                 (t
                  (flora-nearest-unclosed-open-paren-aux (1- count) limit)))
           )
          ((flora-looking-back nearest-closing-paren limit)
           (setq pos (match-beginning 0))
           (goto-char pos)
           (cond ((flora-in-comment-ergotext-string-or-qatom pos)
                  (flora-nearest-unclosed-open-paren-aux count limit))
                 (t
                  (flora-nearest-unclosed-open-paren-aux (1+ count) limit)))
          )
    )))


;; Intended to search for things like beginning of \if \then \else where
;; \if is a context string and \then or \else are trigger strings.
;; Similarly with \while-\do, \unless-\do, and \do-\until.
;; `end-str' here is a match for the ending of control statements of the form
;; ) \else and ) \until.
(defun flora-get-ctrl-stmt-indentation (trigger-str context-str end-str &optional pos limit)
  (or pos (setq pos (point)))
  (or limit (setq limit (nearest-preceding-flora-clause-delimiter pos)))

  (let (newpos newpos2 match-pos)
    (save-excursion
      (goto-char pos)
      (beginning-of-line)
      (cond (;; no extra indent if " \else" or " \until".
             ;; Also for (...-->> ... ;...)
             (looking-at-p end-str)
             (if (or (and (looking-at-p "[ \t]*\\\\else")
                          (flora-search-backwards-skip-comments-and-whitespace
                           "\\\\if" limit)
                          (setq match-pos (point)))
                     (and (looking-at-p "[ \t]*\\\\until")
                          (flora-search-backwards-skip-comments-and-whitespace
                           "\\\\do" limit)
                          (setq match-pos (point)))
                     (and (flora-search-backwards-skip-comments-and-whitespace
                           context-str limit)
                          (setq match-pos (point)))
                     (and (flora-search-backwards-skip-comments-and-whitespace
                           "\\\\do" limit)
                          (setq match-pos (point))))
                 (goto-char match-pos))
             (if (and (= (char-after pos) ?\))
                      (flora-looking-back "^[ \t]*" nil pos))
                 ;; looking at spaces then the )
                 (1+ (current-column))
               (current-column)))
            (;; the trigger string must be not on the same line
             (re-search-forward trigger-str pos t)
             nil)
            ((flora-skip-comments-and-whitespace-backwards-if-match
              (concat trigger-str "[ \t\n]*") limit)
             ;; pos is after trigger-str + space
             (or (flora-search-backwards-skip-comments-and-whitespace
                  context-str limit)
                 ;; \do is an exceptional context.
                 ;; We first search for the context of
                 ;; \if, \while, \unless and only IF THAT FAILS we assume that
                 ;; we are dealing with a \do-\while or \do-\until
                 (flora-search-backwards-skip-comments-and-whitespace
                  "\\\\do" limit))
             (+ flora-ctrl-stmt-shift (current-column)))
            ((and (setq newpos (flora-nearest-unclosed-open-paren pos limit))
                  (flora-sitting-in
                   "^[ \t]*)?[ \t]*\\\\else[ \t]*(?[ \t]*$" newpos))
             ;; we are inside the else-part of the statement
             (if (and (= (char-after pos) ?\))
                      (flora-looking-back "^[ \t]*" nil pos))
                 ;; looks like closing paren of \else(
                 (1+ (flora-indentation-level-of-line newpos))
               ;; probably inside \else (.....), not closing paren
               (+ flora-ctrl-stmt-shift 
                  (flora-indentation-level-of-line newpos)))
             )
            ((and ;; taking care of parentheses
              (setq newpos (flora-nearest-unclosed-open-paren pos limit))
              (goto-char newpos)
              (looking-at-p ".[ \t]*$")
              (looking-back (concat trigger-str "[ \t\n]*") limit 'greedy))
             (or (and
                  (not (flora-sitting-in "^[ \t]*)[ \t]*\\\\until" pos))
                  (re-search-backward context-str limit t)
                  )
                 ;; \do is an exceptional context.
                 ;; We first search for the context of
                 ;; \if, \while, \unless and only IF THAT FAILS we assume that
                 ;; we are dealing with a \do-\while or \do-\until
                 (re-search-backward "\\\\do"))
             ;; if looking at a closing ")" of control statement then do not
             ;; shift to the right by flora-ctrl-stmt-shift
             (if (and (= (char-after pos) ?\))
                      (flora-looking-back "^[ \t]*" nil pos))
                 (1+ (current-column))
               (+ flora-ctrl-stmt-shift (current-column)))
             )) ; cond
      ) ; save
    ) ; let
  )
         
(defun flora-indenting-connective (pos)
  (save-excursion
    (goto-char pos)
    (beginning-of-line)
    (looking-at-p (concat "[ \t]*" flora-infix-connective))))

(defun flora-backward-char (num)
  (setq num (min (1- (point)) num))
  (or (bobp)
      (forward-char (- num))))

(defun flora-backward-line (num)
  (or (bobp)
      (forward-line (- num))))

(defun flora-fill-paragraph (&optional arg)
  (interactive "P")
  (if (flora-in-mline-comment)
      (fill-paragraph arg)))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;
;;; Inferior Ergo mode
;;;
(defvar inferior-flora-mode-map nil)

(defun inferior-flora-mode ()
  "Major mode for interacting with an inferior Ergo process.

The following commands are available:
\\{inferior-flora-mode-map}

Entry to this mode calls the value of `inferior-flora-mode-hook' with no
arguments, if that value is non-nil.  Likewise with the value of
`comint-mode-hook'. 
`inferior-flora-mode-hook' is called after `comint-mode-hook'.

You can send text to the inferior flora from other buffers
using the commands \\[flora-load-buffer] \\[flora-load-file], and
\\[flora-load-region]. 

Return at end of buffer sends line as input.
Return not at end copies rest of line to end and sends it.
\\[comint-kill-input] and \\[backward-kill-word] are kill commands, imitating normal Unix input editing.
\\[comint-interrupt-subjob] interrupts the shell or its current subjob if any.
\\[comint-stop-subjob] stops. \\[comint-quit-subjob] sends quit signal."
  (interactive)
  (require 'comint)
  (comint-mode)
  (flora-mode-variables)
  ;;(setq major-mode 'inferior-ergo-mode
  (setq major-mode 'comint-mode
        mode-name  "Ergo process"
        comint-prompt-regexp "^ergo> *"
        comint-input-ring-file-name (expand-file-name "~/.ergo-history"))
  (if inferior-flora-mode-map 
      nil
    (setq inferior-flora-mode-map (copy-keymap comint-mode-map))
    (flora-mode-commands inferior-flora-mode-map)
    (define-key inferior-flora-mode-map "\M-\t" 'comint-dynamic-complete))
  (use-local-map inferior-flora-mode-map)
  (run-hooks 'inferior-flora-mode-hook)
  (or (file-exists-p comint-input-ring-file-name)
      (write-region 1 1 comint-input-ring-file-name))
  (comint-read-input-ring)
)

(defun run-flora-background ()
  "Run an Ergo process.
Input and output via buffer *ergo*."
  (if (not (get-process flora-process-name))
      (with-current-buffer (if ergo-command-line
			       (make-comint flora-process-name
					    ergo-program-name
					    nil
					    "-e"
					    ergo-command-line)
			     (make-comint
			      flora-process-name ergo-program-name))
	(inferior-flora-mode))))



;;;###autoload
(defun run-ergo ()
  "Run an Ergo process. Input and output via buffer *ergo*, and
switch to the buffer."
  (interactive)
  (run-flora-background)
  (show-flora-buffer 'switch))


(defun flora-load-region (&optional beg end module)
  "Send the region to the Ergo process.
The region must be created in advance."
  (interactive)
  (or (and beg end)
      (if (not (mark t))
	  (error "No region specified"))
      (setq beg (min (point) (mark t))
	    end (max (point) (mark t))))
  (let ((tmpfile-name (flora-make-temp-file beg end))
        command)
    (setq command 
          (format "load{'%s' >> %s}.\n" tmpfile-name (or module "main")))
    (run-flora-background)
    (save-excursion
      (process-send-string
       flora-process-name 
       command
       ))
    (show-flora-buffer)
    ))

(defun flora-send-region-as-query (&optional beg end)
  "Send the region to the Ergo process as a query.
The region must be a valid query terminated with a period."
  (interactive "r")
  (run-flora-background)
  (let ((query (buffer-substring-no-properties beg end)))
    (save-excursion
      (process-send-string flora-process-name query)
      (process-send-string flora-process-name "\n"))
    )
  (show-flora-buffer))

(defun flora-load-buffer (&optional module)
  "Send the current buffer to the Ergo process.
Does not offer to save files."
  (interactive)
  (let ((file (file-name-nondirectory (buffer-file-name)))
	flora-offer-save)
    (if file
	(progn
	  (setq file (concat flora-temp-file-prefix file))
	  (write-region (point-min) (point-max) file)
	  (flora-load-file file module))
      (flora-load-region 
       (point-min-marker) (point-max-marker) module))
    ))


(defun flora-load-file (&optional file module add)
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
    (if flora-offer-save
	(save-some-buffers))
    (run-flora-background)
    (if add
        (process-send-string
         flora-process-name
         (format "add{'%s' >> %s}.\n" file (or module "main")))
      (process-send-string
       flora-process-name
       (format "load{'%s' >> %s}.\n" file (or module "main"))))
    (show-flora-buffer)))

(defun flora-load-file-to-module ()
  "Prompt for a module into which to load the file. Then prompt for file."
  (interactive)
  (let (module)
    (setq module (flora-ask-module))
    (flora-load-file nil module)))

(defun flora-load-buffer-to-module ()
  "Prompt for a module into which to load the file."
  (interactive)
  (let (module)
    (setq module (flora-ask-module))
    (flora-load-buffer module)))

(defun flora-load-region-to-module ()
  "Prompt for a module into which to load the region."
  (interactive)
  (let (module)
    (setq module (flora-ask-module))
    (flora-load-region nil nil module)))

(defun flora-add-file-to-module ()
  "Prompt for a module to which to add the file. Then prompt for file."
  (interactive)
  (let (module)
    (setq module (flora-ask-module-add))
    (flora-load-file nil module)))

(defun flora-add-buffer-to-module ()
  "Prompt for a module to which to add the file."
  (interactive)
  (let (module)
    (setq module (flora-ask-module-add))
    (flora-load-buffer module)))

(defun flora-add-region-to-module ()
  "Prompt for a module to which to add the region."
  (interactive)
  (let (module)
    (setq module (flora-ask-module-add))
    (flora-load-region nil nil module)))

(defun flora-ask-module ()
  (read-string "Module to load to (main): " nil
               'flora-module-load-history "main"))
(defun flora-ask-module-add ()
  (read-string "Module to add to (main): " nil
               'flora-module-load-history "main"))

(defun ergo-interrupt()
  (interactive)
  (interrupt-process flora-process-name))

(defun ergo-quit()
  (interactive)
  (quit-process flora-process-name))

(defun ergo-restart ()
  (interactive)
  (run-flora-background)
  (process-send-string flora-process-name flora-forget-string)
  (sit-for 2)
  (run-ergo)
  (sit-for 0))  ;; synchronize

(defun flora-switch-to-flora-buffer ()
  (interactive)
  (run-flora-background)
  (pop-to-buffer flora-process-buffer))

;; SWITCH means switch to the inferior Ergo buffer
(defun show-flora-buffer (&optional switch)
  (let ((wind (selected-window)))
    (with-temp-buffer
      (sit-for 1))
      (set-buffer flora-process-buffer)
      (or (flora-get-visible-buffer-window flora-process-buffer)
	  (progn
	    (display-buffer flora-process-buffer)
	    (switch-to-buffer-other-window flora-process-buffer)))
      ;; time is needed for XSB to return. otherwise, the point will be off
      (goto-char (point-max))
      (or switch
	  (select-window wind))))

(defun flora-get-visible-buffer-window (buff)
  (if (flora-buffer-live-p buff)
      (if flora-xemacs-p
	  (get-buffer-window buff t)
	(get-buffer-window buff 'visible))))


(defun flora-make-temp-file (start end)
  (let* ((f (make-temp-name (concat flora-temp-file-prefix "ergo"))))
    ;; create the file
    (write-region start end
		  (concat f ".ergo")
		  nil          ; don't append---erase
		  'no-message) 
    (expand-file-name f)))


(provide 'ergo)


;;; ergo.el ends here

