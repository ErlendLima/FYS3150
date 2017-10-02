(TeX-add-style-hook
 "rapport"
 (lambda ()
   (TeX-add-to-alist 'LaTeX-provided-class-options
                     '(("revtex4-1" "aps" "reprint")))
   (TeX-add-to-alist 'LaTeX-provided-package-options
                     '(("fontenc" "T1") ("inputenc" "utf8") ("babel" "english") ("xcolor" "usenames" "dvipsnames" "svgnames" "table") ("caption" "font={scriptsize}") ("listings" "procnames") ("algpseudocode" "noend")))
   (add-to-list 'LaTeX-verbatim-environments-local "lstlisting")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "path")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "url")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "lstinline")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "nolinkurl")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "hyperbaseurl")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "hyperimage")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "hyperref")
   (add-to-list 'LaTeX-verbatim-macros-with-delims-local "path")
   (add-to-list 'LaTeX-verbatim-macros-with-delims-local "lstinline")
   (TeX-run-style-hooks
    "latex2e"
    "revtex4-1"
    "revtex4-110"
    "fontenc"
    "inputenc"
    "fontspec"
    "babel"
    "csquotes"
    "url"
    "textcomp"
    "xcolor"
    "caption"
    "amsmath"
    "amsthm"
    "amsfonts"
    "amssymb"
    "enumerate"
    "tikz"
    "float"
    "listings"
    "pstool"
    "pgfplots"
    "wrapfig"
    "graphicx"
    "epstopdf"
    "afterpage"
    "physics"
    "multirow"
    "gensymb"
    "algorithm"
    "microtype"
    "algpseudocode"
    "colortbl"
    "geometry"
    "hyperref"
    "subcaption"
    "lipsum"
    "nth"
    "siunitx")
   (TeX-add-symbols
    '("nonref" 1)
    '("noref" 1)
    '("unit" 1)
    "blankpage"
    "numberthis")
   (LaTeX-add-labels
    "sec:introduction"
    "sec:theory"
    "sec:harmonic"
    "eq:schroedinger"
    "eq:scaledschroedinger"
    "sec:method"
    "sec:results"
    "sec:algorithm"
    "fig:juliatiming"
    "fig:juliaiterations"
    "sec:hamiltonsol"
    "sec:discussion"
    "sec:conclusion")
   (LaTeX-add-bibliographies
    "references")
   (LaTeX-add-xcolor-definecolors
    "dkgreen"
    "dred"
    "dblue"
    "lgrey"
    "gray"
    "darkblue")
   (LaTeX-add-caption-DeclareCaptions
    '("\\DeclareCaptionFont{white}" "Font" "white")
    '("\\DeclareCaptionFormat{listing}" "Format" "listing")))
 :latex)

