local tonumber = tonumber
local tostring = tostring
local error = error

module('tango.utils.socket_message')

local send = 
   function(socket,message)
      -- send length of the string as ascii line
      local sent,err = socket:send(tostring(#message)..'\n')
      if not sent then
         error(err)
      end
      -- send the actual message data
      sent,err = socket:send(message)
      if not sent then
         error(err)
      end
   end

local receive = 
   function(socket,on_error)
      local responselen,err = socket:receive('*l')
      if not responselen then
         error(err)
      end                         
      -- convert ascii len to number of bytes
      responselen = tonumber(responselen)
      if not responselen then
         error('length as ascii number string expected')
      end                       
      -- receive the actual response table dataa
      local response,err = socket:receive(responselen)
      if not response then
         error(err)
      end            
      return response
   end

return {
  send = send,
  receive = receive
}
