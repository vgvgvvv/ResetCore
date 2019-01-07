#!/usr/bin/lua
local run_client_test = 
   function(server_backend,client_backend,option)
     local cmd = 'lua test_client.lua %s %s %s'
     os.execute(cmd:format(server_backend,client_backend,option or ''))
   end

run_client_test('copas_socket','socket')
if pcall(require,'ssl') then
  run_client_test('copas_socket','socket','ssl')
end
run_client_test('ev_socket','socket')
run_client_test('zmq','zmq')



