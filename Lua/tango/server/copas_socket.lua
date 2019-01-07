local socket = require'socket'
local copas = require'3rd.tango.server.copas'
local coxpcall = require'3rd.tango.server.coxpcall'
local copcall = copcall
local pcall = pcall
local print = print
local send_message = require'3rd.tango.utils.socket_message'.send
local receive_message = require'3rd.tango.utils.socket_message'.receive
local dispatcher = require'3rd.tango.dispatcher'
local default = require'3rd.tango.config'.server_default
local require = require 
local ssl = nil

module('tango.server.copas_socket')

new = 
  function(config)
    config = default(config)
    config.pcall = copcall
    config.interface = config.interface or '*'
    config.port = config.port or 12345
    config.ssl_timeout = config.ssl_timeout or 10
    
    if config.sslparams then 
      ok,ssl = pcall(require,'ssl')
      if not ok then
        error(ssl)
      end
    end
    
      local request_loop =
      function(sock)
        sock:setoption('tcp-nodelay',true)
        
        if config.sslparams then
          sock = ssl.wrap(sock, config.sslparams)
          sock:settimeout(config.ssl_timeout, 't')
          ok,message = sock:dohandshake()
          if not ok then
            print('tango ssl handshake failed with:' .. message)
            return
          end
        end
        
        local wrapsock = copas.wrap(sock)        
        local dispatcher = dispatcher.new(config)        
        local serialize = config.serialize
        local unserialize = config.unserialize        
        
        local ok,err = copcall(
          function()
            while true do
              local request_str = receive_message(wrapsock)
              local request = unserialize(request_str)
              local response = dispatcher:dispatch(request)
              local response_str = serialize(response)
              send_message(wrapsock,response_str)
              wrapsock:flush()
            end
          end)
        if not ok then
          print(err)
        end
      end
    return socket.bind(config.interface,config.port),request_loop    
  end

loop = 
  function(config)
    copas.addserver(new(config))
    copas.loop()
  end

return {
  new = new,
  loop = loop
}


