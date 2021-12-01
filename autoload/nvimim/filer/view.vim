" params: ['bufnr': int]
" return: view
function nvimim#filer#view#create(buf, win)
  return #{buf: a:buf, win: a:win, offset: 1, marking: 0, marked: [], nodes: []}
endfunction

" params: ['index': int, 'node': node, 'parent': node_view, 'nest': int', line': string]
" return: node_view
function nvimim#filer#view#create_node(index, node, parent, nest, line)
  return #{index: a:index, node: a:node, parent: a:parent, nest: a:nest, line: a:line, marked: 0}
endfunction
"
" params: ['view': view, 'tree': node]
function nvimim#filer#view#redraw(view, tree)
  let a:view.nodes = []
  call nvimim#filer#view#render(a:view, a:tree, {}, 0)
  call nvimim#filer#view#draw(a:view)
endfunction

" params: ['view': view, 'tree': node]
function nvimim#filer#view#draw(view)
  let lines = []
  for node in a:view.nodes
    if node.marked
      call add(lines, node.line.' *')
    else
      call add(lines, node.line)
    endif
  endfor

  call nvim_buf_set_option(a:view.buf, 'modifiable', v:true)
  call nvim_buf_set_lines(a:view.buf, 0, -1, 0, lines)
  call nvim_buf_set_option(a:view.buf, 'modifiable', v:false)
endfunction

" params: ['view': view, 'tree': node, 'parent': node_view, 'nest': int]
function nvimim#filer#view#render(view, tree, parent, nest)
  if g:nvimim_filer_show_hidden
    for child in a:tree.children
      call nvimim#filer#view#render_node(a:view, child, a:parent, a:nest)
    endfor
  else
    for child in a:tree.children
      if child.hidden
	continue
      endif
      call nvimim#filer#view#render_node(a:view, child, a:parent, a:nest)
    endfor
  endif
endfunction

" params: ['view': view, 'node': node, 'parent': node_view, 'nest': int]
function nvimim#filer#view#render_node(view, node, parent, nest)
  let line = repeat(' ', a:nest)

  if a:node.type == 'dir'
    call nvimim#filer#view#render_dir_node(a:view, a:node, a:parent, a:nest, line)
  else
    call nvimim#filer#view#render_file_node(a:view, a:node, a:parent, a:nest, line)
  endif
endfunction

" params: ['view': view, 'node': node]
function nvimim#filer#view#render_dir_node(view, node, parent, nest, line)
  if a:node.expanded
    let line = a:line.g:nvimim_filer_tree_opened_icon.' '.a:node.name.'/'
    call add(a:view.nodes, nvimim#filer#view#create_node(len(a:view.nodes), a:node, a:parent, a:nest, line))
    call nvimim#filer#view#render(a:view, a:node, a:view.nodes[-1], a:nest + 1)
  else
    let line = a:line.g:nvimim_filer_tree_closed_icon.' '.a:node.name.'/'
    call add(a:view.nodes, nvimim#filer#view#create_node(len(a:view.nodes), a:node, a:parent, a:nest, line))
  endif
endfunction

" params: ['view': view, 'node': node]
function nvimim#filer#view#render_file_node(view, node, parent, nest, line)
  if a:node.perm == 3
    let line = a:line.g:nvimim_filer_file_icon.' '.a:node.name
  else
    let line = a:line.g:nvimim_filer_privfile_icon.' '.a:node.name
  endif
  call add(a:view.nodes, nvimim#filer#view#create_node(len(a:view.nodes), a:node, a:parent, a:nest, line))
endfunction

" params: ['view': view]
" return: node_view
function nvimim#filer#view#selected(view)
  let linenr = line('.')
  let index = linenr - a:view.offset
  return a:view.nodes[index]
endfunction

" params: ['view': view, 'index': int]
function nvimim#filer#view#select(view, index)
  call cursor(a:view.offset + a:index, 0)
endfunction

" params: ['view': view]
" return: List<node_view>
function nvimim#filer#view#marked(view)
  return a:view.marked
endfunction

" params: ['view': view, 'index': int]
function nvimim#filer#view#mark(view, index)
  let node = a:view.nodes[a:index]
  if node.marked
    let node.marked = 0
    call remove(a:view.marked, index(a:view.marked, node))
  else
    let node.marked = 1
    call add(a:view.marked, node)
  endif
  call nvimim#filer#view#draw(a:view)
endfunction
