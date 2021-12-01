if exists('b:current_syntax')
  finish
endif

syntax match filerIndicator '^\s*..'
syntax match filerDir '.*/$' contains=filerIndicator

hi def link filerIndicator Operator
hi def link filerDir Identifier

let b:current_syntax = 'filertree'
