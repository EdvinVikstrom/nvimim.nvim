" return: string
function nvimim#util#get_working_dir()
  return expand('$PWD')
endfunction

" params: ['name': string]
" return: string
function nvimim#util#realpath(name)
  return trim(system('realpath "'.a:name.'"'))
endfunction

" params: ['path': string]
" return: string
function nvimim#util#basename(path)
  let last = strlen(a:path)
  while last >= 0
    if a:path[last] == '/'
      break
    endif
    let last -= 1
  endwhile
  let last += 1
  return strpart(a:path, last, strlen(a:path) - last)
endfunction

" params: ['path': string]
" return: string
function nvimim#util#parent(path)
  let parent = strpart(a:path, 0, strlen(a:path) - strlen(nvimim#util#basename(a:path)))
  if empty(parent)
    return '/'
  endif
  return nvimim#util#realpath(parent)
endfunction

" params: ['path': string]
" return: boolean
function nvimim#util#is_hidden(path)
  let name = nvimim#util#basename(a:path)
  return strlen(name) != 0 ? name[0] == '.' : 0
endfunction

" params: ['path': string]
" return: string
function nvimim#util#extension(path)
  let dot = -1

  let index = strlen(a:path) - 1
  while index >= 0
    if a:path[index] == '.'
      let dot = index
      break
    endif
    let index -= 1
  endwhile

  if dot != -1
    return strpart(a:path, dot + 1, strlen(a:path) - dot)
  endif
  return ''
endfunction

" params: ['parent': string, 'name': string]
" return: string
function nvimim#util#child(parent, name)
  if a:parent[strlen(a:parent) - 1] == '/'
    return a:parent.a:name
  endif
  return a:parent.'/'.a:name
endfunction

" params: ['path': string, 'name': string]
" return: string
function nvimim#util#rename(path, name)
  return nvimim#util#child(nvimim#util#parent(a:path), a:name)
endfunction

function nvimim#util#open_url(url)
  let ret = system('firefox --new-tab "'.a:url.'"')
endfunction

function nvimim#util#decode_uri(uri)
  let uri_len = len(a:uri)
  let endpoint_len = uri_len

  let index = 0
  while index != uri_len
    if a:uri[index] == '?'
      let endpoint_len = index
      break
    endif
    let index += 1
  endwhile
  let index += 1

  let params = []

  let start = index
  while index < uri_len
    if a:uri[index] == '='
      call add(params, #{key: strpart(a:uri, start, index - start), value: ''})

      let index += 1
      let start = index
      while index < uri_len
	if a:uri[index] == '&'
	  break
	endif
	let index += 1
      endwhile

      let params[-1].value = strpart(a:uri, start, index - start)
      let index += 1
    endif
  endwhile

  return #{endpoint: strpart(a:uri, 0, endpoint_len), params: params}
endfunction
