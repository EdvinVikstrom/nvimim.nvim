function nvimim#file#create(path)
  return #{path: a:path, name: nvimim#util#basename(a:path), type: getftype(a:path), time: getftime(a:path), hidden: nvimim#util#is_hidden(a:path)}
endfunction

" params: ['files': List(file), 'file': file]
" return: int
function nvimim#file#find(files, file)
  let index = 0
  for item in a:files
    if nvimim#file#equals(item, a:file)
      return index
    endif
    let index += 1
  endfor
  return -1
endfunction

" params: ['file1': file, 'file2': file]
" return: boolean
function nvimim#file#equals(file1, file2)
  return a:file1['path'] == a:file2['path'] && a:file1['type'] == a:file2['type']
endfunction

let g:nvimim#file#c_exts = ['\V\..c', '\V\..h']
let g:nvimim#file#cpp_exts = ['\V\..cpp', '\V\..cxx', '\V\..cc', '\V\..hpp', '\V\..hxx', '\V\..hh']
let g:nvimim#file#py_exts = ['\V\..py', '\V\..python']
let g:nvimim#file#js_exts = ['\V\..js', '\V\..javascript']
let g:nvimim#file#ruby_exts = ['\V\..rb', '\V\..ruby', '\V\..gem']
let g:nvimim#file#rust_exts = ['\V\..rs', '\V\..rust', 'Cargo.toml']
let g:nvimim#file#lua_exts = ['\V\..lua']
let g:nvimim#file#java_exts = ['\V\..java']
let g:nvimim#file#php_exts = ['\V\..php']
let g:nvimim#file#vim_exts = ['\V\..vim', '\V\..nvim']
let g:nvimim#file#sh_exts = ['\V\..sh', '\V\..zsh', '\V\..bash']

let g:nvimim#file#html_exts = ['\V\..html', '\V\..htm']
let g:nvimim#file#json_exts = ['\V\..json']
let g:nvimim#file#md_exts = ['\V\..md', '\V\..markdown']

let g:nvimim#file#make_exts = ['GNUmakefile', 'makefile', 'Makefile']
let g:nvimim#file#cmake_exts = ['\V\..cmake', 'CMakeLists.txt']
let g:nvimim#file#txt_exts = ['\V\..txt']
let g:nvimim#file#conf_exts = ['\V\..ini', '\V\..conf']

" params: ['ext': string]
" return: char
function nvimim#file#find_icon(name)
  if nvimim#file#match_ext(a:name, g:nvimim#file#c_exts)
    return ''
  elseif nvimim#file#match_ext(a:name, g:nvimim#file#cpp_exts)
    return ''
  elseif nvimim#file#match_ext(a:name, g:nvimim#file#py_exts)
    return ''
  elseif nvimim#file#match_ext(a:name, g:nvimim#file#js_exts)
    return ''
  elseif nvimim#file#match_ext(a:name, g:nvimim#file#ruby_exts)
    return ''
  elseif nvimim#file#match_ext(a:name, g:nvimim#file#rust_exts)
    return ''
  elseif nvimim#file#match_ext(a:name, g:nvimim#file#lua_exts)
    return ''
  elseif nvimim#file#match_ext(a:name, g:nvimim#file#java_exts)
    return ''
  elseif nvimim#file#match_ext(a:name, g:nvimim#file#php_exts)
    return ''
  elseif nvimim#file#match_ext(a:name, g:nvimim#file#vim_exts)
    return ''
  elseif nvimim#file#match_ext(a:name, g:nvimim#file#sh_exts)
    return ''
  elseif nvimim#file#match_ext(a:name, g:nvimim#file#html_exts)
    return ''
  elseif nvimim#file#match_ext(a:name, g:nvimim#file#json_exts)
    return ''
  elseif nvimim#file#match_ext(a:name, g:nvimim#file#md_exts)
    return ''
  elseif nvimim#file#match_ext(a:name, g:nvimim#file#make_exts)
    return ''
  elseif nvimim#file#match_ext(a:name, g:nvimim#file#cmake_exts)
    return ''
  elseif nvimim#file#match_ext(a:name, g:nvimim#file#txt_exts)
    return ''
  elseif nvimim#file#match_ext(a:name, g:nvimim#file#conf_exts)
    return ''
  endif
  return ' '
endfunction

function nvimim#file#match_ext(name, ext)
  for item in a:ext
    if a:name =~? item
      return 1
    endif
  endfor
  return 0
endfunction
