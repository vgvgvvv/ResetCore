local socket = require'socket'
local ev = require'ev'
local default_loop = ev.Loop.default
local send_message = require'3rd.tango.utils.socket_message'.send
local receive_message = require'3rd.tango.utils.socket_message'.receive
local dispatcher = require'3rd.tango.dispatcher'
local default = require'3rd.tango.config'.server_default
local pcall = pcall
local print = print

module('tango.server.ev_socket')

new = 
  function(config)  
    config = default(config)
    local serialize = config.serialize
    local unserialize = config.unserialize   
    local dispatcher = dispatcher.new(config)
    local server = socket.bind(config.interfaces or "*", 
                               config.port or 12345)
    return ev.IO.new(
      function(loop)        
        local client = server:accept()
        local response_str
        local send_response = ev.IO.new(
          function(loop, send_response)
            local ok,err = pcall(
              function()
                send_response:stop(loop)                  
                send_message(client,response_str)
              end)
            if not ok then
              print(err)
            end
          end,
          client:getfd(),
          ev.WRITE)
        
        ev.IO.new(
          function(loop, receive_request)
            receive_request:stop(loop)
            local ok,err = pcall(
              function()
                local request_str = receive_message(client)
                local request = unserialize(request_str)
                local response = dispatcher:dispatch(request)
                response_str = serialize(response)
                send_response:start(loop)              
                receive_request:start(loop)   
              end)
            if not ok then
              print(err)
            end
          end,
          client:getfd(),
          ev.READ):start(loop)
      end,
      server:getfd(),
      ev.READ)
  end

loop = function(config)
         new(config):start(default_loop)
         default_loop:loop()
       end



