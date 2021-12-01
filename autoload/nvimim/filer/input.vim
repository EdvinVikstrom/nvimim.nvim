function nvimim#filer#input#init()
  call nvim_buf_set_keymap(g:nvimim#filer#bufnr, 'n', '<Enter>', ':call nvimim#filer#input#open()<CR>', #{silent: v:true})
  call nvim_buf_set_keymap(g:nvimim#filer#bufnr, 'n', 'l', ':call nvimim#filer#input#expand()<CR>', #{silent: v:true})
  call nvim_buf_set_keymap(g:nvimim#filer#bufnr, 'n', 'h', ':call nvimim#filer#input#close()<CR>', #{silent: v:true})
  call nvim_buf_set_keymap(g:nvimim#filer#bufnr, 'n', 'K', ':call nvimim#filer#input#make_directory()<CR>', #{silent: v:true})
  call nvim_buf_set_keymap(g:nvimim#filer#bufnr, 'n', 'N', ':call nvimim#filer#input#make_file()<CR>', #{silent: v:true})
  call nvim_buf_set_keymap(g:nvimim#filer#bufnr, 'n', 'r', ':call nvimim#filer#input#rename()<CR>', #{silent: v:true})
  call nvim_buf_set_keymap(g:nvimim#filer#bufnr, 'n', 'x', ':call nvimim#filer#input#delete()<CR>', #{silent: v:true})
  call nvim_buf_set_keymap(g:nvimim#filer#bufnr, 'n', 'm', ':call nvimim#filer#input#mark()<CR>', #{silent: v:true})
  call nvim_buf_set_keymap(g:nvimim#filer#bufnr, 'n', '.', ':call nvimim#filer#input#toggle_hidden()<CR>', #{silent: v:true})
endfunction

function nvimim#filer#input#open()
  let selected = nvimim#filer#view#selected(g:nvimim#filer#tree_view)

  if selected.node.type == 'dir'
    call nvimim#filer#load_directory(selected.node.path)
  else
    call nvimim#open_file(selected.node.path)
  endif
endfunction

function nvimim#filer#input#expand()
  let selected = nvimim#filer#view#selected(g:nvimim#filer#tree_view)
  let linenr = line('.')

  if selected.node.type == 'dir'
    if selected.node.expanded
      call nvimim#filer#tree#close(selected.node, g:nvimim_filer_close_children)
      call nvimim#filer#view#redraw(g:nvimim#filer#tree_view, g:nvimim#filer#tree)
    else
      call nvimim#filer#tree#expand(selected.node)
      call nvimim#filer#tree#sort(selected.node, g:nvimim_filer_directory_sort)
      call nvimim#filer#view#redraw(g:nvimim#filer#tree_view, g:nvimim#filer#tree)
      call cursor(linenr + 1, 0)
    endif
  else
    call nvimim#open_file(selected.node.path)
  endif
endfunction

function nvimim#filer#input#close()
  let selected = nvimim#filer#view#selected(g:nvimim#filer#tree_view)

  if selected.node.expanded
    call nvimim#filer#tree#close(selected.node, g:nvimim_filer_close_children)
    call nvimim#filer#view#redraw(g:nvimim#filer#tree_view, g:nvimim#filer#tree)
  else
    if !empty(selected.parent)
      call nvimim#filer#view#select(g:nvimim#filer#tree_view, selected.parent.index)
      call nvimim#filer#tree#close(selected.node.parent, g:nvimim_filer_close_children)
      call nvimim#filer#view#redraw(g:nvimim#filer#tree_view, g:nvimim#filer#tree)
    else
      call nvimim#filer#load_directory(nvimim#util#parent(selected.node.parent.path))
    endif
  endif
endfunction

function nvimim#filer#input#make_directory()
  let selected = nvimim#filer#view#selected(g:nvimim#filer#tree_view)
  
  if selected.node.expanded
    let node = selected.node
  else
    let node = selected.node.parent
  endif

  let names = nvimim#helper#input_list('New directory names')
  for name in names
    let path = nvimim#util#child(node.path., name)
    if isdirectory(path)
      echoerr 'directory already exists ['.path.']'
      continue
    endif
    call mkdir(path, 'p')
  endfor

  call nvimim#filer#tree#reload(node)
  call nvimim#filer#tree#sort(node, g:nvimim_filer_directory_sort)
  call nvimim#filer#view#redraw(g:nvimim#filer#tree_view, g:nvimim#filer#tree)
endfunction

function nvimim#filer#input#make_file()
  let selected = nvimim#filer#view#selected(g:nvimim#filer#tree_view)
  
  if selected.node.expanded
    let node = selected.node
  else
    let node = selected.node.parent
  endif

  let names = nvimim#helper#input_list('New file names')
  for name in names
    let path = nvimim#util#child(node.path, name)
    if filereadable(path)
      echoerr 'file already exists ['.path.']'
      continue
    endif

    " if !filewritable(path)
    "   echo 'cannot write file ['.path.']'
    "   continue
    " endif

    call writefile([], path, 'b')
  endfor

  call nvimim#filer#tree#reload(node)
  call nvimim#filer#tree#sort(node, g:nvimim_filer_directory_sort)
  call nvimim#filer#view#redraw(g:nvimim#filer#tree_view, g:nvimim#filer#tree)
endfunction

function nvimim#filer#input#rename()
  let selected = nvimim#filer#view#selected(g:nvimim#filer#tree_view)

  let name = nvimim#helper#input('Enter new name', selected.node.name)
  if empty(name)
    return
  endif

  let path = nvimim#util#rename(selected.node.path, name)

  if filereadable(path) || isdirectory(path)
    echoerr 'file already exists ['.path.']'
    return
  endif

  call rename(selected.node.path, path)

  call nvimim#filer#tree#reload(selected.node.parent)
  call nvimim#filer#tree#sort(selected.node.parent, g:nvimim_filer_directory_sort)
  call nvimim#filer#view#redraw(g:nvimim#filer#tree_view, g:nvimim#filer#tree)
endfunction

function nvimim#filer#input#delete()
  if empty(nvimim#filer#view#marked(g:nvimim#filer#tree_view))
    let items = [nvimim#filer#view#selected(g:nvimim#filer#tree_view)]
  else
    let items = nvimim#filer#view#marked(g:nvimim#filer#tree_view)
  endif

  let todelete = []

  for item in items

    if item.node.type == 'dir'
      if !isdirectory(item.node.path)
	echoerr 'directory does not exists ['.item.node.path.']'
	continue
      endif
    elseif !filereadable(item.node.path)
      echoerr 'file does not exists ['.item.node.path.']'
      continue
    endif

    call add(todelete, item.node)
  endfor

  if len(todelete) == 0
    return
  endif

  if !nvimim#helper#input_yesno('Sure you want to remove '.len(todelete).' file(s)?', -1)
    return
  endif

  let deleted = 0
  for item in todelete
    call nvimim#safe_remove(item.path)
    call nvimim#filer#tree#remove(item.parent, item.path)
    let deleted += 1
  endfor
  echo deleted.' file(s) removed.'
  call nvimim#filer#view#redraw(g:nvimim#filer#tree_view, g:nvimim#filer#tree)
endfunction

function nvimim#filer#input#mark()
  let selected = nvimim#filer#view#selected(g:nvimim#filer#tree_view)

  call nvimim#filer#view#mark(g:nvimim#filer#tree_view, selected.index)
endfunction

function nvimim#filer#input#toggle_hidden()
  let g:nvimim_filer_show_hidden = 1 - g:nvimim_filer_show_hidden
  call nvimim#filer#view#redraw(g:nvimim#filer#tree_view, g:nvimim#filer#tree)
endfunction
