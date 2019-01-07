local zmq = require"zmq"
require'tango' -- automatically import tango.ref ans tango.unref
local proxy = require'3rd.tango.proxy'
local default = require'3rd.tango.config'.client_default

module('tango.client.zmq')

local context

connect = 
  function(config)
    config = default(config)
    config.url = config.url or 'tcp://localhost:12345'
    -- prevent multiple calls to zmq.init!
    config.context = config.context or context or zmq.init(1)
    context = config.context
    local socket = config.context:socket(zmq.REQ)
    socket:connect(config.url)
    local serialize = config.serialize
    local unserialize = config.unserialize
    local send_request = 
      function(request)             
        local request_str = serialize(request)
        socket:send(request_str)
      end    
    local recv_response = 
      function()
        local response_str = socket:recv()
        return unserialize(response_str)    
      end    
    return proxy.new(send_request,recv_response)
  end

return {
  connect = connect
}
