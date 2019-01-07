local type = type
local error = error
local unpack = unpack
local tostring = tostring
local print = print

module('tango.dispatcher')

local error_msg = 
   function(var_name,err)
      local msg = 'tango server error "%s": %s' 
      return msg:format(var_name,err)
   end

local new = 
  function(config)        
    local d = {
      functab = config.functab,
      pcall = config.pcall,
      read_access = config.read_access,
      write_access = config.write_access,
      dispatch = 
        function(self,request)    
          local var = self.functab
          local var_name = request[1]
          local last_part
          local last_var
          for part in var_name:gmatch('[%w_]+') do
             last_part = part
             last_var = var
             if type(var) == 'table' then
                var = var[part]
            else
               return {false,error_msg(var_name,'no such variable')}
            end  
          end        
          if type(var) == 'function' then
             return {self.pcall(var,unpack(request,2))}
          else
             local val = request[2]
             if val then
                if not self.write_access then
                   return {false,error_msg(var_name,'no write_access')}
                else
                   return {self.pcall(
                              function()
                                 last_var[last_part] = val
                              end)}
                end
             else
                if not self.read_access then
                   return {false,error_msg(var_name,'no read_access')}
                else
                   return {true,var}
                end
             end
          end        
          
        end    
    }

    d.refs = {}
    d.functab.tango = d.functab.tango or {}
    
    d.functab.tango.ref_create = 
      function(create_method,...)
        local result = d:dispatch({create_method,...})
        if result[1] == true then
          local obj = result[2]
          if type(obj) == 'table' or type(obj) == 'userdata' then
            local id = tostring(obj)
            d.refs[id] = obj
            return id
          else
            error('tango.ref proxy did not create table nor userdata')
          end
        else
          error(result[2])
        end
      end

    d.functab.tango.ref_release = 
      function(refid)
        d.refs[refid] = nil
      end
    
    d.functab.tango.ref_call = 
      function(refid,method_name,...)
        local obj = d.refs[refid]
        if obj then
          return obj[method_name](obj,...)
        else
          error('tango.ref invalid id' .. refid)
        end          
      end    

    return d
  end

return {
  new = new
}
