local require = require
local globals = _G
local pcall = pcall

module('tango.config')

server_default = 
  function(config)
    config = config or {}
    config.functab = config.functab or globals
    config.serialize = config.serialize or require'3rd.tango.utils.serialization'.serialize
    config.unserialize = config.unserialize or require'3rd.tango.utils.serialization'.unserialize
    config.pcall = pcall
    if config.write_access == nil then
      config.write_access = true
    end
    if config.read_access == nil then
      config.read_access = true
    end
    return config
  end

client_default = 
  function(config)
    config = config or {}
    config.serialize = config.serialize or require'3rd.tango.utils.serialization'.serialize
    config.unserialize = config.unserialize or require'3rd.tango.utils.serialization'.unserialize
    return config
  end

return {
  server_default = server_default,
  client_default = client_default
}
    
