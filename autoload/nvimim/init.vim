
" Global options
let g:nvimim_auto_write = get(g:, 'nvimim_auto_write', 0)
let g:nvimim_safe_remove = get(g:, 'nvimim_safe_remove', 1)

" 0: none, 1: top, 2: bottom
let g:nvimim_filer_position = get(g:, 'nvimim_filer_position', 'right')
let g:nvimim_filer_width = get(g:, 'nvimim_filer_width', 40)
let g:nvimim_filer_directory_sort = get(g:, 'nvimim_filer_directory_first', 1)
let g:nvimim_filer_close_children = get(g:, 'nvimim_filer_close_children', 1)
let g:nvimim_filer_tree_opened_icon = get(g:, 'nvimim_filer_tree_opened_icon', '')
let g:nvimim_filer_tree_closed_icon = get(g:, 'nvimim_filer_tree_closed_icon', '')
let g:nvimim_filer_file_icon = get(g:, 'nvimim_filer_file_icon', '-')
let g:nvimim_filer_privfile_icon = get(g:, 'nvimim_filer_privfile_icon', 'x')
let g:nvimim_filer_show_hidden = get(g:, 'nvimim_filer_show_hidden', 0)

let g:nvimim_sptfy_client_id = ''
let g:nvimim_sptfy_client_secret = ''

function nvimim#init#init()
  call nvimim#init()
  call nvimim#filer#init()
  " call nvimim#sptfy#init()
endfunction
