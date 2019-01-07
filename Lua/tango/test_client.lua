local server_backend = arg[1]
local client_backend = arg[2]
local option = arg[3]

local tango = require'tango'
local config = {}
if option then
  if option == 'ssl' then
    config.sslparams = require'test_ssl_config'.client
  end
end

local connect = tango.client[client_backend].connect

local spawn_server = 
  function(backend,access_str)
    local cmd = [[
        lua test_server.lua %s %s %s &
        echo $!            
    ]]
    cmd = cmd:format(backend,access_str,option or '')
    local process = io.popen(cmd)
    local pid = process:read()
    if backend ~= 'zmq' then
      os.execute('sleep 1')
    end
    return {
      process = process,
      pid = pid,
      kill = function()
               os.execute('kill '..pid)
             end
    }
  end

local tests = 0
local failed = 0
local ok = 0
local test = function(txt,f)
               tests = tests + 1
               io.write(txt..' ... ')
               local ret = f()
               if ret and ret ~= false  then
                 ok = ok + 1
                 io.write('ok\n')
               else
                 failed = failed + 1
                 io.write('failed\n')
               end
             end

local server = spawn_server(server_backend,'rw')
local client = connect(config)

print('==============================')
print('running tests with:')
print('server backend:',server_backend)
print('client backend:',client_backend)
if option then
  print('option:',option)
end
print('------------------------------')

test('add test',
     function()
       return client.add(1,2)==3
     end)

test('echo test',
     function()
       local tab = {number=444,name='horst',bool=true}
       local tab2 = client.echo(tab)
       return tab.number==tab2.number and tab.name==tab2.name and tab.bool==tab2.bool
     end)

test('multiple return values',
     function()
       local a,b,c = 1.234,true,{el=11}
       local a2,b2,c2 = client.echo(a,b,c)
       return a==a2 and b==b2 and c.el==c2.el
     end)

test('string error test',
     function()
       local status,msg = pcall(function()client.strerror()end)
       return status==false and type(msg) == 'string' and msg:find('testmessage') 
     end)

test('custom error test',
     function()
       local errtab = {code=117}
       local status,errtab2 = pcall(function()client.customerror(errtab)end)
       return status==false and type(errtab2) == 'table' and errtab2.code==errtab.code
     end)

test('nested method name test',
     function()
       return client.nested.method.name()==true
     end)

test('tango.ref with io.popen',
     function()
       local pref = tango.ref(client.io.popen,'echo hello')
       local match = pref:read('*a'):find('hello')
       pref:close()
       tango.unref(pref)
       return match
     end)

test('tango.ref with person',
     function()
       local pref = tango.ref(client.person,'horst')
       pref:name('peter')
       local match = pref:name() == 'peter'
       tango.unref(pref)
       return match
     end)

test('creating and accessing variables with number',
     function()
       client.x(4)
       return client.x() == 4 and client.double_x() == 8
     end)

test('creating and accessing variables with tables',
     function()
       client.abc({sub='horst',tab={}})
       client.abc.tab.num(1234)
       local abc = client.abc()
       return type(abc) == 'table' and abc.sub == 'horst' and abc.tab.num == 1234
     end)

test('accessing not existing tables causes error',
     function()
       local ok,err = pcall(
         function()
           client.horst.dieter()
         end)
       return ok == false and err:find('horst.dieter')
     end)

server:kill()
server = spawn_server(server_backend,'r')
client = connect(config)

test('reading remote variable',
     function()         
       local d = client.data()
       return d.x == 0 and d.y == 3
     end)

test('writing remote variable causes error',
     function()
       local ok,err = pcall(
         function()
           client.data(33)
         end)
       return ok == false
     end)

server:kill()
server = spawn_server(server_backend,'w')
client = connect(config)

test('reading remote variable causes error',
     function()
       local ok,err = pcall(
         function()
           client.data()
         end)
       return ok == false
     end)

test('writing remote variable',
     function()
       local ok,err = pcall(
         function()
           client.data(33)
         end)
       return ok == true
     end)

server:kill()

print('--------------------------------')
print('#TESTS',tests)
print('#OK',ok)
print('#FAILED',failed)
print('================================')

