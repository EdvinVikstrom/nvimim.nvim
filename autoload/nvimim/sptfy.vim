function nvimim#sptfy#init()
  let g:nvimim#sptfy#url = 'https://accounts.spotify.com/api/token'

  call nvimim#sptfy#authorize()

  " echo 'auth: '.string(g:nvimim#sptfy#token)
  " echo 'user: '.string(nvimim#sptfy#request('me'))
endfunction

function nvimim#sptfy#init_token()
  let url = 'https://accounts.spotify.com/api/token'

  let raw = system('curl -s '.url.' '.
	\ '-X POST '.
	\ '--basic '.
	\ '--user '.g:nvimim_sptfy_client_id.':'.g:nvimim_sptfy_client_secret.' '.
	\ '-H "Content-Type: application/x-www-form-urlencoded" '.
	\ '-d "grant_type=client_credentials"'
	\ )

  let g:nvimim#sptfy#token = json_decode(raw)
endfunction

function nvimim#sptfy#authorize()

  let scope = 'user-read-private+user-read-email'
  let redirect = 'http://localhost:8888/callback'
  let state = 'veryrandomstring'

  let url = 'https://accounts.spotify.com/authorize?'.
	\ 'response_type=code&'.
	\ 'client_id='.g:nvimim_sptfy_client_id.'&'.
	\ 'scope='.scope.'&'.
	\ 'redirect_uri='.redirect.'&'.
	\ 'state='.state

  lua require('nvimim/sptfy')
  call v:lua.sptfy_callback()
  "call nvimim#util#open_url(url)
  "let res = rpcrequest(g:nvimim#jobid, 'SptfyCallback')

endfunction

function nvimim#sptfy#callback(data)
  echo 'data: '.data
  let strs = split(a:data, ' ')
  let uri = nvimim#util#decode_uri(strs[1])
  echo 'uri: '.string(uri)

  let g:nvimim#sptfy#access_token = ''
endfunction

function nvimim#sptfy#request(endpoint)
  let url = 'https://api.spotify.com/v1/'.a:endpoint

  let raw = system('curl -s '.url.' '.
	\ '-X GET '.
	\ '-H "Authorization: Bearer '.g:nvimim#sptfy#access_token.'" '
	\ )

  return json_decode(raw)
endfunction
