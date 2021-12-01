" params: ['dir': file]
" return: node
function nvimim#filer#tree#create(dir, parent = {})
  let tree = nvimim#filer#node#create(a:dir, a:parent)
  call nvimim#filer#tree#reload(tree)
  return tree
endfunction

" params: ['tree': node, 'node': node]
function nvimim#filer#tree#add(tree, node)
  let a:node.parent = a:tree
  call add(a:tree.children, a:node)
endfunction

function nvimim#filer#tree#remove(tree, path)
  call filter(a:tree.children, 'v:val.path != a:path')
endfunction

" params: ['tree': node, 'file': file]
function nvimim#filer#tree#add_file(tree, file)
  call add(a:tree.children, nvimim#filer#node#create(a:file, a:tree))
endfunction

" params: ['tree': node, 'path': string]
function nvimim#filer#tree#remove_file(tree, path)
  let index = 0
  while index != len(a:tree.children)
    if a:tree.children[index].path == a:path
      call remove(a:tree.children, index)
      break
    endif
    let index += 1
  endwhile
endfunction

" params: ['tree': node]
function nvimim#filer#tree#expand(tree)
  let a:tree.expanded = 1
  call nvimim#filer#tree#reload(a:tree)
endfunction

" params: ['tree': node, 'close_children': boolean]
function nvimim#filer#tree#close(tree, close_children)
  let a:tree.expanded = 0
  if a:close_children
    for child in a:tree.children
      call nvimim#filer#tree#close(child, a:close_children)
    endfor
  endif
endfunction

" params: ['tree': node]
function nvimim#filer#tree#reload(tree)
  let a:tree.loaded = 1

  let files = split(glob(a:tree.path.'/*'))
  let files = files + split(glob(a:tree.path.'/.*'))

  " add or update files
  for path in files
    let name = nvimim#util#basename(path)
    let node = nvimim#filer#node#create(path, a:tree)

    let index = nvimim#filer#tree#find(a:tree, node.path)

    " if tree does contain item
    if index != -1
      let child = a:tree.children[index]
      if child.loaded && node.time > child.time
	call nvimim#filer#tree#reload(child)
      endif
    else
      call add(a:tree.children, node)
    endif
  endfor

  " remove files
  call filter(a:tree.children, 'index(files, v:val.path) != -1')
endfunction

" params: ['tree': node, 'path': string]
" return: int
function nvimim#filer#tree#find(tree, path)
  let index = 0
  for item in a:tree.children
    if item.path == a:path
      return index
    endif
    let index += 1
  endfor
  return -1
endfunction

" params: ['tree': node, 'conf': int]
function nvimim#filer#tree#sort(tree, conf)
  if a:conf == 1
    call sort(a:tree.children, function('nvimim#filer#tree#sort_callback2'))
  elseif a:conf == 2
    call sort(a:tree.children, function('nvimim#filer#tree#sort_callback3'))
  else
    call sort(a:tree.children, function('nvimim#filer#tree#sort_callback1'))
  endif
endfunction

" Sort alphabetically
function nvimim#filer#tree#sort_callback1(node1, node2)
  let path1 = toupper(a:node1.path)
  let path2 = toupper(a:node2.path)

  if path1 < path2
    return -1
  elseif path1 > path2
    return 1
  endif
  return 0
endfunction

" Sort alphabetically with directories on top
function nvimim#filer#tree#sort_callback2(node1, node2)
  if a:node1.type == 'dir' && a:node2.type != 'dir'
    return -1
  elseif a:node1.type != 'dir' && a:node2.type == 'dir'
    return 1
  endif
  return nvimim#filer#tree#sort_callback1(a:node1, a:node2)
endfunction

" Sort alphabetically with directories on bottom
function nvimim#filer#tree#sort_callback3(node1, node2)
  if a:node1.type == 'dir' && a:node2.type != 'dir'
    return 1
  elseif a:node1.type != 'dir' && a:node2.type == 'dir'
    return -1
  endif
  return nvimim#filer#tree#sort_callback1(a:node1, a:node2)
endfunction
