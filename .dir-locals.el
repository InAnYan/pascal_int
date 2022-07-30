((nil
  .
  ((eval
    .
    (let ((root (locate-dominating-file default-directory ".dir-locals.el")))
      (setq company-clang-arguments
            (list
             "-I" (expand-file-name "include" root)))   )))))
