local rawget = rawget
local rawset = rawset
local type = type
local error = error
local unpack = unpack
local setmetatable = setmetatable
local print = print

module('tango.proxy')


local function proxy_call(send_request,recv_response,method_name)
    return setmetatable(
      {
        method_name = method_name,
        send_request = send_request,
        recv_response = recv_response
      },
      {
        __index= 
          function(self,sub_method_name)
            -- look up if proxy already exists
            local proxy = rawget(self,sub_method_name)
            if not proxy then 
              local new_method_name
              if not method_name then
                new_method_name = sub_method_name
              else
                new_method_name = method_name..'.'..sub_method_name
              end
              -- create new call proxy
              proxy = proxy_call(send_request,recv_response,new_method_name)
              -- store for subsequent access
              rawset(self,sub_method_name,proxy)
            end                            
            return proxy
          end,        
        __call=
          function(self,...)
            send_request({method_name,...})
            local response = recv_response()
            if response[1] == true then
              return unpack(response,2)
            else
              error(response[2],2)
            end
          end
      })
  end

return {
  new = proxy_call
}

