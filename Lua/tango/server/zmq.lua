local zmq = require"zmq"
zmq.poller = require'zmq.poller'
local serialize = require'3rd.tango.utils.serialization'.serialize
local unserialize = require'3rd.tango.utils.serialization'.unserialize
local dispatcher = require'3rd.tango.dispatcher'
local default = require'3rd.tango.config'.server_default
local print = print

module('tango.server.zmq')

local new = 
  function(config)
    config = default(config)
    config.url = config.url or 'tcp://*:12345'
    config.context = config.context or zmq.init(1)
    config.poller = config.poller or zmq.poller(2)
    local serialize = config.serialize
    local unserialize = config.unserialize
    local dispatcher = dispatcher.new(config)
    local socket = config.context:socket(zmq.REP)
    socket:bind(config.url)
    local poller = config.poller
    local response_str
    local handle_request
    local send_response = 
      function()
        socket:send(response_str)
        poller:modify(socket,zmq.POLLIN,handle_request)
      end    
    handle_request = 
      function()
        local request_str = socket:recv()
        if not request_str then
          socket:close()
          return 
        end
        local request = unserialize(request_str)
        local response = dispatcher:dispatch(request)
        response_str = serialize(response) 
        poller:modify(socket,zmq.POLLOUT,send_response)
      end
    
    poller:add(socket,zmq.POLLIN,handle_request)
    return config
  end


local loop = 
  function(config)     
    local server = new(config)
    server.poller:start()
    server.context:term()          
  end

return {
  loop = loop,
  new = new
}
