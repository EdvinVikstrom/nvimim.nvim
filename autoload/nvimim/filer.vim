function nvimim#filer#init()
  call nvimim#filer#init_variables()
  call nvimim#filer#init_window()
  call nvimim#filer#init_buffer()
  call nvimim#filer#init_autocmds()
  call nvimim#filer#input#init()

  call nvimim#filer#load_directory(nvimim#util#get_working_dir())
endfunction

function nvimim#filer#destroy()
  if exists('g:nvimim#filer#winid')
    unlet g:nvimim#filer#winid
  endif
endfunction

function nvimim#filer#init_variables()
endfunction

function nvimim#filer#init_window()
  vnew
  let g:nvimim#filer#winnr = 1
  let g:nvimim#filer#winid = win_getid(g:nvimim#filer#winnr)
  call nvim_win_set_width(g:nvimim#filer#winid, g:nvimim_filer_width)

  if g:nvimim_filer_position ==? 'right'
    execute g:nvimim#filer#winnr.'wincmd r'
  endif
endfunction

function nvimim#filer#init_buffer()
  let g:nvimim#filer#bufname = 'filer'
  let g:nvimim#filer#buffer = bufadd(g:nvimim#filer#bufname)
  let g:nvimim#filer#bufnr = bufnr(g:nvimim#filer#buffer)
  call nvim_buf_set_option(g:nvimim#filer#bufnr, 'modifiable', v:false)
  call nvim_buf_set_option(g:nvimim#filer#bufnr, 'buflisted', v:false)
  call nvim_buf_set_option(g:nvimim#filer#bufnr, 'buftype', 'nofile')
  call nvim_buf_set_option(g:nvimim#filer#bufnr, 'syntax', 'filer')
  call nvim_win_set_buf(g:nvimim#filer#winid, g:nvimim#filer#buffer)
endfunction

function nvimim#filer#init_autocmds()
  augroup nvimim
    autocmd DirChanged * call nvimim#filer#load_directory(nvimim#util#get_working_dir())
  augroup END
endfunction

function nvimim#filer#load_directory(dir)
  let g:nvimim#filer#directory = a:dir
  let g:nvimim#filer#tree = nvimim#filer#tree#create(a:dir)
  let g:nvimim#filer#tree_view = nvimim#filer#view#create(g:nvimim#filer#bufnr, g:nvimim#filer#winid)

  call nvimim#filer#tree#sort(g:nvimim#filer#tree, g:nvimim_filer_directory_sort)
  call nvimim#filer#view#redraw(g:nvimim#filer#tree_view, g:nvimim#filer#tree)
  call nvimim#filer#view#select(g:nvimim#filer#tree_view, 0)
endfunction
