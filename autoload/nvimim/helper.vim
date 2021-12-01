" params: ['text': string]
" return: list<string>
function nvimim#helper#input_list(text)
  let str = input(a:text." (comma separated): ")
  return split(str, ',')
endfunction

" params: ['text': string, 'def': int]
" return: boolean
function nvimim#helper#input_yesno(text, def)
  if a:def == 1
    let str = input(#{prompt: a:text." [Y/n]: ", cancelreturn: 'N'})
    return str ==? 'N' ? 0 : 1
  elseif a:def == -1
    let str = input(#{prompt: a:text." [y/N]: ", cancelreturn: 'N'})
    return str ==? 'Y' ? 1 : 0
  endif

  while 1
    let str = input(#{prompt: a:text." [y/n]: ", cancelreturn: 'N'})
    if str ==? 'Y'
      return 1
    elseif str ==? 'N'
      return 0
    endif
    continue
  endwhile
endfunction

function nvimim#helper#input(text, val = '', def = '')
  let str = input(#{prompt: a:text.": ",  default: a:val, cancelreturn: a:def})
  return str
endfunction

" params: ['text': string]
function nvimim#helper#input_wait(text)
  let res = input(a:text)
endfunction
