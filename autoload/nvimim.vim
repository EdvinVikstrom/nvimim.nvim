function nvimim#init()
  let g:nvimim#main_window = nvim_get_current_win()

  augroup nvimim
    " let g:nvimim#id = jobstart(['./nvimxx'], {'rpc': v:true})

    " autocmd TextChanged * call nvimim#event_text_changed()
    " autocmd TextChangedI * call nvimim#event_text_changed()

    autocmd WinClosed * call nvimim#event_window_closed(expand('<afile>'))
    autocmd FileWriteCmd * quitall
  augroup END
endfunction

function nvimim#open_file(file)
  let bufnr = winbufnr(g:nvimim#main_window)
  if nvim_buf_get_option(bufnr, 'modified')
    if g:nvimim#auto_write
      call nvim_command(':write')
    else
      call nvim_err_writeln('file not saved')
      return
    endif
  endif

  call nvim_set_current_win(g:nvimim#main_window)
  call nvim_command(':edit '.a:file)
endfunction

function nvimim#safe_remove(file)
  if g:nvimim_safe_remove
    echo 'removed '.a:file
  else
    call delete(a:file, 'fd')
  endif
endfunction

" Events
function nvimim#event_window_closed(win)
  if a:win == g:nvimim#main_window
    call nvimim#filer#destroy()
    quitall
  endif
endfunction
